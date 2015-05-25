/*
  Simple MIDI Controller (one Volume only!!)
  For ATTiny 45/85/44/84/841

  (CC) 2015 by D.F.Mac.
*/

/*
# mi:muz:prot#1
#                    +------+
#       RESET A0 PB5 |1    8| VCC
#  (Volume)A3/D3/PB3 |2    7| PB2/D2/A1/INT0    (USB D-)
#     OC1B/A2/D4/PB4 |3    6| PB1/D1/OC0B/OC1A  (USB D+)
#                GND |4    5| PB0/D0/OC0A       (LED)
#                    +------+
#
# mi:muz:prot#2
#                     +-------+
#                  VCC|1    14|GND
#                XTAL1|2    13|PA0/D0/A0 (USB D-)
#                XTAL2|3    12|PA1/D1/A1 (USB D+) 
#                RESET|4    11|PA2/D2/A2 (Volume)
#        (LED)  D8/PB2|5    10|PA3/D3/A3
#            A7/D7/PA7|6     9|PA4/D4/A4/SCL/SCK
#   MOSI/SDA/A6/D6/PA6|7     8|PA5/D5/A5/MISO
#                     +-------+
#
# mi:muz:prot#3
#                       +------+
#          RESET A0 PB5 |1    8| VCC
# (USB D-)    A3/D3/PB3 |2    7| PB2/D2/A1/INT0 (Volume)    
# (LED)  OC1B/A2/D4/PB4 |3    6| PB1/D1/OC0B/OC1A  (USB D+)
#                   GND |4    5| PB0/D0/OC0A
#                       +------+
*/


#include "VUSBMidiATtiny.h"
#include <avr/wdt.h>

// LED 
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
#define LED_PIN 8
#elif defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) 
#if defined (ARDUINO_MIMUZ_PROT3)
#define LED_PIN 4
#elif defined (ARDUINO_MIMUZ_EXPR1)
#define LED_PIN 3
#else
#define LED_PIN 0
#endif
#endif

// Volume PIN
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
#define VOLUME_PIN 2
#elif defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) 
#if defined (ARDUINO_MIMUZ_PROT3)
#define VOLUME_PIN 1
#else
#define VOLUME_PIN 3
#endif
#endif

int isplay = 0;
int cnt = 0;
int maxvalue = 5;
int oVolume = -1;

void trigLedOff(){
  if(isplay == 1){
    cnt++;
    if(cnt >= maxvalue){
      isplay = 0;
      cnt = 0;
      digitalWrite(LED_PIN,LOW);
    }
  }
}

void setup(){
  UsbMidi.init();
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,HIGH);
  UsbMidi.delayMs(10);
  digitalWrite(LED_PIN,LOW);
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  UsbMidi.update();
  trigLedOff();
  
  int volume = analogRead(VOLUME_PIN);
  volume >>= 3;

  if((volume != oVolume)&&(oVolume != -1)){
    digitalWrite(LED_PIN,HIGH);
    cnt = 0;
    isplay = 1;
    UsbMidi.sendCtlChange(1,1,volume);
  }
  oVolume = volume;

  UsbMidi.delayMs(10);
}

