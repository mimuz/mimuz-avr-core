 /*
  blink (Hybrid MIDI Version)
  
  Support MCU and Boards are following as:

  mi:muz:expr#1 (ATtiny45/85)
  mi:muz:expr#2 (ATtiny44/841)

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
#include <avr/wdt.h>

// I2C
#define I2C_SLAVE_ADDR  HMMAddr_None            // i2c slave address (0x26)

int isplay = 0;
int maxvalue = 300;
int cnt = 0;

void onNoteOn(byte ch, byte note, byte vel){
  isplay = 1;
  cnt = 0;
  ledOn();
}

void onNoteOff(byte ch, byte note, byte vel){
  isplay = 0;
  cnt = 0;
  ledOff();
}

void trigNoteOff(){
  if(isplay == 1){
    cnt++;
    if(cnt >= maxvalue){
      isplay = 0;
      cnt = 0;
      ledOff();
    }
  }
}

void setup() {                
  HybridMidi.init(I2C_SLAVE_ADDR);
  HybridMidi.setHdlNoteOn(onNoteOn);
  HybridMidi.setHdlNoteOff(onNoteOff);
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  HybridMidi.update();
  trigNoteOff();
  HybridMidi.delayMs(10);
}

