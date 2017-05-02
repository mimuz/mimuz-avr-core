/*
  Simple MIDI Controller
  For BrushKnob v1 (ATTiny 45/85)

  (CC) 2016 by D.F.Mac.

  BrushKnob is licensed by @kmwtr
  https://github.com/kmwtr/BrushKnob
*/

/*
# BrushKnob v1 (ATtiny85 16.5Mhz)
#                        +------+
#       RESET     A0 PB5 |1    8| VCC
# (Encoder-A)     A3 PB3 |2    7| PB2 A1/INT0/SCL (USB D+)
# (Encoder-B)OC1B/A2 PB4 |3    6| PB1 OC0B/OC1A   (USB D-)
#                    GND |4    5| PB0 OC0A/SDA    (Switch)
#                        +------+
*/

#include "VUSBMidiATtiny.h"
#include <avr/wdt.h>

int maxvalue = 5;
int oVolume = -1;

#define SWITCH_PIN 0
#define ENCO_A_PIN 3
#define ENCO_B_PIN 4

void setup(){
  UsbMidi.init();
  pinMode(SWITCH_PIN,INPUT_PULLUP);
  pinMode(ENCO_A_PIN,INPUT_PULLUP);
  pinMode(ENCO_B_PIN,INPUT_PULLUP);
  
  wdt_enable(WDTO_2S);
}

int oSw = HIGH;
int oEncA = HIGH;
uint8_t val = 0;
boolean isPlay = false;
uint8_t targ = 0;

void loop() {
  int sw = HIGH;
  int encA = HIGH, encB = HIGH;
  uint8_t note = 0;
  
  wdt_reset();
  UsbMidi.update();
  
  note = val >> 1;
  
  sw = digitalRead(SWITCH_PIN);
  if((sw == LOW)&&(oSw == HIGH)){
    UsbMidi.sendNoteOn(1,val,100);
    isPlay = true;
  }else if((sw == HIGH)&&(oSw == LOW)){
    UsbMidi.sendNoteOff(1,val);
    isPlay = false;
  }
  oSw = sw;
  
  encA = digitalRead(ENCO_A_PIN);
  if((encA == LOW)&&(oEncA == HIGH)){
    encB = digitalRead(ENCO_B_PIN);
    if(encB == HIGH){
      targ = 1;
    }else{
      targ = 0;
    }
  }else if((encA == HIGH)&&(oEncA == LOW)){
    if(isPlay == true){
      UsbMidi.sendNoteOff(1,val);
    }
    if(targ == 1){
      val ++;
    }else{
      val --;
    }    
  }
  oEncA = encA;
  
  UsbMidi.delayMs(1);
}

