#include "hashmap.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>

map* mapInit(size_t size){
	map* m = (map*)malloc(sizeof(map));
	m->capacity = 32;
	m->valSize = size;
	m->data = calloc(m->capacity, sizeof(hlNode*));
}

hlNode* hlNodeInit(size_t size, char* key, void* value){
	hlNode* newNode = (hlNode*)malloc(sizeof(hlNode));
	newNode->key = strdup(key);
	newNode->next = NULL;
	newNode->val = NULL;
	if (value != NULL){
		newNode->val = malloc(size);
		memcpy(newNode->val, value, size);
	}
	return newNode;
}

uint32_t asciiWeightHash(char* key, uint32_t capacity){
	uint32_t result = 0;
	uint32_t len = sizeof(key);
	for (int i = 0;i<len;++i){
		result += key[i] * pow(128, len-1-i);
	}
	return result%capacity;
}

void mapListInsert(hlNode* n, size_t size, char* key, void* value){
	hlNode* last = n;
	while(n != NULL){
		if (strcmp(n->key, key) == 0){
			memcpy(n->val, value, size);
			return;
		}
		last = n;
		n = n->next;
	}
	hlNode* newNode = hlNodeInit(size, key, value);
	last->next = newNode;
}

void mapInsert(map* m, char* key, void* value){
	uint32_t index = asciiWeightHash(key, m->capacity);
	if (m->data[index] != NULL){
		mapListInsert(m->data[index], m->valSize, key, value);
		return;
	}
	hlNode* newNode = hlNodeInit(m->valSize, key, value);
	m->data[index] = newNode;
}

void* mapGet(map* m, char* key){
	uint32_t hash = asciiWeightHash(key, m->capacity);
	hlNode* n = m->data[hash];
	hlNode* last = n;
	while(n != NULL){
		if (strcmp(key, n->key)==0){
			return n->val;
		}
		last = n;
		n=n->next;
	}
	if (last != NULL){
		hlNode* newNode = hlNodeInit(m->valSize, key, NULL);
		last->next = newNode;
		return newNode;
	}
	hlNode* newNode = hlNodeInit(m->valSize, key, NULL);
	m->data[hash] = newNode;
	return newNode;
}

void mapRemove(map* m, char* key){
	uint32_t index = asciiWeightHash(key, m->capacity);
	hlNode* temp = m->data[index];
	if (temp == NULL){
		return;
	}
	if (temp->next == NULL){
		hlNodeFree(temp);
		temp = NULL;
		m->data[index] = NULL;
		return;
	}
	if(strcmp(key, temp->key)==0){
		m->data[index] = temp->next;
		temp->next = NULL;
		hlNodeFree(temp);
		temp = NULL;
		return;
	}
	hlNode* prev = temp;
	temp = temp->next;
	while(temp!=NULL && strcmp(key, temp->key)!=0){
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL){
		return;
	}
	prev->next = temp->next;
	temp->next = NULL;
	hlNodeFree(temp);
}

void hlNodeFree(hlNode* n){
	while(n!=NULL){
		if (n->val != NULL){
			free(n->key);
			free(n->val);
			n->key = NULL;
			n->val = NULL;
		}
		hlNode* temp = n;
		n = n->next;
		free(temp);
		temp = NULL;
	}
	n = NULL;
}

void mapClose(map* m){
	for (int i = 0;i<m->capacity;++i){
		if (m->data[i] != NULL){
			hlNodeFree(m->data[i]);
		}
	}
	free(m->data);
	m->data = NULL;
	free(m);
	m = NULL;
}

mapIterator* mapIteratorInit(map* m){
	mapIterator* n = (mapIterator*)malloc(sizeof(mapIterator));
	n->m = m;
	uint32_t i;
	for (i = 0;i<m->capacity && m->data[i]==NULL;++i){}
	if (i==m->capacity){
		n->index = -1;
		n->current = NULL;
		return n;
	}
	n->current = m->data[i];
	n->index = i;
	return n;
}

void mapIteratorNext(mapIterator* n){
	if (n->current->next != NULL){
		n->current = n->current->next;
		return;
	}
	uint32_t i;
	for (i = ++n->index;i<n->m->capacity && n->m->data[i]==NULL;++i){}
	if (i==n->m->capacity){
		n->index = -1;
		n->current = NULL;
		return;
	}
	n->current = n->m->data[i];
	n->index = i;
	return;
}

void mapIteratorClose(mapIterator* i){
	free(i);
	i = NULL;
}
