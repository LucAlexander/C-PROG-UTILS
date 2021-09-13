#define _GNU_SOURCE
#include "graphics.h"
#include "gmath.h"
#include "fileLoader.h"
#include "hashmap.h"
#include "strutils.h"
#include <string.h>

#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static view renderView = {0, 0, 0, 0, 0, 0};
static fontHandler fonts = {NULL, NULL, NULL, NULL};

static uint32_t windowW = 1920;
static uint32_t windowH = 1080;

static float spriteScaleX = 1;
static float spriteScaleY = 1;

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
	windowW = width;
	windowH = height;
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

void renderSetSpriteScale(float scaleX, float scaleY){
	spriteScaleX = scaleX;
	spriteScaleY = scaleY;
	SDL_RenderSetScale(renderer, spriteScaleX, spriteScaleY);
}

float scaleOnX(float val){
	return val/spriteScaleX;
}

float scaleOnY(float val){
	return val/spriteScaleY;
}

void renderSetView(view v){
	renderView = v;
	const SDL_Rect port = {v.px, v.py, v.pw, v.ph};
	SDL_RenderSetViewport(renderer, &port);
}

void toggleFullscreen(){
	uint32_t flags = (SDL_GetWindowFlags(window) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (SDL_SetWindowFullscreen(window, flags) < 0){
		printf("[!] Toggling fullscreen mode failed\n");
		return;
	}
	int32_t x = 0;
	int32_t y = 0;
	SDL_GetWindowSize(window, &x, &y);
	if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0){
		/*
		LINEAR WILL INTERPOLATE COLORS BETWEEN PIXELS
		0 IS STANDARD FOR PIXEL ART
		*/
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		//SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, 0);
		//SDL_RenderSetLogicalSize(renderer, x, y);
	}
	SDL_SetWindowSize(window, windowW, windowH);
}

v2 viewToWorldV2(v2 coords){
	coords.x += renderView.x;
	coords.y += renderView.y;
	return coords;
}

v2 worldToViewV2(v2 coords){
	coords.x -= renderView.x;
	coords.y -= renderView.y;
	return coords;
}

v2 viewToWorld(float x, float y){
	v2 a = {x,y};
	return viewToWorldV2(a);
}

v2 worldToView(float x, float y){
	v2 a = {x,y};
	return worldToViewV2(a);
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

void formatDestRectToView(SDL_Rect* destRect){
	destRect->x-=renderView.x;
	destRect->y-=renderView.y;
}

void formatDestFRectToView(SDL_FRect* destRect){
	destRect->x-=renderView.x;
	destRect->y-=renderView.y;
}

void blitSurface(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect){
	formatDestRectToView(&destRect);
	SDL_RenderCopy(renderer, texture, srcRect, &destRect);
}

void blitSurfaceEX(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect, double angle, SDL_Point* center, SDL_RendererFlip flip){
	formatDestRectToView(&destRect);
	SDL_RenderCopyEx(renderer, texture, srcRect, &destRect, angle, center, flip);
}

void blitSurfaceF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect){
	formatDestFRectToView(&destRect);
	SDL_RenderCopyF(renderer, texture, srcRect, &destRect);
}

void blitSurfaceEXF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect, double angle, SDL_FPoint* center, SDL_RendererFlip flip){
	formatDestFRectToView(&destRect);
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
	drawRect(r.x, r.y, r.w, r.h, p);
}

void drawRect(float x1, float y1, float x2, float y2, uint8_t p){
	SDL_FRect bound = {
		x1-renderView.x,
		y1-renderView.y,
	       	x2,
	       	y2
	};
	if (p & FILL){
		SDL_RenderFillRectF(renderer, &bound);
	}
	if (p & OUTLINE){
		SDL_RenderDrawRectF(renderer, &bound);
	}
}

void drawRectV2B(v2 a, v2 b, uint8_t p){
	drawRectB(a.x, a.y, b.x, b.y, p);
}

void drawRectV4B(v4B r, uint8_t p){
	drawRectB(r.left, r.top, r.right, r.bottom, p);
}

void drawRectB(float x1, float y1, float x2, float y2, uint8_t p){
	SDL_FRect bound = {
		x1-renderView.x,
		y1-renderView.y,
	       	x2-x1-renderView.x,
	       	y2-y1-renderView.y
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
	fonts.fnt=NULL;
	fonts.list = mapInit(sizeof(font));
}

void loadFont(const char* src){
	loadFontC(src, 255, 255, 255, 255);
}

void loadFontC(const char* src, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	font* fnt = (font*)mapGet(fonts.list, src);
	if (fnt!=NULL){
		return;
	}
	font f;
	memset(f.glyphMap, 0, sizeof(SDL_Texture*)*128);
	f.r = r;
	f.g = g;
	f.b = b;
	f.a = a;
	f.kerning = 1;
	f.leading = 1;
	f.ptSize = 16;
	f.scale = 1;
	TTF_Font* lFont = TTF_OpenFont(src, f.ptSize);
	SDL_Color fg = {f.r, f.g, f.b};
	uint32_t i;
	char c[2];
	c[1] = '\0';
	for (i = 0;i<strlen(fonts.charList);++i){
		c[0] = fonts.charList[i];
		SDL_Surface* s = TTF_RenderText_Solid(lFont, c, fg);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		if (t!=NULL){
			SDL_SetTextureAlphaMod(t, f.a);
			f.glyphMap[(uint8_t)c[0]] = t;
		}
		SDL_FreeSurface(s);
	}
	TTF_CloseFont(lFont);
	mapInsert(fonts.list, src, &f);
}

void setFont(char* fnt){
	font* temp = mapGet(fonts.list, fnt);
	if (temp == NULL){
		printf("[!] No font %s is loaded, and cannot be set to active font\n",fnt);
		return;
	}
	fonts.activeFont = fnt;
	fonts.fnt = temp;
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

void resetFontGlyphBlend(font* f){
	uint8_t i;
	for (i = 0;i<128;++i){
		SDL_Texture* t = f->glyphMap[i];
	        if (t != NULL){
			SDL_SetTextureAlphaMod(t,f->a);
			SDL_SetTextureColorMod(t,f->r, f->g, f->b);
		}
	}
}

void drawTextV2(v2 pos, char* text){
	drawText(pos.x, pos.y, text);
}

void drawTextV2C(v2 pos, char* text, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	drawTextC(pos.x, pos.y, text, r, g, b, a);
}

void drawText(float x, float y, char* text){
	if (text==NULL){
		return;
	}
	float cSize = fonts.fnt->scale*fonts.fnt->ptSize;
	SDL_Rect dest = {x, y, cSize, cSize};
	uint32_t i;
	for (i = 0;i<strlen(text);++i){
		drawCharacter(text[i], &dest, x, cSize, fonts.fnt);
	}
}

void drawTextC(float x, float y, char* text, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	if (text==NULL){
		return;
	}
	float cSize = fonts.fnt->scale*fonts.fnt->ptSize;
	SDL_Rect dest = {x, y, cSize, cSize};
	uint32_t i;
	for (i = 0;i<strlen(text);++i){
		drawCharacterC(text[i], &dest, x, cSize, fonts.fnt, r, g, b, a);
	}
	resetFontGlyphBlend(fonts.fnt);
}

SDL_Texture* handleWhitespace(char c, SDL_Rect* dest, float startX, float cSize, font* f){
	switch(c){
		case '\n':
			dest->y += cSize + f->leading;
			dest->x = startX;
		return NULL;
		case '\t':
			dest->x += (cSize+f->kerning)*4;
		return NULL;
		case ' ':
			dest->x += cSize+f->kerning;
		return NULL;
	}
	return f->glyphMap[(uint8_t)c];
}

void drawCharacter(char c, SDL_Rect* dest, float startX, float cSize, font* f){
	SDL_Texture* t = handleWhitespace(c, dest, startX, cSize, f);
	if(t==NULL){
		return;
	}
	blitSurface(t, NULL, *dest);
	dest->x += cSize + f->kerning;
}

void drawCharacterC(char c, SDL_Rect* dest, float startX, float cSize, font* f, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	SDL_Texture* t = handleWhitespace(c, dest, startX, cSize, f);
	if(t==NULL){
		return;
	}
	SDL_SetTextureColorMod(t, r, g, b);
	SDL_SetTextureAlphaMod(t, a);
	blitSurface(t, NULL, *dest);
	dest->x += cSize + f->kerning;
}

void drawTextEX(float x, float y, int32_t n, ...){
	float cSize = fonts.fnt->scale*fonts.fnt->ptSize;
	SDL_Rect dest = {x, y, cSize, cSize};
	uint32_t i;
	va_list args;
	va_start(args, n);
	const char* text;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
	while(n > 0){
		red = fonts.fnt->r;
		green = fonts.fnt->g;
		blue = fonts.fnt->b;
		alpha = fonts.fnt->a;
		text = va_arg(args, const char*);
		n--;
		if (n > 0){
			red = va_arg(args,int);
			green = va_arg(args,int);
			blue = va_arg(args,int);
			alpha = va_arg(args,int);
			n-=4;
		}
		for (i = 0;i<strlen(text);++i){
			drawCharacterC(text[i], &dest, x, cSize, fonts.fnt, red, green, blue, alpha);
		}
	}
	resetFontGlyphBlend(fonts.fnt);
	va_end(args);
}

void textWidthCharEval(char c, uint32_t* longest, uint32_t* current){
	if (c=='\n'){
		*longest = *current > *longest ? *current : *longest;
		*current = 0;
		return;
	}
	(*current)++;
}

float getTextWidth(const char* c){
	uint32_t len = strlen(c);
	uint32_t i;
	uint32_t longest = 0;
	uint32_t current = 0;
	for(i=0;i<len;++i){
		textWidthCharEval(c[i], &longest, &current);
	}
	longest = current > longest ? current : longest;
	return fonts.fnt->scale*fonts.fnt->ptSize*longest;
}

float getTextHeight(const char* c){
	uint32_t len = strlen(c);
	uint32_t i;
	uint32_t current = 1;
	for(i =0;i<len;++i){
		if (c[i]=='\n'){
			printf("here\n");
			current++;
		}
	}
	return fonts.fnt->scale*fonts.fnt->ptSize*current;
}

