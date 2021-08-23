#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <inttypes.h>

/*TODO
 * Fonts: color and alpha for the moment ignored, come back to them
 * 	revisit with rendertext blended
 * Test
 * Sprite component
	sprite
		calls graphics to load const char* src
		responsible for freeing texture data
*/

struct v2;
struct v4;
struct map;

enum RECT_DRAW_PARAMS{
	FILL=1,
	OUTLINE=2
};

typedef struct view{
	float x;
	float y;
	float px;
	float py;
	float pw;
	float ph;
}view;

typedef struct font{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	uint32_t kerning;
	uint32_t leading;
	float ptSize;
	struct map* glyphMap;
}font;

typedef struct fontHandler{
	char* charList;
	char* activeFont;
	struct map* list;
}fontHandler;

void graphicsInit(uint32_t width, uint32_t height, const char* windowTitle);
void graphicsClose();

SDL_Texture* getTexture(const char* src);

void renderSetView(view v);

void renderFlip();
void renderClear();

void renderSetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void blitSurface(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect);
void blitSurfaceEX(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect destRect, double angle, SDL_Point* center, SDL_RendererFlip flip);
void blitSurfaceF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect);
void blitSurfaceEXF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect destRect, double angle, SDL_FPoint* center, SDL_RendererFlip flip);

void drawLineV2(struct v2, struct v2);
void drawLine(float x, float y, float xx, float yy);

void drawRectV2(struct v2, struct v2, uint8_t p);
void drawRectV4(struct v4, uint8_t p);
void drawRect(float x1, float y1, float x2, float y2, uint8_t p);

void fontHandlerInit();
void loadFont(const char* src);
void setFont(char* fnt);
void fontClose(font* f);
void fontHandlerClose();

#endif
