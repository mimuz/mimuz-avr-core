// MimuzHelper.h
//
// 2015.05.21 : add by D.F.Mac.

#ifndef __MimuzHelper_h__
#define __MimuzHelper_h__

#include "pins_arduino.h"

// ------------------------------------------
// LED Macro
// ------------------------------------------
#if defined (ARDUINO_MIMUZ_PROT2) || defined (ARDUINO_MIMUZ_EXPR2)
#define LED_MASK (1 << 2)
#define LED_DDR  DDRB
#define LED_PORT PORTB
#elif defined (ARDUINO_MIMUZ_PROT3)
#define LED_MASK (1 << 4)
#define LED_DDR  DDRB
#define LED_PORT PORTB
#elif defined (ARDUINO_MIMUZ_EXPR1)
#define LED_MASK (1 << 3)
#define LED_DDR  DDRB
#define LED_PORT PORTB
#else
#define LED_MASK (1 << 0)
#define LED_DDR  DDRB
#define LED_PORT PORTB
#endif

#define ledInit() (LED_DDR |= LED_MASK)
#define ledOn() (LED_PORT |= LED_MASK)
#define ledOff() (LED_PORT &= ~LED_MASK)

// analogRead() for ATtiny841
#ifdef __cplusplus
extern "C"{
#endif 

#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)

int analogRead841(uint8_t pin)
{
  uint8_t low, high;

  if(pin < 8){
    DIDR0 |= (1 << pin);
  }else{
    if(pin == 8){
      DIDR1 |= 0x04;
    }else if(pin == 9){
      DIDR1 |= 0x08;
    }else if(pin == 10){
      DIDR1 |= 0x02;
    }else{
      DIDR1 |= 0x01;
    }
  }

  ADMUXB = 0;    // vref = VCC
  ADMUXA = pin;  // Select ADC Pin in Single Ended Mode

  ADCSRA |= (1 << ADSC); // ADEN, ADPS2-1 were arleady set in Arduino core's wiring.c
  while ((ADCSRA & (1 << ADSC)) != 0){}
  
  low  = ADCL;
  high = ADCH;

  if(pin < 8){
    DIDR0 &= ~(1 << pin);
  }else{
    if(pin == 8){
      DIDR1 &= ~0x04;
    }else if(pin == 9){
      DIDR1 &= ~0x08;
    }else if(pin == 10){
      DIDR1 &= ~0x02;
    }else{
      DIDR1 &= ~0x01;
    }
  }    

  return (high << 8) | low;
}

#define analogRead(x) analogRead841(x)

void pinMode841(uint8_t pin, uint8_t mode)
{
  uint8_t bit = digitalPinToBitMask(pin);
  volatile uint8_t *reg, *out;

  if(pin < 8){
    out = (volatile uint8_t *)&PUEA;
    reg = (volatile uint8_t *)&DDRA;
  }else{
    out = (volatile uint8_t *)&PUEB;
    reg = (volatile uint8_t *)&DDRB;
  }

  if (mode == INPUT) { 
    uint8_t oldSREG = SREG;
    cli();
    *reg &= ~bit;
    *out &= ~bit;
    SREG = oldSREG;
  } else if (mode == INPUT_PULLUP) {
    uint8_t oldSREG = SREG;
    cli();
    *reg &= ~bit;
    *out |= bit;
    SREG = oldSREG;
  } else {
    uint8_t oldSREG = SREG;
    cli();
    *reg |= bit;
    SREG = oldSREG;
  }
}

#define pinMode(x,y) pinMode841(x,y)


#endif // defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)

#ifdef __cplusplus
} // extern "C"
#endif

#endif	// __MimuzHelper_h__
