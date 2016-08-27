/*
  mi:muz:tuch#1 v0.1 (For ATtiny44)
  
  MIDI Bridge 

  (CC) 2016 by D.F.Mac.

*/

/*
# mi:muz:expr#2 (ATtiny44/841 16.0Mhz)
#                        +-------+
#                     VCC|1    14|GND
#                   XTAL1|2    13|PA0/A0 (S0)
#                   XTAL2|3    12|PA1/A1 (S1)
#                   RESET|4    11|PA2/A2 (S2)
# (LED)               PB2|5    10|PA3/A3 (S3)
# (S5)             A7/PA7|6     9|PA4/A4/SCL/SCK (USB D+)
# (USB D-)MOSI/SDA/A6/PA6|7     8|PA5/A5/MISO (S4)
#                        +-------+
*/

#include "HybridMidiAttiny.h"
#include <avr/wdt.h>
#include <EEPROM.h>

// I2C
#define I2C_SLAVE_ADDR HMMAddr_1      // i2c slave address (0x33)
uint8_t pins[]  = {0,1,2,3,5,7};
uint8_t notes[] = {24,20,21,22,23,25,I2C_SLAVE_ADDR};  // x34 Ver.
uint8_t onoff[] = {0,0,0,0,0,0};
uint8_t ch = 0;
#define PINS_MAX 6

uint8_t mode = 0; // 0: none 1: write 2: read

#define ROMADDR_FLAG 15 // 8:stored, not 8:not stored

uint8_t ledTime;
#define LEDTIMEMAX 64

void onCtlChange(byte ch, byte num, byte value){
  uint8_t func,note;
  ledOn();
  ledTime =LEDTIMEMAX;
  if(ch == 15){
    func = num - 110;
    if(func == 9){        // 119
      mode = value;
    }else if(func < 7){  // 117= I2C Addr / Preset-number 110-116
      if(mode == 1){
        EEPROM.write(func,value);
        notes[func] = value;
      }else{  // mode == 2
        HybridMidi.sendCtlChange(15,119,1);
        HybridMidi.update();
        HybridMidi.sendCtlChange(15,num,notes[func]);
        HybridMidi.update();
      }
      mode = 0;
    }
  }
  wdt_reset();
}


void loadNums(){
  uint8_t cnt;
  uint8_t cntmax = PINS_MAX + 1; // note numbers + I2cSlaveAddr
  uint8_t store = EEPROM.read(ROMADDR_FLAG);
  if(store == 8){
    for(cnt = 0;cnt < cntmax;cnt ++){
      notes[cnt] = EEPROM.read(cnt);
    }
  }else{
    for(cnt = 0;cnt < cntmax;cnt ++){
      EEPROM.write(cnt,notes[cnt]);
    }
    EEPROM.write(ROMADDR_FLAG,8);
  }
}

void setup(){
  loadNums();
  HybridMidi.init(notes[PINS_MAX]);
  HybridMidi.setHdlCtlChange(onCtlChange);
  wdt_enable(WDTO_2S);
  DDRA &= ~((1 << PA7)|(1 << PA5)|(1 << PA3)|(1 << PA2)|(1 << PA1)|(1 << PA0));
}

void trigNoteOff(){
  if(ledTime > 0){
    ledTime --;
    if(ledTime == 0){
      ledOff();
    }
  }
}

void loop() {
  uint8_t p;
  uint8_t mask;
  wdt_reset();
  HybridMidi.update();

  mask = (1 << pins[ch]);
  p = PINA;

  trigNoteOff();
 
  if((p & mask) == mask){
    if(onoff[ch] == 0){
      onoff[ch] = 1;
      ledTime = LEDTIMEMAX;
      ledOn();
      HybridMidi.sendNoteOn(0,notes[ch],100);
    }
  }else if((p & mask) == 0){
    if(onoff[ch] == 1){
      onoff[ch] = 0;
      HybridMidi.sendNoteOff(0,notes[ch]);
    }
  }
  HybridMidi.delayMs(1);
  ch ++;
  if(ch >= PINS_MAX){
    ch = 0;
  }
}

