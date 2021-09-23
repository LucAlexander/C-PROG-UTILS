#include "gcomponents.h"
#include "graphics.h"
#include "gtime.h"
#include "ecs.h"
#include "rng.h"

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
	s->visible = true;
	s->angle = 0;
	SDL_FPoint point = {w/2, h/2};
	s->center = point;
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
	if(s->visible){
		SDL_FRect dest = {pos->x-s->center.x, pos->y-s->center.y, s->displayW, s->displayH};
		blitSurfaceEXF(s->texture, &s->drawBound, dest, s->angle, &s->center, SDL_FLIP_NONE);
	}
}

void drawSpriteV3(Sprite* s, v3* pos){
	v2 altered = {pos->x, pos->y-(pos->z/2)};
	drawSprite(s, &altered);
}

void freeSpriteData(Sprite* s){
	SDL_DestroyTexture(s->texture);
	s->texture = NULL;
}

// GUI

void guiNodeTextContent(GuiNode* n, Text t){
	n->textContent = t;
	n->registry |= 1;
}

void guiNodeRemoveTextContent(GuiNode* n){
	n->registry &= ~(1);
}

void guiNodePanel(GuiNode* n, v4 dimensions, v4 color){
	n->panel = dimensions;
	n->panelColor = color;
	n->registry |= 2;
}

void guiNodeRemovePanel(GuiNode* n){
	n->registry &= ~(2);
}

void guiNodePressable(GuiNode* n, bool pressable){
	if(pressable){
		n->registry |= (1<<2);
		return;
	}
	n->registry &= ~(1<<2);
	n->pressBound.x = 0;
	n->pressBound.y = 0;
	n->pressBound.w = 0;
	n->pressBound.h = 0;
}

void guiNodeSetPressBound(GuiNode* n, v4 dimensions){
	n->pressBound = dimensions;
}

void guiNodeVisible(GuiNode* n, bool visible){
	if (visible){
		n->registry |= (1<<3);
		return;
	}
	n->registry &= ~(1<<3);
}


void guiNodeDrawPanel(GuiNode* n){
	if (n->registry & 2){
		v4* c = &n->panelColor;
		renderSetColor(c->x, c->y, c->w, c->h);
		drawRectV4(n->panel, FILL);
		renderSetColor(0, 0, 0, 0);
	}
}

void guiNodeDrawTexture(GuiNode* n){
	if (n->registry & (1<<4)){
		drawSprite(&n->texture, &n->spritePos);
	}
}

void guiNodeDrawText(GuiNode* n){
	if (n->registry & 1){
		Text* temp = &n->textContent;
		drawTextV2C(temp->pos, (char*)temp->content, temp->color.x, temp->color.y, temp->color.w, temp->color.h);
	}
}

void guiNodeDraw(GuiNode* n){
	if (n->registry & (1<<3)){
		guiNodeDrawPanel(n);
		guiNodeDrawTexture(n);
		guiNodeDrawText(n);
	}
}

void guiNodeTexture(GuiNode* n, Sprite t, v2 pos){
	freeSpriteData(&n->texture);
	n->texture = t;
	n->spritePos = pos;
	n->registry |= (1<<4);
}

void guiNodeRemoveTexture(GuiNode* n){
	freeSpriteData(&n->texture);
	n->registry &= ~(1<<4);
}

void freeGuiNodeData(GuiNode* n){
	freeSpriteData(&n->texture);
}

// PARTICLE SYSTEM

void partSysTryEmit(ParticleSystem* psys, v2* pos, uint32_t id){
	psys->timeToNextEmit -= gTimeDelta();
	if (psys->timeToNextEmit <= 0){
		psys->timeToNextEmit = psys->timeBetweenEmits;
		partSysEmit(psys, pos, id);
	}
}

void partSysEmit(ParticleSystem* psys, v2* pos, uint32_t id){
	if (psys->emitCount-- <= 0){
		entDestroy(id);
		return;
	}
	uint32_t k;
	for(k = 0;k<psys->particlesPerEmit;++k){
		//EMIT PARTICLE TODO
		// test implementation
		uint32_t p = entCreate();
		v2 partPos = {pos->x, pos->y};
		Particle part = {100};
		Forces partForce = {5, 5, 0, 0};
		Sprite s;
		spriteInit(&s, getTexture("particle.png"), 1, 4, 4);
		entAdd(p, POS2D, &partPos);
		entAdd(p, SPRITE, &s);
		entAdd(p, FORCES, &partForce);
		entAdd(p, PARTICLE, &part);
	}
}

