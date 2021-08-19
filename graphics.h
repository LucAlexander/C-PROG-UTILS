#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <inttypes.h>

/*TODO
 * View
 * loadMedia
 * Sprite and texture handling
 * Font
 * Test
*/

struct v2;
struct v4;

enum RECT_DRAW_PARAMS{
	FILL=1,
	OUTLINE=2
};

void graphicsInit(uint32_t width, uint32_t height, const char* windowTitle);
void graphicsClose();

void renderFlip();
void renderClear();

void renderSetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void blitSurface(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect);
void blitSurfaceEX(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect, double angle, SDL_Point* center, SDL_RendererFlip flip);
void blitSurfaceF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect* destRect);
void blitSurfaceEXF(SDL_Texture* texture, SDL_Rect* srcRect, SDL_FRect* destRect, double angle, SDL_FPoint* center, SDL_RendererFlip flip);

void drawLineV2(struct v2, struct v2);
void drawLine(float x, float y, float xx, float yy);

void drawRectV2(struct v2, struct v2, uint8_t p);
void drawRectV4(struct v4, uint8_t p);
void drawRect(float x1, float y1, float x2, float y2, uint8_t p);

#endif
