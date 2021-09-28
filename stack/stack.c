#include <stdio.h>
#include <string.h>
#include "stack.h"

Stack* stackInit(size_t size){
	Stack* stack = malloc(sizeof(Stack*));
	stack->capacity = 32;
	stack->data = calloc(stack->capacity, size);
	stack->size = size;
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

void stackResize(Stack* stack){
	void* resizedBlock = realloc(stack->data, stack->size*stack->capacity*2);
	uint8_t* newBlock = (uint8_t*)(resizedBlock);
	newBlock += stack->size*(stack->count);
	memset(newBlock, 0, stack->size*stack->capacity);
	stack->data = resizedBlock;
	stack->capacity*=2;
}

void stackPush(Stack* stack, void* data){
	if (stack->count == stack->capacity){
		stackResize(stack);
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
