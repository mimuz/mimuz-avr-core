// 
// USB to I2C MIDI bridge for Arduino UNO Rev.3 with mi:muz:mstr-uno v0.1
// 
// Author       : D.F.Mac. @TripArts Music
// License      : Creative Commons Attribution (https://creativecommons.org/licenses/by/4.0/)
// Dependencies : Arduino IDE 1.6.3 or later (https://www.arduino.cc/en/main/software)
//              : Arduino MIDI Library by @fortysevenfx (https://github.com/FortySevenEffects/arduino_midi_library/)
//              : mocoLUFA by @morecat_lab (https://github.com/kuwatay/mocolufa)
//              : and mi:muz Project by D.F.Mac. @TripArts Music (https://github.com/tadfmac/mi-muz)
// Hardware     : Arduino UNO rev.3 (https://www.arduino.cc/en/Main/arduinoBoardUno)
//              : mi:muz:mstr-uno v0.1
// Last Release : 2015.07.25
// 
// I2C Slave Channels are: 
//
//   0x26       : For MIDI Sounder
//      Slave devices can be received MIDI message broadcasting from master.
//      This number can be used in multiple slave devices, but devices can't be sending MIDI messages.
//  
//   0x30-0x37  : For MIDI Controller
//      Sleve devices can be sending MIDI messages to master, but devices must be 
//      choose independent number between in 0x30 t0 0x37.
//      Devices can't be use same number in same i2c port.
//

#include <Wire.h>
#include <avr/wdt.h>
#include <MIDI.h>  

MIDI_CREATE_DEFAULT_INSTANCE();

const uint8_t ADDR_NONE = 0x26; 
const uint8_t ADDR_MIN = 0x30; 
const uint8_t ADDR_MAX = 0x37; 

uint8_t checkaddr;
uint8_t READ_INTERVAL = 64;
uint8_t read_cnt; 

// Pin definition
#define LED_x26 10
#define LED_x30 2
#define LED_x31 3
#define LED_x32 4
#define LED_x33 5
#define LED_x34 6
#define LED_x35 7
#define LED_x36 8
#define LED_x37 9
#define LED_TEST 13

void setup() {                
  pinMode(LED_x26,OUTPUT);
  pinMode(LED_x30,OUTPUT);
  pinMode(LED_x31,OUTPUT);
  pinMode(LED_x32,OUTPUT);
  pinMode(LED_x33,OUTPUT);
  pinMode(LED_x34,OUTPUT);
  pinMode(LED_x35,OUTPUT);
  pinMode(LED_x36,OUTPUT);
  pinMode(LED_x37,OUTPUT);
  Wire.begin();
  MIDI.begin();
  MIDI.turnThruOff();
  MIDI.setInputChannel(MIDI_CHANNEL_OMNI);
  read_cnt = READ_INTERVAL;
  checkaddr = ADDR_MIN;
  wdt_enable(WDTO_2S);
}

uint8_t cnt26 = 0;
uint8_t cnt3x[8] = {0,0,0,0,0,0,0,0};
uint8_t wcnt = 0;
uint8_t wbuf[4];
const uint8_t LED_LIGHT_TIME = 128;

uint8_t checkReadData(){
  uint8_t ret = 0;
  if(wbuf[0] != 0xff){
    if(wbuf[1] != 0xff){
      if(wbuf[2] != 0xff){
        if(wbuf[3] != 0xff){
          ret = 1;
        }
      }
    }
  }
  return ret;
}

void loop() {
  uint8_t num = 0;
  int cnt1 = 0,cnt2 = 0;

  wdt_reset();

  // USB to I2C (broadcast)
  if(MIDI.read()) {
    int kind = MIDI.getType();
    if((kind==midi::NoteOn)||(kind==midi::NoteOff)||(kind==midi::ControlChange)){
      uint8_t mididata[4];
      mididata[0] = (kind >> 4);
      mididata[1] = kind | (MIDI.getChannel()-1); // (1-16)->(0-15)
      mididata[2] = MIDI.getData1();
      mididata[3] = MIDI.getData2();
      digitalWrite(LED_x26,HIGH);
      Wire.beginTransmission(0);  // the message will be broadcasted to devices has same address.
      Wire.write(mididata[0]);
      Wire.write(mididata[1]);
      Wire.write(mididata[2]);
      Wire.write(mididata[3]);
      Wire.endTransmission();
      cnt26 = LED_LIGHT_TIME;
    }
  }

  // I2C to USB
  read_cnt --;
  if(read_cnt == 0){
    read_cnt = READ_INTERVAL;
    num = Wire.requestFrom(checkaddr,(uint8_t)4);
    if(num !=0){
      wcnt = 0;
      while(Wire.available()){
        wbuf[wcnt%4] = Wire.read();
        wcnt++;
        if(wcnt >= 4){
          break;
        }
      }
      if(checkReadData()){
        cnt1 = checkaddr-ADDR_MIN;
        cnt3x[cnt1] = LED_LIGHT_TIME;
        digitalWrite(2+cnt1,HIGH);
        MIDI.send((midi::MidiType)(wbuf[1] & 0xf0),wbuf[2],wbuf[3],(wbuf[1]&0x0f)+1);
      }
    }
    checkaddr ++;
    if(checkaddr > ADDR_MAX){
      checkaddr = ADDR_MIN;
    }
  }
  // LED turn off
  if(cnt26 > 0){
    cnt26 --;
    if(cnt26 == 0){
      digitalWrite(LED_x26,LOW);
    }
  }
  for(cnt2 = 0;cnt2 < 8;cnt2++){
    if(cnt3x[cnt2] > 0){
      cnt3x[cnt2] --;
      if(cnt3x[cnt2] == 0){
        digitalWrite(2+cnt2,LOW);
      }
    }
  }

  delayMicroseconds(5);
}

