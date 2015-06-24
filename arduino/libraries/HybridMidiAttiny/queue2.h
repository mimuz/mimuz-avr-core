// Queue.h
// 4byte array ring buffer
//
// 2015.05.08 : add by D.F.Mac.
// 2015.05.18 : re-writed

#ifndef __queue_h__
#define __queue_h__

#include <util/delay.h>
#include <avr/wdt.h>

#ifdef __cplusplus
extern "C"{
#endif 

#define B4QUEUE_SIZE 16 // (BLOCK_NUM * ELEM_SIZE)
#define ELEM_SIZE 4

typedef struct {
	uint8_t top;
	uint8_t last;
	uint8_t num;
	uint8_t queue_size;	// not use yet.
	uint8_t queue[B4QUEUE_SIZE];
} stB4Arrq;

void b4arrq_init(stB4Arrq *qp);
uint8_t b4arrq_push(stB4Arrq *qp, uint8_t *p);
uint8_t *b4arrq_pop(stB4Arrq *qp);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
