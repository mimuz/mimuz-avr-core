/*

HybridMidiAttiny

V-USB / I2C Slave hybrid MIDI Arduino Library for mi:muz:expr

based : 

[1] V-USB by Objective Development Software GmbH
  http://www.obdev.at/products/vusb/index.html

[2] vusb-for-arduino 
  https://code.google.com/p/vusb-for-arduino/downloads/list

[3] Atmel Attiny45 / Attiny85 based USB MIDI controller by Thorsten
  http://electronicsodyssey.blogspot.jp/2011/10/atmel-attiny45-attiny85-based-usb-midi.html

[4] TinyWireS
  http://playground.arduino.cc/Code/USIi2c
  https://github.com/rambo/TinyWire

[5] VUSBMidiATtiny
  https://github.com/tadfmac/mi-muz/tree/master/arduino/libraries/VUSBMidiATtiny

[6] TWISlave for ATtiny841

[1]->[2]
[3]+[2]->[5]
[5]+[4]+[6]-> 

Licenses:

	Under the GPLv2 License.

*/

#ifndef __hybridMidiAttiny_h__
#define __hybridMidiAttiny_h__

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/wdt.h>
#include "./utility/usbdrv.h"

#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
#include "./utility/i2c841s.h"
#include "pins_arduino.h"
#else
#include "./utility/TinyWireS.h"
#endif

#include "MimuzHelper.h"
#include "commonfunc.h"
#include "usbdef.h"
#include "queue2.h"

typedef uint8_t byte;

///////////
// queue
///////////

stB4Arrq txq;
stB4Arrq rxq;

///////////////////////////////////////////////////////////////////
// I2C
///////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C"{
#endif 

// I2C
#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)

void onRcv(uint8_t *pbuf, uint8_t size)
{
  handleMidiMessage(pbuf,size);
}

#else

volatile byte rcvbuf[8];

void receiveEvent(uint8_t howMany)
{
  byte cnt;
  byte cnt2 = 1;
  byte kind;
  byte *p;
  for(cnt = 0;cnt < howMany;cnt++){
    *(rcvbuf+cnt) = TinyWireS.receive();
  }
  handleMidiMessage((uchar *)rcvbuf,howMany);
}

volatile byte sendpos = 0;
volatile byte sendBuffer[] = {0xff,0xff,0xff,0xff};
volatile byte isDataReady = false;
void requestEvent(void)
{
	if(isDataReady){
    	TinyWireS.send(sendBuffer[sendpos]);
    	sendpos++;
    	if (sendpos >= 4){
        	sendpos = 0;
        	sendBuffer[0]= 0xff;
        	isDataReady = false;
        }
    }else{
    	sendpos = 0;
    	TinyWireS.send(0xff);
    }
}
#endif

///////////////////////////////////////////////////////////////////
// COMMON
///////////////////////////////////////////////////////////////////

void (*cbNoteOff)(byte ch, byte note, byte vel);
void (*cbNoteOn)(byte ch, byte note, byte vel);
void (*cbCtlChange)(byte ch, byte num, byte value);

byte checkMidiMessage(byte *pMidi){
  if(((*(pMidi + 1) & 0xf0)== 0x90)&&(*(pMidi + 3) != 0)){
    return 2;
  }else if(((*(pMidi + 1) & 0xf0)== 0x90)&&(*(pMidi + 3) == 0)){
    return 1;
  }else if((*(pMidi + 1) & 0xf0)== 0x80){
    return 1;
  }else if((*(pMidi + 1) & 0xf0)== 0xb0){
    return 3;
  }else{
    return 0;
  }
}

void handleMidiMessage(uchar *data, uchar len)
{
	b4arrq_push(&rxq,data);
	if(len > 4){
		b4arrq_push(&rxq,data+4);
	}
}

void processMidiMessage(){
	uint8_t *pbuf;
	uint8_t kindmessage;
	if(rxq.num > 0){
		pbuf = b4arrq_pop(&rxq);
		kindmessage = checkMidiMessage(pbuf);
		if(kindmessage == 1){
			if(cbNoteOff != NULL){
				(*cbNoteOff)(*(pbuf+1)&0x0f,*(pbuf+2)&0x7f,*(pbuf+3)&0x7f);
			}
		}else if(kindmessage == 2){
			if(cbNoteOn != NULL){
				(*cbNoteOn)(*(pbuf+1)&0x0f,*(pbuf+2)&0x7f,*(pbuf+3)&0x7f);
			}
		}else if(kindmessage == 3){
			if(cbCtlChange != NULL){
				(*cbCtlChange)(*(pbuf+1)&0x0f,*(pbuf+2)&0x7f,*(pbuf+3)&0x7f);
			}
		}
	}
}

#ifdef __cplusplus
} // extern "C"
#endif


///////////////////////////////////////////////////////////////////
// HybridMidiDevice Class
///////////////////////////////////////////////////////////////////

#define	MODE_I2C 0
#define MODE_USB 1

#define setI2CAddr(x) ((x)<<1)
#define setI2CAddrGCE(x) (((x)<<1) | 0x01)

#define HMMAddr_None	0x26
#define HMMAddr_0		0x30
#define HMMAddr_1		0x31
#define HMMAddr_2		0x32
#define HMMAddr_3		0x33
#define HMMAddr_4		0x34
#define HMMAddr_5		0x35
#define HMMAddr_6		0x36
#define HMMAddr_7		0x37

class HybridMidiDevice {

public:
	HybridMidiDevice () {
	}
  
  	void init(uint8_t addr){
		slaveaddr = addr;
		ledInit();
  		modeRead();
  		b4arrq_init(&rxq);
  		if(mode){
#if defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) 
			ACSR |= (1<<ACD); // Disable analog comparator
#elif defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
			ACSR0A |= (1<<ACD0); // Disable analog comparator
			ACSR1A |= (1<<ACD1); // Disable analog comparator
#endif
			cli();
#if defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)
			calibrateOscillator();
#endif
			usbInit();
			usbDeviceDisconnect();
			delayMs(250);
			usbDeviceConnect();
			cbNoteOff = NULL;
			cbNoteOn = NULL;
			cbCtlChange = NULL;
			sei();
  		}else{
#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
  			if(slaveaddr == 0x26){
				i2cs.init(setI2CAddrGCE(slaveaddr));
  			}else{
				i2cs.init(setI2CAddr(slaveaddr));
  			}
	  		i2cs.setOnReceived(onRcv);
#else
		    TinyWireS.begin(slaveaddr);      // init I2C Slave mode
		    TinyWireS.onReceive(receiveEvent);
		    TinyWireS.onRequest(requestEvent);
#endif
  		}
#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
		T841_TIMER_PINS_DEFAULT;
#endif
  	}

	void update() {
		processMidiMessage();
		if(mode){ // USB
			usbPoll();
		}else{	  // I2C
#if !(defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__))
  			TinyWireS_stop_check();
#endif
  		}
	}

	void check() {
		while (!usbInterruptIsReady()) {
			usbPoll();
		}
	}

	void sendNoteOn(byte ch, byte note, byte vel){
		buffer[0] = 0x09;
		buffer[1] = 0x90 | ch;
		buffer[2] = 0x7f & note;
		buffer[3] = 0x7f & vel;
		sendMidiMessage(buffer,4);
	}

	void sendNoteOff(byte ch, byte note){
		buffer[0] = 0x08;
		buffer[1] = 0x80 | ch;
		buffer[2] = 0x7f & note;
		buffer[3] = 0;
		sendMidiMessage(buffer,4);
	}

	void sendCtlChange(byte ch, byte num, byte value){
		buffer[0] = 0x0b;
		buffer[1] = 0xb0 | ch;
		buffer[2] = 0x7f & num;
		buffer[3] = 0x7f & value;
		sendMidiMessage(buffer,4);
	}

 	void setHdlNoteOff(void (*fptr)(byte ch, byte note, byte vel)){
 		cbNoteOff = fptr;
 	}

	void setHdlNoteOn(void (*fptr)(byte ch, byte note, byte vel)){
 		cbNoteOn = fptr;
	}

	void setHdlCtlChange(void (*fptr)(byte ch, byte num, byte value)){
 		cbCtlChange = fptr;
	}

	void delayMs(unsigned int ms){
		for( int i=0; i<ms; i++ ) {
#if !(defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__))
			if(!mode){
	  			TinyWireS_stop_check();
			}
#endif
	   		delayMicroseconds(1000);
		}
	}

private:
	byte buffer[4];
	uint8_t mode;
	uint8_t slaveaddr;

	void modeRead(){
  		int p;
		USBPLUS_DDR &= ~USBPLUS_MASK;

		delayMs(500);
  
		p = USBPLUS_PIN;
		if(p & USBPLUS_MASK){
			mode = MODE_I2C;
			ledOn();
			delayMs(300);
			ledOff();
			delayMs(300);
			ledOn();
			delayMs(300);
			ledOff();
			delayMs(300);
		}else{
			mode = MODE_USB;
			ledOn();
			delayMs(300);
			ledOff();
			delayMs(300);
		}
	}

	void sendMidiMessage(byte *data, byte size) {
		if(size != 4){
			return;	// This function is now support only 4 byte data.
		}
		if(mode){ // USB
			while (!usbInterruptIsReady()) {
				usbPoll();
			}
   			usbSetInterrupt(data, size);
   		}else{    // I2C
#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
    		i2cs.setSendData(data,size);
#else
    		setSendData(data,size);
#endif
   		}
 	}

#if !(defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__))
 	void setSendData(uint8_t *data, uint8_t size){
		cli();
		sendBuffer[0] = data[0];
		sendBuffer[1] = data[1];
		sendBuffer[2] = data[2];
		sendBuffer[3] = data[3];
		sendpos = 0;
		isDataReady = true;
		sei();
 	}
#endif
};

HybridMidiDevice HybridMidi;

#endif // __hybridMidiAttiny_h__
