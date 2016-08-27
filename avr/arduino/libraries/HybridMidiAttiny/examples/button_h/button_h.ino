/*
  button (Hybrid MIDI Version)

  Support MCU and Boards are following as:

  mi:muz:expr#1 (ATtiny85)
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
#define I2C_SLAVE_ADDR  HMMAddr_0      // i2c slave address (0x30)

// Button PIN
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
#define BUTTON_PIN 2
#elif defined (__AVR_ATtiny85__) 
#define BUTTON_PIN 3
#endif

int obutton = LOW;

void setup(){
  HybridMidi.init(I2C_SLAVE_ADDR);
  pinMode(BUTTON_PIN,INPUT);
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  HybridMidi.update();
  
  int button = digitalRead(BUTTON_PIN);

  if((button == HIGH)&&(obutton != button)){
    ledOn();
    HybridMidi.sendCtlChange(0,0,127);
  }else if((button == LOW)&&(obutton != button)){
    ledOff();
    HybridMidi.sendCtlChange(0,0,0);
  }
  obutton = button;
  HybridMidi.delayMs(10);
}

