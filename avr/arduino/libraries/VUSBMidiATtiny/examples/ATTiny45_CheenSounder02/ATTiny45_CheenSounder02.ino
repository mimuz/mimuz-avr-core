//////////////////////////////////////////////////////////////
// ATTiny45 CheenSounder v2
// (CC) 2015 by D.F.Mac.
//////////////////////////////////////////////////////////////
// ATTiny 45/85 
//                     +------+
//       RESET D5 | A0 |      | VCC
// (Cheen OUT) D3 | A3 |      | A1 / D2 (USB D-)
//       (LED) D4 | A2 |      | D1      (USB D+)
//                 GND |      | D0 (Cheen IN)
//                     +------+
//
//////////////////////////////////////////////////////////////

#if defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)
#include "VUSBMidiATtiny.h"
#endif

#include <avr/wdt.h>

#define PORTD0 0x01
#define PORTD1 0x02
#define PORTD2 0x04
#define PORTD3 0x08
#define PORTD4 0x10
#define PORTD5 0x20

int isplay = 0;
boolean playing = false;
int cnt = 0;
int cnt2 = 0;
int maxvalue = 640;
int ButtonInterval = 1024;

void onNoteOn(byte ch, byte note, byte vel){
  isplay = 1;
  cnt = 0;
  PORTB |= PORTD3;
}

void trigNoteOff(){
  if(isplay == 1){
    cnt++;
    if(cnt >= maxvalue){
      isplay = 0;
      cnt = 0;
      PORTB &= ~PORTD3;
    }
  }
}

void setup() {
  wdt_enable(WDTO_2S);

  DIDR0 |= 0x18;               // ADC2D ADC3D Digital Input Disable
  DDRB &= ~PORTD0;             // D0 Input Mode
  DDRB |= (PORTD3 | PORTD4);   // D3, D4 Output Mode

  // reset LED 
  PORTB |= PORTD4;
  delayMs(200);
  PORTB &= ~PORTD4;
  delayMs(200);
  PORTB |= PORTD4;
  delayMs(200);
  PORTB &= ~PORTD4;
  delayMs(200);
  PORTB |= PORTD4;
  delayMs(200);
  PORTB &= ~PORTD4;
  
  TIMSK &= ~(1 << TOIE0);          //  disable timer 1 Interrupt

  UsbMidi.init();
  UsbMidi.setHdlNoteOn(onNoteOn);
}

void loop() {
  int p;

  wdt_reset();
  UsbMidi.update();
  trigNoteOff();

  cnt2 ++;
  if(cnt2 >= ButtonInterval){
    p = PINB;
    if(p & PORTD0){
      PORTB |= PORTD4;
      if(playing == false){
        UsbMidi.sendNoteOn(1,48,100);
      }
      playing = true;
    }else{
      PORTB &= ~PORTD4;
      if(playing == true){
        UsbMidi.sendNoteOff(1,48);
      }
      playing = false;
    }
    cnt2 = 0;
  }
}
 
void delayMs(unsigned int ms) {
  for( int i=0; i<ms; i++ ) {
    delayMicroseconds(1000);
  }
}
