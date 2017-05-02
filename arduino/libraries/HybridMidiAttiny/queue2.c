// Queue.c
// 4byte array ring buffer
//
// 2015.05.08 : add by D.F.Mac.

#include "queue2.h"

#ifdef __cplusplus
extern "C"{
#endif 

void b4arrq_init(stB4Arrq *qp){
	qp->top = 0;
	qp->last = 0;
	qp->num = 0;
	qp->queue_size = B4QUEUE_SIZE;
}

uint8_t b4arrq_next(uint8_t value){
	return (value + ELEM_SIZE) % B4QUEUE_SIZE;
}

uint8_t b4arrq_push(stB4Arrq *qp, uint8_t *p){
	uint8_t *q = qp->queue;
	uint8_t last = qp->last;
	if(b4arrq_next(last) == qp->top){
		return 0;
	}
	*(q+last) = *p;
	*(q+(last+1)) = *(p+1);
	*(q+(last+2)) = *(p+2);
	*(q+(last+3)) = *(p+3);
	qp->last = b4arrq_next(last);
	qp->num ++;
	return 1;
}

uint8_t *b4arrq_pop(stB4Arrq *qp){
	uint8_t *pRes = (uint8_t *)0;
	uint8_t *p = qp->queue;
	uint8_t top = qp->top;
	if(top != qp->last){
		pRes = p+top;
		qp->top = b4arrq_next(top);
		qp->num --;
	}
	return pRes;
}

#ifdef __cplusplus
} // extern "C"
#endif

