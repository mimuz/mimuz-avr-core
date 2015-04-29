/*

based : 

- V-USB by Objective Development Software GmbH
	http://www.obdev.at/products/vusb/index.html

- vusb-for-arduino 
  https://code.google.com/p/vusb-for-arduino/downloads/list

- Atmel Attiny45 / Attiny85 based USB MIDI controller by Thorsten
  http://electronicsodyssey.blogspot.jp/2011/10/atmel-attiny45-attiny85-based-usb-midi.html

Licenses:

	Under the GNU License.
	See License.txt.

*/

#ifndef __VUSBMidiATtiny_h__
#define __VUSBMidiATtiny_h__

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
//#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "usbdrv.h"

typedef uint8_t byte;

// This descriptor is based on http://www.usb.org/developers/devclass_docs/midi10.pdf
// 
// Appendix B. Example: Simple MIDI Adapter (Informative)
// B.1 Device Descriptor
//
PROGMEM const char deviceDescrMIDI[] = {	/* USB device descriptor */
	18,			/* sizeof(usbDescriptorDevice): length of descriptor in bytes */
	USBDESCR_DEVICE,	/* descriptor type */
	0x10, 0x01,		/* USB version supported */
	0,			/* device class: defined at interface level */
	0,			/* subclass */
	0,			/* protocol */
	8,			/* max packet size */
	USB_CFG_VENDOR_ID,	/* 2 bytes */
	USB_CFG_DEVICE_ID,	/* 2 bytes */
	USB_CFG_DEVICE_VERSION,	/* 2 bytes */
	1,			/* manufacturer string index */
	2,			/* product string index */
	0,			/* serial number string index */
	1,			/* number of configurations */
};

// B.2 Configuration Descriptor
PROGMEM const char configDescrMIDI[] = {	/* USB configuration descriptor */
	9,			/* sizeof(usbDescrConfig): length of descriptor in bytes */
	USBDESCR_CONFIG,	/* descriptor type */
	101, 0,			/* total length of data returned (including inlined descriptors) */
	2,			/* number of interfaces in this configuration */
	1,			/* index of this configuration */
	0,			/* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
	USBATTR_SELFPOWER,	/* attributes */
#else
	USBATTR_BUSPOWER,	/* attributes */
#endif
	USB_CFG_MAX_BUS_POWER / 2,	/* max USB current in 2mA units */

// B.3 AudioControl Interface Descriptors
// The AudioControl interface describes the device structure (audio function topology) 
// and is used to manipulate the Audio Controls. This device has no audio function 
// incorporated. However, the AudioControl interface is mandatory and therefore both 
// the standard AC interface descriptor and the classspecific AC interface descriptor 
// must be present. The class-specific AC interface descriptor only contains the header 
// descriptor.

// B.3.1 Standard AC Interface Descriptor
// The AudioControl interface has no dedicated endpoints associated with it. It uses the 
// default pipe (endpoint 0) for all communication purposes. Class-specific AudioControl 
// Requests are sent using the default pipe. There is no Status Interrupt endpoint provided.
	/* AC interface descriptor follows inline: */
	9,			/* sizeof(usbDescrInterface): length of descriptor in bytes */
	USBDESCR_INTERFACE,	/* descriptor type */
	0,			/* index of this interface */
	0,			/* alternate setting for this interface */
	0,			/* endpoints excl 0: number of endpoint descriptors to follow */
	1,			/* */
	1,			/* */
	0,			/* */
	0,			/* string index for interface */

// B.3.2 Class-specific AC Interface Descriptor
// The Class-specific AC interface descriptor is always headed by a Header descriptor 
// that contains general information about the AudioControl interface. It contains all 
// the pointers needed to describe the Audio Interface Collection, associated with the 
// described audio function. Only the Header descriptor is present in this device 
// because it does not contain any audio functionality as such.
	/* AC Class-Specific descriptor */
	9,			/* sizeof(usbDescrCDC_HeaderFn): length of descriptor in bytes */
	36,			/* descriptor type */
	1,			/* header functional descriptor */
	0x0, 0x01,		/* bcdADC */
	9, 0,			/* wTotalLength */
	1,			/* */
	1,			/* */

// B.4 MIDIStreaming Interface Descriptors

// B.4.1 Standard MS Interface Descriptor
	/* interface descriptor follows inline: */
	9,			/* length of descriptor in bytes */
	USBDESCR_INTERFACE,	/* descriptor type */
	1,			/* index of this interface */
	0,			/* alternate setting for this interface */
	2,			/* endpoints excl 0: number of endpoint descriptors to follow */
	1,			/* AUDIO */
	3,			/* MS */
	0,			/* unused */
	0,			/* string index for interface */

// B.4.2 Class-specific MS Interface Descriptor
	/* MS Class-Specific descriptor */
	7,			/* length of descriptor in bytes */
	36,			/* descriptor type */
	1,			/* header functional descriptor */
	0x0, 0x01,		/* bcdADC */
	65, 0,			/* wTotalLength */

// B.4.3 MIDI IN Jack Descriptor
	6,			/* bLength */
	36,			/* descriptor type */
	2,			/* MIDI_IN_JACK desc subtype */
	1,			/* EMBEDDED bJackType */
	1,			/* bJackID */
	0,			/* iJack */

	6,			/* bLength */
	36,			/* descriptor type */
	2,			/* MIDI_IN_JACK desc subtype */
	2,			/* EXTERNAL bJackType */
	2,			/* bJackID */
	0,			/* iJack */

//B.4.4 MIDI OUT Jack Descriptor
	9,			/* length of descriptor in bytes */
	36,			/* descriptor type */
	3,			/* MIDI_OUT_JACK descriptor */
	1,			/* EMBEDDED bJackType */
	3,			/* bJackID */
	1,			/* No of input pins */
	2,			/* BaSourceID */
	1,			/* BaSourcePin */
	0,			/* iJack */

	9,			/* bLength of descriptor in bytes */
	36,			/* bDescriptorType */
	3,			/* MIDI_OUT_JACK bDescriptorSubtype */
	2,			/* EXTERNAL bJackType */
	4,			/* bJackID */
	1,			/* bNrInputPins */
	1,			/* baSourceID (0) */
	1,			/* baSourcePin (0) */
	0,			/* iJack */


// B.5 Bulk OUT Endpoint Descriptors

//B.5.1 Standard Bulk OUT Endpoint Descriptor
	9,			/* bLenght */
	USBDESCR_ENDPOINT,	/* bDescriptorType = endpoint */
	0x1,			/* bEndpointAddress OUT endpoint number 1 */
	3,			/* bmAttributes: 2:Bulk, 3:Interrupt endpoint */
	8, 0,			/* wMaxPacketSize */
	10,			/* bIntervall in ms */
	0,			/* bRefresh */
	0,			/* bSyncAddress */

// B.5.2 Class-specific MS Bulk OUT Endpoint Descriptor
	5,			/* bLength of descriptor in bytes */
	37,			/* bDescriptorType */
	1,			/* bDescriptorSubtype */
	1,			/* bNumEmbMIDIJack  */
	1,			/* baAssocJackID (0) */


//B.6 Bulk IN Endpoint Descriptors

//B.6.1 Standard Bulk IN Endpoint Descriptor
	9,			/* bLenght */
	USBDESCR_ENDPOINT,	/* bDescriptorType = endpoint */
	0x81,			/* bEndpointAddress IN endpoint number 1 */
	3,			/* bmAttributes: 2: Bulk, 3: Interrupt endpoint */
	8, 0,			/* wMaxPacketSize */
	10,			/* bIntervall in ms */
	0,			/* bRefresh */
	0,			/* bSyncAddress */

// B.6.2 Class-specific MS Bulk IN Endpoint Descriptor
	5,			/* bLength of descriptor in bytes */
	37,			/* bDescriptorType */
	1,			/* bDescriptorSubtype */
	1,			/* bNumEmbMIDIJack (0) */
	3,			/* baAssocJackID (0) */
};

uchar usbFunctionDescriptor(usbRequest_t * rq)
{
	if (rq->wValue.bytes[1] == USBDESCR_DEVICE) {
		usbMsgPtr = (uchar *) deviceDescrMIDI;
		return sizeof(deviceDescrMIDI);
	} else {		/* must be config descriptor */
		usbMsgPtr = (uchar *) configDescrMIDI;
		return sizeof(configDescrMIDI);
	}
}

static uchar sendEmptyFrame;


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"{
#endif 

#if !(defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__))

static void calibrateOscillator(void)
{
	uchar       step = 128;
	uchar       trialValue = 0, optimumValue;
	int         x, optimumDev, targetValue = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);

	/* do a binary search: */
    do {
        OSCCAL = trialValue + step;
        x = usbMeasureFrameLength();    /* proportional to current real frequency */
        if(x < targetValue)             /* frequency still too low */
            trialValue += step;
        step >>= 1;
    } while (step > 0);

    /* We have a precision of +/- 1 for optimum OSCCAL here */
    /* now do a neighborhood search for optimum value */
    optimumValue = trialValue;
    optimumDev = x; /* this is certainly far away from optimum */
    for(OSCCAL = trialValue - 1; OSCCAL <= trialValue + 1; OSCCAL++) {
        x = usbMeasureFrameLength() - targetValue;
        if(x < 0) x = -x;
        if (x < optimumDev) {
            optimumDev = x;
            optimumValue = OSCCAL;
        }
    }
    OSCCAL = optimumValue;
}

#endif

void hadUsbReset(void)
{
	cli();
#if !(defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__))
	calibrateOscillator();
#endif
	sei();
}

uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (usbRequest_t *)((void *)data);

	if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
		/* class request type */
		/*  Prepare bulk-in endpoint to respond to early termination   */
		if ((rq->bmRequestType & USBRQ_DIR_MASK) ==USBRQ_DIR_HOST_TO_DEVICE){
			sendEmptyFrame = 1;
		}
	}
	return 0xff;
}

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

void usbFunctionWriteOut(uchar *data, uchar len)
{
	byte cnt;
	byte kindmessage;
	byte interrupt;
	byte *pMidi;

//	cli();

	interrupt = len / 4;
	for(cnt = 0;cnt < interrupt;cnt++){
		pMidi = data + (cnt * 4);
		kindmessage = checkMidiMessage(pMidi);
		if(kindmessage == 1){
			if(cbNoteOff != NULL){
				(*cbNoteOff)(*(pMidi+1)&0x0f,*(pMidi+2)&0x7f,*(pMidi+3)&0x7f);
			}
		}else if(kindmessage == 2){
			if(cbNoteOn != NULL){
				(*cbNoteOn)(*(pMidi+1)&0x0f,*(pMidi+2)&0x7f,*(pMidi+3)&0x7f);
			}
		}else if(kindmessage == 3){
			if(cbCtlChange != NULL){
				(*cbCtlChange)(*(pMidi+1)&0x0f,*(pMidi+2)&0x7f,*(pMidi+3)&0x7f);
			}
		}
	}
//	sei();
}

#ifdef __cplusplus
} // extern "C"
#endif


class UsbMidiDevice {

public:
	UsbMidiDevice () {
	}

	void init(){
#if defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) 
		ACSR |= (1<<ACD); // Disable analog comparator
//		PCMSK0 = 0;

#elif defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
		ACSR0A |= (1<<ACD0); // Disable analog comparator
		ACSR1A |= (1<<ACD1); // Disable analog comparator
		SREG |= 0x80; // I bit (0x80) Set to global interrupt enable
//		PCMSK = 0;
#endif

		cli();
#if !(defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__))
		calibrateOscillator();
#endif
		usbInit();
		usbDeviceDisconnect();
		_delay_ms(250);
		usbDeviceConnect();
		cbNoteOff = NULL;
		cbNoteOn = NULL;
		cbCtlChange = NULL;
		sei();
	}
  
	void update() {
		usbPoll();
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

private:
	byte buffer[4];

	void sendMidiMessage(byte *data, byte size) {
		while (!usbInterruptIsReady()) {
			usbPoll();
		}
   	usbSetInterrupt(data, size);
 	}

};

UsbMidiDevice UsbMidi = UsbMidiDevice();

#endif // __VUSBMidiATtiny_h__
