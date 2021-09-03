#ifndef GCOMPONENTS_H
#define GCOMPONENTS_H

#include "gmath.h"
#include "hashmap.h"

#include <stdbool.h>

#include <SDL2/SDL.h>

// COMPONENT ID REGISTRATION
enum COMPONENT_ID{
	HITBOX=0,
	POS2D=1,
	POS3D=2,
	SPRITE=3
};

// HITBOX
typedef struct Hitbox{
	v4 rect;
	v2 offset;
}Hitbox;

// SPRITE AND ANIMATIONS
typedef struct Animation{
	SDL_Rect frames;
	uint32_t frameCount;
	bool repeat;
}Animation;

typedef struct Sprite{
	Animation anims[32];
	SDL_Texture* texture;
	SDL_Rect drawBound;
	uint32_t animations;
	uint32_t frameSpeed;
	uint32_t timeElapsed;
	uint32_t frame;
	uint32_t displayW;
	uint32_t displayH;
	int32_t textureW;
	int32_t textureH;
	int32_t currentAnimation;
	bool play;
	bool visible;
	double angle;
	SDL_FPoint center;
}Sprite;

void spriteInit(Sprite* s, SDL_Texture*, uint32_t frameSpeed, uint32_t w, uint32_t h);

void addSpriteAnimation(Sprite* s, Animation a);

void playAnimation(Sprite* s, uint32_t index);

void incrementAnimationFrame(Sprite* s);

void moveSpriteAnimationBound(Sprite*);

void loopSpriteAnimationBuffer(Sprite* s);

void drawSprite(Sprite* s, v2* pos);

void drawSpriteV3(Sprite* s, v3* pos);

#endif
