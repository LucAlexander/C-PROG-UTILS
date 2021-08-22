#include "fileLoader.h"
#include "hashmap.h"

#include <SDL2\SDL_image.h>

static fileLoader loader = {0};

void fileLoaderInit(){
	loader.textures = mapInit(sizeof(SDL_Surface));
}

SDL_Surface* loadImage(const char* src){
	SDL_Surface* item = (SDL_Surface*)mapGet(loader.textures, src);
	if (item != NULL){
		return item;
	}
	item = IMG_Load(src);
	mapInsert(loader.textures, src, item);
	return item;
}

void fileLoaderClose(){
	mapIterator* mit = mapIteratorInit(loader.textures);
	while(mit->index != -1){
		SDL_FreeSurface((SDL_Surface*)mit->current->val);
		mapIteratorNext(mit);
	}
	mapIteratorClose(mit);
	mapClose(loader.textures);
}
