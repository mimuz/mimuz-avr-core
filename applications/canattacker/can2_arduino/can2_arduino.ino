 /*
  can atacker (Arduino UNO Version)
  
  Support MCU and Boards are following as:

  Arduino UNO & mocoLUFA(https://github.com/kuwatay/mocolufa)

  (CC) 2015 by D.F.Mac.

*/

#include <avr/wdt.h>
#include <MIDI.h>  

MIDI_CREATE_DEFAULT_INSTANCE();

#define CAN_PIN 8
#define CHEEN_PIN 9
#define LED_PIN 13

// Note Number
#define NOTENUM1 46
#define NOTENUM2 47

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
      digitalWrite(LED_PIN,HIGH);
      velCheen = minCheen + ((maxCheen / 128) * vel); 
      cntCheen = 0;
      digitalWrite(CHEEN_PIN,HIGH);
      isCheenPlay = 1;
    }else if(note == NOTENUM2){
      digitalWrite(LED_PIN,HIGH);
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
        digitalWrite(LED_PIN,LOW);
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
        digitalWrite(LED_PIN,LOW);
      }
      digitalWrite(CAN_PIN,LOW);
    }
  }
}

void setup() {                
  MIDI.begin();
  MIDI.turnThruOff();
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI);
  pinMode(CHEEN_PIN,OUTPUT);
  pinMode(CAN_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  wdt_enable(WDTO_2S);
}

void loop() {
  wdt_reset();
  
  if(MIDI.read()) {
    if(MIDI.getType()==midi::NoteOn){
      onNoteOn(MIDI.getChannel()-1,MIDI.getData1(),MIDI.getData2());
    }
  }
  trigNoteOff();
}

