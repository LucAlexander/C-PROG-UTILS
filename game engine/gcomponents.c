#include "gcomponents.h"
#include "graphics.h"
#include "gtime.h"

#include <string.h>
#include <stdio.h>

// SPRITE AND ANIMATIONS

void loopSpriteAnimationBound(Sprite* s){
	Animation a = s->anims[s->currentAnimation];
	if(s->frame == a.frameCount-1){
		s->play = a.repeat;
		if (s->play){
			s->drawBound = a.frames;
			s->frame = 0;
		}
		return;
	}
	moveSpriteAnimationBound(s);
}

void moveSpriteAnimationBound(Sprite* s){
	s->drawBound.x += s->drawBound.w;
	if (s->drawBound.x >= s->textureW){
		s->drawBound.x = 0;
		s->drawBound.y += s->drawBound.h;
	}
	if (s->drawBound.y >= s->textureH){
		s->drawBound.y = 0;
	}
	s->frame++;
}

void incrementAnimationFrame(Sprite* s){
	if (s->currentAnimation == -1||!s->play){
		return;
	}
	s->timeElapsed += gTimeDelta();
	if (s->timeElapsed >= s->frameSpeed){
		s->timeElapsed -= s->frameSpeed;
		loopSpriteAnimationBound(s);
	}
}

void spriteInit(Sprite* s, SDL_Texture* t, uint32_t frameSpeed, uint32_t w, uint32_t h){
	memset(s->anims, 0, 32*sizeof(Animation));
	s->texture = t;
	SDL_Rect rect = {0, 0, w, h};
	s->drawBound = rect;
	s->animations = 0;
	s->frameSpeed = frameSpeed;
	s->timeElapsed = 0;
	s->displayW = w;
	s->displayH = h;
	s->frame = -1;
	SDL_QueryTexture(t, NULL, NULL, &s->textureW, &s->textureH);
	s->currentAnimation = -1;
	s->play = false;
}

void addSpriteAnimation(Sprite* s, Animation a){
	s->anims[s->animations++] = a;
}

void playAnimation(Sprite* s, uint32_t index){
	s->currentAnimation = index;
	s->drawBound = s->anims[s->currentAnimation].frames;
	s->play = true;
	s->frame = 0;
}

void drawSprite(Sprite* s, v2* pos){
	//TODO extend blit surface params[angle etc]
	SDL_Rect dest = {pos->x, pos->y, s->displayW, s->displayH};
	blitSurface(s->texture, &s->drawBound, dest);
}
