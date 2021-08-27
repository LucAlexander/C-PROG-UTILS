#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <inttypes.h>

typedef struct hlNode{
	struct hlNode* next;
	char* key;
	void* val;
} hlNode;

hlNode* hlNodeInit(size_t size, const char* key, void* value);

void hlNodeFree(hlNode* n);

typedef struct map{
	hlNode** data;
	uint32_t capacity;
	size_t valSize;
}map;

typedef struct{
	map* m;
	hlNode* current;
	uint32_t index;
} mapIterator;

map* mapInit(size_t size);

uint32_t asciiWeightHash(const char* key, uint32_t capacity);

void mapListInsert(hlNode* n, size_t size, const char* key, void* value);

void mapInsert(map* m, const char* key, void* value);

void* mapGet(map* m, const char* key);

void mapRemove(map* m, const char* key);

void mapClear(map* m);

void mapClose(map* m);

mapIterator* mapIteratorInit(map* m);

void mapIteratorNext(mapIterator* i);

void mapIteratorClose(mapIterator* i);
#endif
