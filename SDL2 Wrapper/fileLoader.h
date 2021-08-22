#ifndef FILELOADER_H
#define FILELOADER_H

#include <SDL2\SDL.h>

struct map;

typedef struct fileLoader{
	struct map* textures;
}fileLoader;

void fileLoaderInit();

SDL_Surface* loadImage(const char* src);

void fileLoaderClose();

#endif
