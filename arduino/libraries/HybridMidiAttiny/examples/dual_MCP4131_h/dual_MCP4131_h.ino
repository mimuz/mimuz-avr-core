 /*
  dual MCP4131 Potentiometer (Hybrid MIDI Version)
  
  Support MCU and Boards are following as:

  mi:muz:expr#2 (ATtiny44/841)

  (CC) 2015 by D.F.Mac.

*/

/*
# mi:muz:expr#2 (ATtiny44/841 16.0Mhz)
#                        +-------+
#                     VCC|1    14|GND
#                   XTAL1|2    13|PA0/A0
#                   XTAL2|3    12|PA1/A1 (MOSI) 
#                   RESET|4    11|PA2/A2 (SS_PIN1)
# (LED)               PB2|5    10|PA3/A3 (SCK)
#                  A7/PA7|6     9|PA4/A4/SCL/SCK (USB D+)
# (USB D-)MOSI/SDA/A6/PA6|7     8|PA5/A5/MISO (SS_PIN2)
#                        +-------+
*/

/*
# mi:muz:expr#2 PA1(MOSI)       --> MCP4131:PIN3
# mi:muz:expr#2 PA2/PA5(SS_PIN) --> MCP4131:PIN1
# mi:muz:expr#2 PA3(SCK)        --> MCP4131:PIN2    
# mi:muz:expr#2 GND             --> MCP4131:PIN4
# mi:muz:expr#2 5V              --> MCP4131:PIN8
                                    MCP4131:PIN5-7 (POT)
*/

#include "HybridMidiAttiny.h"
#include <avr/wdt.h>

// The following constants are defined in pins_arduino.h
// static const uint8_t MISO = 0;
// static const uint8_t MOSI = 1;
// static const uint8_t SS   = 2;
// static const uint8_t SCK  = 3;

#define SS_PIN1 SS
#define SS_PIN2 5 // additional chip select pin

void spi_begin(){
  REMAP = (1<<SPIMAP);
  DDRA |= ((1<<SS_PIN1)|(1<<SS_PIN2)|(1<<MOSI)|(1<<SCK));
  PORTA = (1<<SS_PIN1)|(1<<SS_PIN2);
  PUEA = (1<<SS_PIN1)|(1<<SS_PIN2);
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

void writeValue(uint8_t sspin,uint8_t value){
  digitalWrite(sspin,LOW);
  spi_transfer(0);
  spi_transfer(value);
  digitalWrite(sspin, HIGH);
}

// I2C
#define I2C_SLAVE_ADDR  HMMAddr_None            // i2c slave address (0x26)

int isplay = 0;
int maxvalue = 30;
int cnt = 0;

void onCtlChange(byte ch, byte num, byte value){
  isplay = 1;
  cnt = 0;
  ledOn();
  if(num == 1){ 
    writeValue(SS_PIN1,value);
  }else if(num == 2){
    writeValue(SS_PIN2,value);
  }
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



