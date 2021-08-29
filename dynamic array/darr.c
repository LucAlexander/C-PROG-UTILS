#include "darr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

darr* darrInit(size_t s){
	darr* d = malloc(sizeof(darr));
	d->capacity = 32;
	d->length = 0;
	d->data = malloc(d->capacity*s);
	d->size = s;
	return d;
}

void darrResize(darr* d){
	d->data = malloc(d->capacity*2*d->size);
	d->capacity*=2;
}

void darrReserve(darr* d, uint32_t count){
	uint32_t accounted = (d->capacity-d->length);
	if (accounted >= count){
		return;
	}
	d->data = malloc(d->size*(d->capacity+(count-accounted)));
	d->capacity+=(count-accounted);
}

void darrAdd(darr* d, void* e){
	if (d->length == d->capacity){
		darrResize(d);
	}
	uint8_t* ptr = (uint8_t*)d->data;
	ptr += d->length*d->size;
	memcpy(ptr, e, d->size);
	d->length++;
}

void darrSet(darr* d, void* e, uint32_t index){
	if (index > d->length){
		return;
	}
	if (index == d->length){
		darrAdd(d, e);
	}
	uint8_t* ptr = (uint8_t*)d->data;
	ptr += d->size*index;
	memcpy(ptr, e, d->size);
}

void darrInsert(darr* d, void* e, uint32_t index){
	if (index > d->length){
		return;
	}
	if (index == d->length){
		darrAdd(d, e);
		return;
	}
	if (d->length==d->capacity){
		darrResize(d);
	}
	uint8_t* ptr = (uint8_t*)d->data;
	ptr += d->length*d->size;
	uint8_t* target = ptr-d->size;
	uint32_t i = index;
	while(i < d->length){
		memcpy(ptr, target, d->size);
		ptr = target;
		target -= d->size;
		i++;
	}
	memcpy(ptr, e, d->size);
	d->length++;
}

void darrErase(darr* d, uint32_t index){
	if(index > d->length){
		return;
	}
	uint8_t* ptr = (uint8_t*)d->data;
	ptr += index*d->size;
	uint8_t* target = ptr+d->size;
	while(index < d->length){
		memcpy(ptr, target, d->size);
		ptr = target;
		target += d->size;
		index++;
	}
	d->length--;
}

void darrClear(darr* d){
	d->length = 0;
}

void* darrGet(darr* d, uint32_t index){
	uint8_t* ptr = (uint8_t*)d->data;
	ptr += index*d->size;
	return ptr; 
}

void darrFree(darr* d){
	free(d->data);
	d->data = NULL;
	free(d);
	d = NULL;
}

darrIterator* darrIteratorInit(darr* d){
	darrIterator* dit = malloc(sizeof(darrIterator));
	dit->arr = d;
	dit->index = d->length > 0 ? 0 : -1;
	dit->current = (uint8_t*)d->data;
	return dit;
}

void darrIteratorNext(darrIterator* dit){
	dit->index ++;
	if (dit->index == 0 || dit->index >= dit->arr->length){
		dit->index = -1;
		return;
	}
	dit->current += dit->arr->size;
}

void darrIteratorClose(darrIterator* dit){
	free(dit);
	dit = NULL;
}

