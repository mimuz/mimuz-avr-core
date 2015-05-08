// Queue.c
// 4byte array ring buffer
//
// 2015.05.08 : add by D.F.Mac.

#include "queue.h"

#ifdef __cplusplus
extern "C"{
#endif 

const uint8_t BLOCK_NUM = 8;
const uint8_t ELEM_SIZE = 4;

#define B4QUEUE_SIZE 32 // (BLOCK_NUM * ELEM_SIZE)
uint8_t b4arrq_queue[B4QUEUE_SIZE];
uint8_t b4arrq_top = 0;
uint8_t b4arrq_last = 0;
extern uint8_t b4arrq_num = 0;

void b4arrq_init(void){
	b4arrq_top = 0;
	b4arrq_last = 0;
}

uint8_t b4arrq_next(uint8_t value){
	return (value + ELEM_SIZE) % B4QUEUE_SIZE;
}

uint8_t b4arrq_push(uint8_t *p){
	if(b4arrq_next(b4arrq_last) == b4arrq_top){
		return 0;
	}
	b4arrq_queue[b4arrq_last] = *p;
	b4arrq_queue[b4arrq_last+1] = *(p+1);
	b4arrq_queue[b4arrq_last+2] = *(p+2);
	b4arrq_queue[b4arrq_last+3] = *(p+3);
	b4arrq_last = b4arrq_next(b4arrq_last);
	b4arrq_num ++;
	return 1;
}

uint8_t *b4arrq_pop(void){
	uint8_t *pRes = (uint8_t *)0;
	if(b4arrq_top != b4arrq_last){
		pRes = &(b4arrq_queue[b4arrq_top]);
		b4arrq_top = b4arrq_next(b4arrq_top);
		b4arrq_num --;
	}
	return pRes;
}

#ifdef __cplusplus
} // extern "C"
#endif

