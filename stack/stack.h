#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <inttypes.h>

typedef struct{
	void* data;
	uint32_t count;
	uint32_t capacity;
	size_t size;
}Stack;

Stack* stackInit(size_t size);

void* stackPop(Stack* s);

void stackPush(Stack* s, void* data);

void stackFree(Stack* s);

#endif
