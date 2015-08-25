 /*
  can atacker (Hybrid MIDI Version)
  
  Support MCU and Boards are following as:

  mi:muz:expr#1 (ATtiny45/85)

  (CC) 2015 by D.F.Mac.

*/

#include "HybridMidiAttiny.h"
#include <avr/wdt.h>

// I2C
#define I2C_SLAVE_ADDR  HMMAddr_None            // i2c slave address (0x26)

// LED 
#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
#define CAN_PIN 2
#define CHEEN_PIN 3
#elif defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) 
#define CAN_PIN 1
#define CHEEN_PIN 4
#endif

// Note Number
#define NOTENUM1 43
#define NOTENUM2 45

int isCanPlay = 0;
int isCheenPlay = 0;
int cntCan = 0;
int cntCheen = 0;
int maxCheen = 1024;
int minCheen = 512;
int velCheen = minCheen;
int maxCan = 1024;
int minCan = 512;
int velCan = minCan;

void onNoteOn(byte ch, byte note, byte vel){
  if(vel != 0){
    if(note == NOTENUM1){
      ledOn();
      velCheen = minCheen + ((maxCheen / 128) * vel); 
      cntCheen = 0;
      digitalWrite(CHEEN_PIN,HIGH);
      isCheenPlay = 1;
    }else if(note == NOTENUM2){
      ledOn();
      velCan = minCan + ((maxCan / 128) * vel); 
      cntCan = 0;
      digitalWrite(CAN_PIN,HIGH);
      isCanPlay = 1;
    }
  }
}

void trigNoteOff(){
  if(isCheenPlay == 1){
    cntCheen++;
    if(cntCheen >= velCheen){
      isCheenPlay = 0;
      cntCheen = 0;
      if(isCanPlay == 0){
        ledOff();
      }
      digitalWrite(CHEEN_PIN,LOW);
    }
  }
  if(isCanPlay == 1){
    cntCan++;
    if(cntCan >= velCan){
      isCanPlay = 0;
      cntCan = 0;
      if(isCheenPlay == 0){
        ledOff();
      }
      digitalWrite(CAN_PIN,LOW);
    }
  }
}

void setup() {                
  HybridMidi.init(I2C_SLAVE_ADDR);
  HybridMidi.setHdlNoteOn(onNoteOn);
  pinMode(CHEEN_PIN,OUTPUT);
  pinMode(CAN_PIN,OUTPUT);
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  HybridMidi.update();
  trigNoteOff();
}

