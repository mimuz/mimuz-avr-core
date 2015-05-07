// variant.cpp
// Timer Pins definition for ATtiny841
// CC 2015 by D.F.mac.

#include <avr/pgmspace.h>
#if 0 // for test
#include "arduino.h"
#endif

#if defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
#include "pins_arduino.h"
void initVariant(void);

void initVariant()
{
#if 0 // for test
  DDRB |= 0x04;
  PORTB |= 0x04;
  delay(500);
  PORTB &= ~0x04;
  delay(500);
  PORTB |= 0x04;
  delay(500);
  PORTB &= ~0x04;
  delay(500);
#endif

T841_TIMER_PINS_DEFAULT;

}
#endif