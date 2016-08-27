/*
  con8a0d MIDI Controller (Hybrid MIDI Version)  

  Support MCU and Boards are following as:

  mi:muz:expr#2 (ATtiny44/841)

  (CC) 2015 by D.F.Mac.

*/

/*
# mi:muz:expr#2 (ATtiny44/841 16.0Mhz)
#                        +-------+
#                     VCC|1    14|GND
#                   XTAL1|2    13|PA0/A0 (S0)
#                   XTAL2|3    12|PA1/A1 (S1)
#                   RESET|4    11|PA2/A2 (S2)
# (LED)               PB2|5    10|PA3/A3 (AIN)
#                  A7/PA7|6     9|PA4/A4/SCL/SCK (USB D+)
# (USB D-)MOSI/SDA/A6/PA6|7     8|PA5/A5/MISO (MODE)
#                        +-------+
*/

#include "HybridMidiAttiny.h"
#include <avr/wdt.h>

// I2C
#define I2C_SLAVE_ADDR  HMMAddr_2      // i2c slave address (0x32)

// PIN Number
#define AIN_PIN 3
#define MODE_PIN 5

uint8_t isplay1 = 0;
int cnt1 = 0;
int maxvalue = 50;
int oVolume1 = -1;
int obutton = LOW;

uint8_t channel = 0;
int values[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
const uint8_t numbers[8] = {3,2,1,0,4,5,6,7};
const uint8_t types[8] = {1,1,1,1,1,1,1,1}; // 0:button, 1: volume

int omode = -1;
uint8_t mode_value = 0;

int aread127(int port, int avalue){
  int value = analogRead(port);
  value >>= 3;
  if(avalue != -1){
    value = ((avalue*3)+value)/4;
    value *= 103;  // for 100ohm register
    value /= 100;
    if(value > 127){
      value = 127;
    }
  }

  return value;
}

// 4051 (S0=D0, S1=D1, S2=D2)
void initChannel(){
  DDRA |= 0x07;
}

void setChannel(uint8_t ch){
  PORTA &= ~0x07;
  PORTA |= (ch & 0x07);
}

void trigLedOff(){
  if(isplay1 == 1){
    cnt1++;
    if(cnt1 >= maxvalue){
      isplay1 = 0;
      cnt1 = 0;
      ledOff();
    }
  }
}

void allnoteoff(){
  uint8_t i;
  for(i = 0;i < 8;i++){
    if(types[i] == 0){
      HybridMidi.sendNoteOff(0,numbers[i]+mode_value);
    }
  }
  ledOff();
}

void setup(){
  HybridMidi.init(I2C_SLAVE_ADDR);
  ledOn();
  HybridMidi.delayMs(200);
  ledOff();
  initChannel();
  wdt_enable(WDTO_2S);
}


void loop() {
  int value;
  int mode;
  wdt_reset();
  HybridMidi.update();
  trigLedOff();

  setChannel(channel);
  
  mode = digitalRead(MODE_PIN);
  if((mode == HIGH)&&(mode != omode)){
    allnoteoff();
    mode_value = 0;
  }else if((mode == LOW)&&(mode != omode)){
    allnoteoff();
    mode_value = 8;
  }
  omode = mode;
  
  if(types[channel] == 0){
    pinMode(AIN_PIN,INPUT);
    value = digitalRead(AIN_PIN);
    if((value == HIGH)&&(value != values[channel])){
      ledOn();
      HybridMidi.sendNoteOn(0,numbers[channel]+mode_value,100);
    }else if((value == LOW)&&(value != values[channel])){
      ledOff();
      HybridMidi.sendNoteOff(0,numbers[channel]+mode_value);
    }
  }else if(types[channel] == 1){
    value = aread127(AIN_PIN,values[channel]);
    if((value != values[channel])&&(values[channel] != -1)){
      ledOn();
      cnt1 = 0;
      isplay1 = 1;
      HybridMidi.sendCtlChange(0,numbers[channel]+mode_value,value);
    }
  }
  values[channel] = value;
  channel ++;
  channel &= 0x07;
  HybridMidi.delayMs(1);
}

