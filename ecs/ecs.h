#ifndef ECS_H
#define ECS_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include "stack.h"

enum{
	ALIVE = 1,
	EXISTS = 2
} ENT_FLAGS;

typedef struct{
	uint32_t count;
	uint32_t capacity;
	uint32_t* masks;
	uint32_t* flags;
} Entities;

typedef struct{
	void* data;
	size_t* sizes;
	size_t* offsets;
	size_t size;
	uint32_t capacity;
	uint32_t typeCount;
} Components;

typedef struct{
	uint32_t count;
	uint32_t* list;
} ComponentQuery;

typedef struct{
	Entities entities;
	Components components;
	ComponentQuery query;
	Stack* idBacklog;
} ECS;

void ecsInit(uint32_t n, ...);

uint32_t ecsGenerateEntityId();

void ecsResize();

uint32_t entCreate();

void* entGet(uint32_t eid, uint32_t cid);

void entAdd(uint32_t eid, uint32_t cid, void* data);

void entRemove(uint32_t eid, uint32_t cid);

uint32_t entContains(uint32_t eid, uint32_t cid);

void ecsFreeComponent(uint32_t eid, uint32_t cid);

void ecsClearEntityComponentData(uint32_t eid);

void entDestroy(uint32_t eid);

void ecsDestroyQueue();

ComponentQuery* ecsQuery(uint32_t n, ...);

ComponentQuery* ecsQueryAlive(uint8_t alive, uint32_t n, ...);

void ecsClose();
#endif
