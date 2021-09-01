#include <stdio.h>
#include <string.h>
#include "stack.h"

Stack* stackInit(size_t size){
	Stack* stack = malloc(sizeof(Stack*));
	stack->data = malloc(size);
	stack->size = size;
	stack->capacity = 1;
	stack->count = 0;
	return stack;
}

void* stackPop(Stack* stack){
	uint8_t* item = (uint8_t*)stack->data;
	if (stack->count > 0){
		return item + stack->size*(--stack->count);
	}
	return NULL;
}

void stackPush(Stack* stack, void* data){
	if (stack->count == stack->capacity){
		stack->data = realloc(stack->data, stack->size*stack->capacity*2);
		stack->capacity*=2;
	}
	uint8_t* item = (uint8_t*)(stack->data);
	item += stack->size*(stack->count++);
	memcpy(item, data, stack->size);
}

void stackFree(Stack* stack){
	free(stack->data);
	stack->data = NULL;
	free(stack);
	stack = NULL;
}
