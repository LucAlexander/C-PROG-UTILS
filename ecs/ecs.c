#include "ecs.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

const uint32_t compMax = 32;
const uint32_t entInitial = 32;

static ECS ecs = {0};

void ecsInit(uint32_t n, ...){
	uint32_t i;
	size_t s[compMax];
	size_t o[compMax];
	size_t size = 0;
	va_list v;
	va_start(v, n);
	for(i = 0;i<n;++i){
		s[i] = va_arg(v, size_t);
		o[i] = size;
		size += s[i];
	}
	va_end(v);
	ecs.idBacklog = stackInit(sizeof(uint32_t));
	ecs.components.typeCount = n;
	ecs.components.capacity = compMax;
	ecs.components.data = malloc(size*compMax);
	ecs.components.sizes = malloc(n*sizeof(size_t));
	ecs.components.offsets = malloc(n*sizeof(size_t));
	ecs.components.size = size;
	memcpy(ecs.components.sizes, s, n*sizeof(size_t));
	memcpy(ecs.components.offsets, o, n*sizeof(size_t));
	ecs.entities.count = 0;
	ecs.entities.capacity = entInitial;
	ecs.entities.masks = malloc(entInitial*sizeof(uint32_t));
	ecs.entities.flags = malloc(entInitial*sizeof(uint32_t));
	ecs.query.list = malloc(entInitial*sizeof(uint32_t));
}

uint32_t ecsGenerateEntityId(){
	if (ecs.idBacklog->count > 0){
		return *(uint32_t*)stackPop(ecs.idBacklog);
	}
	return ecs.entities.count++;
}

void ecsResize(){
	uint32_t* flagResize = realloc(ecs.entities.flags, ecs.entities.capacity*sizeof(uint32_t)*2);
	uint32_t* maskResize = realloc(ecs.entities.masks, ecs.entities.capacity*sizeof(uint32_t)*2);
	uint32_t* queryResize = realloc(ecs.query.list, ecs.entities.capacity*sizeof(uint32_t)*2);
	void* dataResize = realloc(ecs.components.data, ecs.components.capacity*ecs.components.size*2);
	ecs.entities.flags = flagResize;
	ecs.entities.masks = maskResize;
	ecs.query.list = queryResize;
	ecs.components.data = dataResize;
	ecs.entities.capacity*=2;
	ecs.components.capacity*=2;
}

uint32_t entCreate(){
	uint32_t entityId = ecsGenerateEntityId();
	if (entityId == ecs.entities.capacity){
		ecsResize();
	}
	ecs.entities.masks[entityId] = 0;
	ecs.entities.flags[entityId] = ALIVE;
	return entityId;
}

void* entGet(uint32_t eid, uint32_t cid){
	return (uint8_t*)ecs.components.data+(eid*ecs.components.size+ecs.components.offsets[cid]);
}

void entAdd(uint32_t eid, uint32_t cid, void* data){
	size_t size = ecs.components.sizes[cid];
	void* memLocation = entGet(eid, cid);
	ecs.entities.masks[eid] |= (1<<cid);
	memcpy(memLocation, data, size);
}

void entRemove(uint32_t eid, uint32_t cid){
	ecs.entities.masks[eid] &= ~(1<<cid);
}

uint32_t entContians(uint32_t eid, uint32_t cid){
	return ecs.entities.masks[eid] & (1<<cid);
}

void entDestroy(uint32_t eid){
	if((ecs.entities.flags[eid] & ALIVE) != 0){
		ecs.entities.flags[eid] &= ~ALIVE;
		ecs.entities.masks[eid] = 0;
		stackPush(ecs.idBacklog, &eid);
	}
}

ComponentQuery* ecsQuery(uint32_t n, ...){
	ecs.query.count = 0;
	uint32_t mask = 0;
	uint32_t i;
	va_list v;
	va_start(v, n);
	for (i = 0;i<n;++i){
		mask |= (1 << va_arg(v, uint32_t));
	}
	va_end(v);
	for (i = 0;i<ecs.entities.count;++i){
		if (((mask & ecs.entities.masks[i]) == mask) && ((ecs.entities.flags[i] & ALIVE) != 0)){
			ecs.query.list[ecs.query.count++] = i;
		}
	}
	return &ecs.query;
}

