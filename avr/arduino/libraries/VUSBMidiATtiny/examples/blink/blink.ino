/*
  USB-MIDI Blink (When MIDI NOTE ON received)
  For ATTiny 45/85/44/84/841
  And ATmega328P

  (CC) 2015-2016 by D.F.Mac.
*/

/*
# mi:muz:prot#1
#                    +------+
#       RESET A0 PB5 |1    8| VCC
#          A3/D3/PB3 |2    7| PB2/D2/A1/INT0    (USB D-)
#     OC1B/A2/D4/PB4 |3    6| PB1/D1/OC0B/OC1A  (USB D+)
#                GND |4    5| PB0/D0/OC0A       (LED)
#                    +------+
#
# mi:muz:prot#2
#                     +-------+
#                  VCC|1    14|GND
#                XTAL1|2    13|PA0/D0/A0 (USB D-)
#                XTAL2|3    12|PA1/D1/A1 (USB D+) 
#                RESET|4    11|PA2/D2/A2
#        (LED)  D8/PB2|5    10|PA3/D3/A3
#            A7/D7/PA7|6     9|PA4/D4/A4/SCL/SCK
#   MOSI/SDA/A6/D6/PA6|7     8|PA5/D5/A5/MISO
#                     +-------+
#
# mi:muz:prot#3
#                       +------+
#          RESET A0 PB5 |1    8| VCC
# (USB D-)    A3/D3/PB3 |2    7| PB2/D2/A1/INT0    
# (LED)  OC1B/A2/D4/PB4 |3    6| PB1/D1/OC0B/OC1A  (USB D+)
#                   GND |4    5| PB0/D0/OC0A
#                       +------+
#
# mi:muz:expr-mega#P1
#
#                       +------+
#             RESET PC6 |1   28| PC5/A5 (USB D+)
#                   PD0 |2   27| PC4/A4 (USB D-)    
#                   PD1 |3   26| PC3/A3
#          (BUTTON) PD2 |4   25| PC2/A2
#                   PD3 |5   24| PC1/A1
#                   PD4 |6   23| PC0/A0 (CdS)
#                   VCC |7   22| GND
#                   GND |8   21| AREF
#                   PB6 |9   20| AVCC
#                   PB7 |10  19| PB5 (LED)
#                   PD5 |11  18| PB4
#                   PD6 |12  17| PB3
#                   PD7 |13  16| PB2
#                   PB0 |14  15| PB1
#                       +------+
*/


#include "VUSBMidiATtiny.h"
#include <avr/wdt.h>

int isplay = 0;
int cnt = 0;
int maxvalue = 640;

void onNoteOn(byte ch, byte note, byte vel){
  isplay = 1;
  cnt = 0;
  ledOn();
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

void setup(){
  UsbMidi.init();
  ledInit();
  ledOn();
  UsbMidi.delayMs(10);
  ledOff();
  UsbMidi.setHdlNoteOn(onNoteOn);
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  UsbMidi.update();
  trigNoteOff();
}

