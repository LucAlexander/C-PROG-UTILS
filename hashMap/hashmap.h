#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <inttypes.h>

typedef struct hlNode{
	struct hlNode* next;
	char* key;
	void* val;
} hlNode;

hlNode* hlNodeInit(size_t size, char* key, void* value);

void hlNodeFree(hlNode* n);

typedef struct{
	hlNode** data;
	uint32_t capacity;
	size_t valSize;
}map;

map* mapInit(size_t size);

uint32_t asciiWeightHash(char* key, uint32_t capacity);

void mapListInsert(hlNode* n, size_t size, char* key, void* value);

void mapInsert(map* m, char* key, void* value);

void* mapGet(map* m, char* key);

void mapRemove(map* m, char* key);

void mapClose(map* m);

#endif
