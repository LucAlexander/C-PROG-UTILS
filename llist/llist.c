#include <stdio.h>
#include <string.h>
#include "llist.h"

llNode* llNodeInit(void* data, size_t size){
	llNode* n = malloc(sizeof(llNode));
	n->next = NULL;
	n->data = malloc(size);
	memcpy(n->data, data, size);
	return n;
}

LinkedList* llInit(size_t size){
	LinkedList* ll = malloc(sizeof(LinkedList));
	ll->head = NULL;
	ll->last = NULL;
	ll->length = 0;
	ll->size = size;
	return ll;
}

void llAdd(LinkedList* ll, void* data){
	ll->length++;
	if (ll->head == NULL){
		ll->head = llNodeInit(data, ll->size);
		ll->last = ll->head;
		return;
	}
	ll->last->next = llNodeInit(data, ll->size);
	ll->last = ll->last->next;
}

void llInsert(LinkedList* ll, void* data, uint32_t index){
	if (ll->head==NULL||index >= ll->length){
		llAdd(ll, data);
		return;
	}
	if (index == 0){
		llNode* newHead = llNodeInit(data, ll->size);
		newHead->next = ll->head;
		ll->head = newHead;
		ll->length++;
		return;
	}
	llNode* temp = ll->head;
	uint32_t i = 1;
	while (i < index){
		temp = temp->next;
		++i;
	}
	llNode* newNode = llNodeInit(data, ll->size);
	newNode->next = temp->next;
	temp->next = newNode;
	ll->length++;
}

void* llGet(LinkedList* ll, uint32_t index){
	if (ll->head == NULL){
		return NULL;
	}
	if (index >= ll->length-1){
		return ll->last->data;
	}
	llNode* temp = ll->head;
	while (index > 0){
		temp = temp->next;
		index--;
	}
	return temp->data;
}

void llErase(LinkedList* ll, uint32_t index){
	if (ll->head == NULL || index >= ll->length){
		return;
	}
	ll->length--;
	if (index == 0){
		llNode* temp = ll->head;
		ll->head = ll->head->next;
		if (temp->data != NULL){
			free(temp->data);
		}
		free(temp);
		return;
	}
	llNode* temp = ll->head;
	while(index > 1){
		temp = temp->next;
		index--;
	}
	llNode* remNode = temp->next;
	temp->next = remNode->next;
	if (remNode->data != NULL){
		free(remNode->data);
	}
	free(remNode);
	remNode = NULL;
	if (ll->last == NULL){
		ll->last = temp;
	}
}

void llFree(LinkedList* ll){
	llNode* temp = ll->head;
	while(temp != NULL){
		llNode* hold = temp->next;
		temp->next = NULL;
		if (temp->data != NULL){
			free(temp->data);
			temp->data = NULL;
		}
		free(temp);
		temp = hold;
		ll->length--;
	}
	ll->last = NULL;
	ll->head = NULL;
	free(ll);
	ll = NULL;
}
