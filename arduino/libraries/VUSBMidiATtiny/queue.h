// Queue.h
// 4byte array ring buffer
//
// 2015.05.08 : add by D.F.Mac.

#ifndef __queue_h__
#define __queue_h__

#include <util/delay.h>
#include <avr/wdt.h>

#ifdef __cplusplus
extern "C"{
#endif 

void b4arrq_init(void);
uint8_t b4arrq_push(uint8_t *p);
uint8_t *b4arrq_pop(void);
extern uint8_t b4arrq_num;

#ifdef __cplusplus
} // extern "C"
#endif

#endif
