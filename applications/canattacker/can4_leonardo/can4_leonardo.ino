// Can Attacker (Arduino Leonardo Version)
// dependency : rkistner/arcore (https://github.com/rkistner/arcore)


#define CAN_PIN 2
#define CHEEN_PIN 3
#define LED_PIN 13

#define NOTENUM1 38
#define NOTENUM2 36

int isCanPlay = 0;
int isCheenPlay = 0;
int cntCan = 0;
int cntCheen = 0;
int maxCheen = 1024;
int minCheen = 512;
int velCheen = minCheen;
int maxCan = 1168;
int minCan = 368;
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
  pinMode(CHEEN_PIN,OUTPUT);
  pinMode(CAN_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  uint8_t mes1;
  uint8_t mes2;
  
  while(MIDIUSB.available() > 0) {
    MIDIEvent e;
    e = MIDIUSB.read();
    mes1 = (e.m1 & 0xf0);
    if(mes1 == 0x90){
      onNoteOn((e.m1 & 0x0f),e.m2,e.m3);
    }
  }
  trigNoteOff();
}

