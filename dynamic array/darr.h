#ifndef DARR_H
#define DARR_H

#include <stddef.h>
#include <inttypes.h>

typedef struct darr{
	void* data;
	uint32_t capacity;
	uint32_t length;
	size_t size;
}darr;

typedef struct darrIterator{
	darr* arr;
	uint32_t index;
	uint8_t* current;
}darrIterator;

darr* darrInit(size_t s);

void darrResize(darr* d);

void darrReserve(darr* d, uint32_t count);

void darrAdd(darr* d, void* e);

void darrSet(darr* d, void* e, uint32_t index);

void darrInsert(darr* d, void* e, uint32_t index);

void darrErase(darr* d, uint32_t index);

void darrClear(darr* d);

void* darrGet(darr* d, uint32_t index);

void darrFree(darr* d);

darrIterator* darrIteratorInit(darr* d);

void darrIteratorNext(darrIterator* dit);

void darrIteratorClose(darrIterator* dit);

#endif
