 /*
  AD8403 Potentiometer (Hybrid MIDI Version)
  
  Support MCU and Boards are following as:

  mi:muz:expr#2 (ATtiny44/841)

  (CC) 2016 by D.F.Mac.

*/

/*
# mi:muz:expr#2 (ATtiny44/841 16.0Mhz)
#                        +-------+
#                     VCC|1    14|GND
#                   XTAL1|2    13|PA0/A0
#                   XTAL2|3    12|PA1/A1 (MOSI)    ->AD8403:12
#                   RESET|4    11|PA2/A2 (SS_PIN1) ->AD8403:11
# (LED)               PB2|5    10|PA3/A3 (SCK)     ->AD8403:14
#                  A7/PA7|6     9|PA4/A4/SCL/SCK (USB D+)
# (USB D-)MOSI/SDA/A6/PA6|7     8|PA5/A5/MISO
#                        +-------+
*/

#include "HybridMidiAttiny.h"
#include <avr/wdt.h>

// The following constants are defined in pins_arduino.h
// static const uint8_t MISO = 0;
// static const uint8_t MOSI = 1;
// static const uint8_t SS   = 2;
// static const uint8_t SCK  = 3;

#define SS_PIN1 2
#define MOSI_PIN 1
#define SCK_PIN 3

void spi_begin(){
  REMAP = (1<<SPIMAP);
  DDRA |= ((1<<SS_PIN1)|(1<<MOSI_PIN)|(1<<SCK_PIN));
  PORTA = (1<<SS_PIN1);
  PUEA = (1<<SS_PIN1);
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
}

uint8_t spi_transfer(uint8_t data){
  uint8_t res;
  SPDR = data;
  while(!(SPSR&(1<<SPIF))){
    wdt_reset();
  }
  res = SPDR;
  SPCR |= (1<<MSTR); // It would't be remove, this is very important!
  return res;
}

void writeValue(uint8_t channel,uint8_t value){
  digitalWrite(SS_PIN1,LOW);
  spi_transfer(0);
  spi_transfer(channel);
  spi_transfer(value);
  digitalWrite(SS_PIN1, HIGH);
}

// I2C
#define I2C_SLAVE_ADDR  HMMAddr_None            // i2c slave address (0x26)

int isplay = 0;
int maxvalue = 30;
int cnt = 0;

void onCtlChange(byte ch, byte num, byte value){
  uint8_t channel = num % 4;
  uint8_t val = value * 2;
  isplay = 1;
  cnt = 0;
  ledOn();
  writeValue(channel,val);
  wdt_reset();
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
  HybridMidi.setHdlCtlChange(onCtlChange);
  spi_begin();
  ledOn();
  HybridMidi.delayMs(50);
  ledOff();
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  HybridMidi.update();
  trigNoteOff();
  HybridMidi.delayMs(1);
}



