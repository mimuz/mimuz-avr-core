/*
  USB-MIDI Blink (When MIDI NOTE ON received)
  For ATTiny 44/84/45/85
*/

// ATTiny45/85 Pins
//              +------+
//         RESET|1    8|VCC
//      PB3/ADC3|2    7|ADC1/PB2 (USB D-)
//      PB4/ADC2|3    6|PB1      (USB D+)
//           GND|4    5|PB0 (LED)
//              +------+
// ATTiny44/84 Pins
//              +-------+
//           VCC|1    14|GND
//         XTAL1|2    13|PA0/ADC0 (USB D-)
//         XTAL2|3    12|PA1/ADC1 (USB D+) 
//         RESET|4    11|PA2/ADC2
// (LED)     PB2|5    10|PA3/ADC3
//      PA7/ADC7|6     9|PA4/ADC4/SCL/SCK
// MOSI/PA6/ADC6|7     8|PA5/ADC5/MISO
//              +-------+

#include "UsbMidiAttiny45in3.h"
#include <avr/wdt.h>

// LED 
#if defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) 
#define LED_INIT() (DDRB |= _BV(2))
#define LED_HIGH() (PORTB |= _BV(2))
#define LED_LOW() (PORTB &= ~_BV(2))
#elif defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) 
#define LED_INIT() (DDRB |= _BV(0))
#define LED_HIGH() (PORTB |= _BV(0))
#define LED_LOW() (PORTB &= ~_BV(0))
#endif

int isplay = 0;
int cnt = 0;
int maxvalue = 640;

void onNoteOn(byte ch, byte note, byte vel){
  isplay = 1;
  cnt = 0;
  LED_HIGH();
}

void trigNoteOff(){
  if(isplay == 1){
    cnt++;
    if(cnt >= maxvalue){
      isplay = 0;
      cnt = 0;
      LED_LOW();
    }
  }
}

void setup() {                
  LED_INIT();
  LED_HIGH();
  delayMs(10);
  LED_LOW();

  wdt_enable(WDTO_2S);
  UsbMidi.init();
  UsbMidi.setHdlNoteOn(onNoteOn);
}

void loop() {
  wdt_reset();
  UsbMidi.update();
  trigNoteOff();
}

void delayMs(unsigned int ms) {
  for( int i=0; i<ms; i++ ) {
    delayMicroseconds(1000);
  }
}

