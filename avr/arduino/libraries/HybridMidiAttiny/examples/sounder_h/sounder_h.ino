/*
  Sounder #1 (Hybrid MIDI Version)  

  Features:
   CC:1 = Distotion Mode
   CC:2 = Portamento Time
   CC:3 = Sustain Time
   CC:4 = LFO Time

  Support MCU and Boards are following as:

  mi:muz:expr#1 (ATtiny85)
  mi:muz:expr#2 (ATtiny841)

  (CC) 2015 by D.F.Mac.

*/

/*
# mi:muz:expr#1 (ATtiny85 16.5Mhz)
#                        +-------+
#           RESET A0 PB5 |1     8| VCC
# (LED)           A3 PB3 |2     7| PB2 A1/SCL    (USB D+)
#            OC1B A2 PB4 |3     6| PB1 OC0B/OC1A
#                    GND |4     5| PB0 OC0A/SDA  (USB D-)
#                        +-------+
#
# mi:muz:expr#2 (ATtiny44/841 16.0Mhz)
#                        +-------+
#                     VCC|1    14|GND
#                   XTAL1|2    13|PA0/A0
#                   XTAL2|3    12|PA1/A1 
#                   RESET|4    11|PA2/A2
# (LED)               PB2|5    10|PA3/A3
#                  A7/PA7|6     9|PA4/A4/SCL/SCK (USB D+)
# (USB D-)MOSI/SDA/A6/PA6|7     8|PA5/A5/MISO
#                        +-------+
*/

#include "HybridMidiAttiny.h"
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__) 
#include "pins_arduino.h"  // for T841_TIMER_ENABLE_OCxx
#endif

#define PORTD0 0x01
#define PORTD1 0x02
#define PORTD2 0x04
#define PORTD3 0x08
#define PORTD4 0x10
#define PORTD5 0x20

// MIDI-CH (0-15)
#define MIDI_CH 0 // MIDI CH (10)

// I2C
#define I2C_SLAVE_ADDR  HMMAddr_None      // i2c slave address (38)

PROGMEM const unsigned int freqs[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,33,35,37,39,41,44,46,49,
  52,55,58,62,65,69,73,78,82,87,93,98,104,110,117,123,
  131,139,147,156,165,175,185,196,208,220,233,247,262,277,294,311,
  330,349,370,392,415,440,466,494,523,554,587,623,659,698,740,784,
  831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,1976,
  2093,2217,2349,2489,2637,2794,2960,3136,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

PROGMEM const uchar sine256[] = {
  127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
  242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,
  221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
  76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
  33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124
};

int dfreq;
volatile unsigned int phaccu;
volatile unsigned int tword_m;
volatile unsigned int r = 16; // >> 7
volatile byte icnt;
volatile byte volume = 0;

#define SHIFT_BASE 8
volatile byte shiftvalue = SHIFT_BASE;
volatile byte level = 1;

volatile int levVol;

#define FADE_MAX 255
unsigned int fade_time = 1;
byte lastnote = 0;
unsigned int fadecnt = 0;

void fadeout(void){
 if(fadecnt != 0){
    fadecnt --;
    if(fadecnt == 0){
      volume --;
      if(volume != 0){
        fadecnt = fade_time * (1+ (FADE_MAX / volume));
      }
    }
  }
}

#define PORTA_MAX 255
#define DIV_PORTA 32
byte porta_time = PORTA_MAX;

byte portacnt = 0;
byte divcnt = 0;
int tfreq = 0;
int sfreq = 0;
boolean ex = true;
boolean ey = true;
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__) 
boolean ez = true;
boolean ez2 = true;
#endif

void portamento(void){
 if(portacnt != 0){
    portacnt --;
    if(portacnt == 0){
      divcnt ++;
      if(divcnt < DIV_PORTA){
        dfreq += sfreq;
        tword_m=r*dfreq;
        portacnt = porta_time;
      }else{
        dfreq = tfreq;
        tword_m=r*dfreq;
        divcnt = 0;
      }
    }
  }    
}

#define LFO_MAX 255
#define DIV_LFO 8
byte lfo_time = LFO_MAX;
byte lfocnt = 0;
byte divlfo = 0;

void lfo(){
 if(lfocnt != 0){
   lfocnt --;
   if(lfocnt == 0){
     lfocnt = lfo_time;
     if(divlfo < DIV_LFO){
       divlfo ++;
     }else{
       divlfo = 0;
     }
     shiftvalue = SHIFT_BASE + divlfo;
   }
 }
}

void onNoteOn(byte ch, byte note, byte vel){
  if(MIDI_CH != ch){
    return;
  }

  fadecnt = 0;
  ledOn();
  tfreq = pgm_read_word(freqs + note);
  sfreq = (tfreq - dfreq) / DIV_PORTA;
  if(volume == 0){
    phaccu = 0xffffffff - (r*sfreq);
  }
  volume = 255;
  portacnt = PORTA_MAX;
  lastnote = note;
}

void onNoteOff(byte ch, byte note, byte vel){
  if(MIDI_CH != ch){
    return;
  }

  if(note == lastnote){
    ledOff();
    fadecnt = FADE_MAX;
  }
}

void onCtlChange(byte ch, byte num, byte value){
  if(num == 1){
    level = (value / 32) + 1;
  }else if(num == 2){
    porta_time = 1+(value * 4);
  }else if(num == 3){
    fade_time = value+1;
  }else if(num == 4){
    if(value < 8){
      lfo_time = 0;
    }else{
      lfo_time = (value >> 3) << 7;
    }
    lfocnt = lfo_time;
    divlfo = DIV_LFO;
    shiftvalue = SHIFT_BASE;
  }
}

void setup() {
  HybridMidi.init(I2C_SLAVE_ADDR);
  HybridMidi.setHdlNoteOff(onNoteOff);
  HybridMidi.setHdlNoteOn(onNoteOn);
  HybridMidi.setHdlCtlChange(onCtlChange);

  dfreq = pgm_read_word(freqs + 40);
  tword_m=r*dfreq;
  volume = 0;

#if defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) 
  DDRB |= (PORTD4);                // D4(OSC1)
  TCCR1 = (1<<CS11)|(1<<CS10);     // 0x03:Prescaler=x4 
  GTCCR = (1<<PWM1B)|(1<<COM1B1);  // 0x40:PWM1B = OCR1B PWM Mode | 0x20:OCR1B ClearCompareMatch
  TIMSK &= ~(1 << TOIE0);          // disable timer 1 Interrupt
  TIMSK |= (1 << TOIE1);           // enable timer 1 Interrupt
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__) 
  DDRA |= 0x20;                    // PA5 (OC1B)
#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__) 
  T841_TIMER_ENABLE_OC1B;          // ENABLE OC1B PIN (Enable TOCC4. See pins_arduino.h)
                                   // IT SHOULD CALL AFTER UsbMidi.init()!!! 
#endif
  TCCR1A = (1<<COM1B1)|(1<<WGM10); // OC1B PWM, PWM(WGM10)
  TCCR1B = (1<<WGM12)|(1<<CS10);   // Prescaler=x1
  TIMSK0 &= ~(1 << TOIE0);         // disable timer 1 Interrupt
  TIMSK1 |= (1 << TOIE1);          // enable timer 1 Interrupt
  OCR1BL = 0;
#endif
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  HybridMidi.update();
  portamento();
  fadeout();
  lfo();
  levVol = level*volume;
}

#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
ISR(TIM1_OVF_vect){
#else
ISR(TIMER1_OVF_vect){
#endif
  sei();
  int v;
  ex ^= 0x01;
  if(ex){
    ey ^= 0x01;
    if(ey){
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__) 
      ez ^=0x01;
      if(ez){
        ez2 ^=0x01;
        if(ez2){
#endif
cli();
          phaccu=phaccu+tword_m;
          icnt=phaccu >> 8;
          v = (int)pgm_read_byte_near(sine256 + icnt);  
sei();
#if defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) 
          OCR1B = (byte)((v*levVol) >> shiftvalue);
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__) 
          OCR1BL = (byte)((v*levVol) >> shiftvalue);
#endif
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__) 
        }
      }
#endif
    }
  }
}


