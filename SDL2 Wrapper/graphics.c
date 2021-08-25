#include "graphics.h"
#include "gmath.h"
#include "fileLoader.h"
#include "hashmap.h"
#include <string.h>

#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static view renderView = {0, 0, 0, 0};
static fontHandler fonts = {NULL, NULL};

void graphicsInit(uint32_t width, uint32_t height, const char* windowTitle){
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	if (window != NULL){
		SDL_DestroyWindow(window);
	}
	if (renderer != NULL){
		SDL_DestroyRenderer(renderer);
	}
	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
	SDL_SetWindowTitle(window, windowTitle);
	view defaultView = {0, 0, width, height};
	renderSetView(defaultView);
	fileLoaderInit();
	fontHandlerInit();
}

void graphicsClose(){
	fontHandlerClose();
	fileLoaderClose();
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

SDL_Texture* getTexture(const char* src){
	SDL_Texture* item = SDL_CreateTextureFromSurface(renderer, loadImage(src));
	if (item == NULL){
		printf("[!] Unable to load image \'%s\'\n",src);
	}
	return item;
}

void renderSetView(view v){
	renderView = v;
	const SDL_Rect port = {v.px, v.py, v.pw, v.ph};
	SDL_RenderSetViewport(renderer, &port);
}

void renderFlip(){
	SDL_RenderPresent(renderer);
}

void renderClear(){
	SDL_RenderClear(renderer);
}

void renderSetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void blitSurface(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect){
	destRect.x-=renderView.x;
	destRect.y-=renderView.y;
	SDL_RenderCopy(renderer, texture, srcRect, &destRect);
}

void blitSurfaceEX(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect, double angle, SDL_Point* center, SDL_RendererFlip flip){
	destRect.x-=renderView.x;
	destRect.y-=renderView.y;
	SDL_RenderCopyEx(renderer, texture, srcRect, &destRect, angle, center, flip);
}

void blitSurfaceF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect){
	destRect.x-=renderView.x;
	destRect.y-=renderView.y;
	SDL_RenderCopyF(renderer, texture, srcRect, &destRect);
}

void blitSurfaceEXF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect, double angle, SDL_FPoint* center, SDL_RendererFlip flip){
	destRect.x-=renderView.x;
	destRect.y-=renderView.y;
	SDL_RenderCopyExF(renderer, texture, srcRect, &destRect, angle, center, flip);
}

void drawLineV2(v2 a, v2 b){
	drawLine(a.x, a.y, b.x, b.y);
}

void drawLine(float x, float y, float xx, float yy){
	SDL_RenderDrawLine(renderer, x-renderView.x, y-renderView.y, xx-renderView.x, yy-renderView.y);
}

void drawRectV2(v2 a, v2 b, uint8_t p){
	drawRect(a.x, a.y, b.x, b.y, p);
}

void drawRectV4(v4 r, uint8_t p){
	drawRect(r.left, r.top, r.right, r.bottom, p);
}

void drawRect(float x1, float y1, float x2, float y2, uint8_t p){
	SDL_FRect bound = {
		x1-renderView.x,
		y1-renderView.y,
	       	x2+renderView.x-x1-renderView.x,
	       	y2+renderView.y-y1-renderView.y
	};
	if (p & FILL){
		SDL_RenderFillRectF(renderer, &bound);
	}
	if (p & OUTLINE){
		SDL_RenderDrawRectF(renderer, &bound);
	}
}

void fontHandlerInit(){
	fonts.charList = "`1234567890-=~!@#$%^&*()_+qwertyuiop[]QWERTYUIOP{}|asdfghjkl;'ASDFGHJKL:zxcvbnm,./ZXCVBNM<>? ";
	fonts.activeFont = "";
	fonts.list = mapInit(sizeof(font));
}

void loadFont(const char* src){
	font* fnt = (font*)mapGet(fonts.list, src);
	if (fnt!=NULL){
		return;
	}
	font f;
	memset(f.glyphMap, 0, sizeof(SDL_Texture*)*128);
	f.r = 255;
	f.g = 255;
	f.b = 255;
	f.a = 255;
	f.kerning = 1;
	f.leading = 1;
	f.ptSize = 16;
	f.scale = 1;
	TTF_Font* lFont = TTF_OpenFont(src, f.ptSize);
	uint32_t i;
	char c[2];
	c[1] = '\0';
	for (i = 0;i<strlen(fonts.charList);++i){
		c[0] = fonts.charList[i];
		SDL_Color fg = {f.r, f.g, f.b};
		SDL_Surface* s = TTF_RenderText_Solid(lFont, c, fg);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		if (t!=NULL){
			f.glyphMap[(uint8_t)c[0]] = t;
		}
		SDL_FreeSurface(s);
	}
	TTF_CloseFont(lFont);
	mapInsert(fonts.list, src, &f);
}

void setFont(char* fnt){
	fonts.activeFont = fnt;
}

void fontClose(font* f){
	uint32_t i;
	for (i = 0;i<128;++i){
		SDL_Texture* t = f->glyphMap[i];
		if (t != NULL){
			SDL_DestroyTexture(t);
		}
	}
}

void fontHandlerClose(){
	mapIterator* mit = mapIteratorInit(fonts.list);
	while(mit->index != -1){
		fontClose((font*)mit->current->val);
		mapIteratorNext(mit);
	}
	mapIteratorClose(mit);
	mapClose(fonts.list);
}

void drawTextV2(v2 pos, char* text){
	drawText(pos.x, pos.y, text);
}

void drawText(float x, float y, char* text){
	font* f = (font*)mapGet(fonts.list, fonts.activeFont);
	if (f == NULL||text==NULL){
		return;
	}
	float cSize = f->scale*f->ptSize;
	SDL_Rect dest = {x, y, cSize, cSize};
	uint32_t i;
	for (i = 0;i<strlen(text);++i){
		drawCharacter(text[i], &dest, x, cSize, f);
	}
}

void drawCharacter(char c, SDL_Rect* dest, float startX, float cSize, font* f){
	switch(c){
		case '\n':
			dest->y += cSize + f->leading;
			dest->x = startX;
		return;
		case '\t':
			dest->x += (cSize+f->kerning)*4;
		return;
		case ' ':
			dest->x += cSize+f->kerning;
		return;
	}
	SDL_Texture* t = f->glyphMap[(uint8_t)c];
	if(t==NULL){
		return;
	}
	blitSurface(t, NULL, *dest);
	dest->x += cSize + f->kerning;
}
