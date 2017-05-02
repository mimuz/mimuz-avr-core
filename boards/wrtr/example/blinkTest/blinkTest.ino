// Blink Test For mi:muz ISP Shield

// LED 
#if defined (__AVR_ATtiny24__) || defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__) || defined (__AVR_ATtiny441__) || defined (__AVR_ATtiny841__)
#define LED_INIT() (DDRB |= _BV(2))
#define LED_HIGH() (PORTB |= _BV(2))
#define LED_LOW() (PORTB &= ~_BV(2))
#elif defined (__AVR_ATtiny13__) || defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__) 
#define LED_INIT() (DDRB |= _BV(3))
#define LED_HIGH() (PORTB |= _BV(3))
#define LED_LOW() (PORTB &= ~_BV(3))
#endif

void setup() {
  // put your setup code here, to run once:
  LED_INIT();
  LED_HIGH();
  delayMs(100);
  LED_LOW();
}

void loop() {
  // put your main code here, to run repeatedly:
  delayMs(1000);
  LED_HIGH();
  delayMs(100);
  LED_LOW();
}

void delayMs(unsigned int ms) {
  for( int i=0; i<ms; i++ ) {
    delayMicroseconds(1000);
  }
}

