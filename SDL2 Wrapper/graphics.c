#include "graphics.h"
#include "gmath.h"

#include <SDL2\SDL_image.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

void graphicsInit(uint32_t width, uint32_t height, const char* windowTitle){
	SDL_Init(SDL_INIT_VIDEO);
	if (window != NULL){
		SDL_DestroyWindow(window);
	}
	if (renderer != NULL){
		SDL_DestroyRenderer(renderer);
	}
	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
	SDL_SetWindowTitle(window, windowTitle);
}

void graphicsClose(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
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

void blitSurface(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect){
	SDL_RenderCopy(renderer, texture, srcRect, destRect);
}

void blitSurfaceEX(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect, double angle, SDL_Point* center, SDL_RendererFlip flip){
	SDL_RenderCopyEx(renderer, texture, srcRect, destRect, angle, center, flip);
}

void blitSurfaceF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect* destRect){
	SDL_RenderCopyF(renderer, texture, srcRect, destRect);
}

void blitSurfaceEXF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect* destRect, double angle, SDL_FPoint* center, SDL_RendererFlip flip){
	SDL_RenderCopyExF(renderer, texture, srcRect, destRect, angle, center, flip);
}

void drawLineV2(v2 a, v2 b){
	drawLine(a.x, a.y, b.x, b.y);
}

void drawLine(float x, float y, float xx, float yy){
	SDL_RenderDrawLine(renderer, x, y, xx, yy);
}

void drawRectV2(v2 a, v2 b, uint8_t p){
	drawRect(a.x, a.y, b.x, b.y, p);
}

void drawRectV4(v4 r, uint8_t p){
	drawRect(r.left, r.top, r.right, r.bottom, p);
}

void drawRect(float x1, float y1, float x2, float y2, uint8_t p){
	SDL_FRect bound = {x1, y1, x2, y2};
	if (p & FILL){
		SDL_RenderFillRectF(renderer, &bound);
	}
	if (p & OUTLINE){
		SDL_RenderDrawRectF(renderer, &bound);
	}
}
