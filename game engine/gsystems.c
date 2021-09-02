#include "gsystems.h"
#include "gcomponents.h"
#include "ecs.h"
#include "gtime.h"

#include "graphics.h"
#include "input.h"

#include <stdio.h>
#include <string.h>

void gmain(){
	ecsInit(4, sizeof(Hitbox), sizeof(v2), sizeof(v3), sizeof(Sprite));
	// TEST CODE
	view v = {
		0, 0,
		0, 0, 
		640, 480
	};
	renderSetView(v);
	for (uint32_t i = 0;i<25;++i){
		uint32_t entity = entCreate();
		v2 pos = {64+i*80, 64};
		Hitbox hb = {
			{0, 0, 64, 64}, 
			{32, 32}
		};
		Sprite spr;
		spriteInit(&spr, getTexture("loading.png"), 4, 32, 32);
		Animation load = {{0, 0, 32, 32}, 32, true};
		addSpriteAnimation(&spr, load);
		playAnimation(&spr, 0);
		entAdd(entity, POS2D, &pos);
		entAdd(entity, HITBOX, &hb);
		entAdd(entity, SPRITE, &spr);
	}
}

void logicSystemsPre(){}

void logicSystems(){
	sysAnimateSprites();


	// TEST CODE
	ComponentQuery* q = ecsQuery(2, POS2D, HITBOX);
	uint32_t i;
	for(i = 0;i<q->count;++i){
		v2* pos = entGet(q->list[i], POS2D);
		Hitbox* hb = entGet(q->list[i], HITBOX);
		hb->rect.x = pos->x - hb->offset.x;
		hb->rect.y = pos->y - hb->offset.y;
	}
	if (keyPressed("A")){
		for (i = 0;i<25;++i){
			entDestroy(i);
		}
	}
	if (keyPressed("S")){
		for (i = 0;i<25;++i){
			uint32_t entity = entCreate();
			v2 pos = {64+i*80, 64};
			Hitbox hb = {
				{0, 0, 64, 64}, 
				{32, 32}
			};
			Sprite spr;
			spriteInit(&spr, getTexture("loading.png"), 4, 32, 32);
			Animation load = {{0, 0, 32, 32}, 32, true};
			addSpriteAnimation(&spr, load);
			playAnimation(&spr, 0);
			entAdd(entity, POS2D, &pos);
			entAdd(entity, HITBOX, &hb);
			entAdd(entity, SPRITE, &spr);
		}
	}
}

void logicSystemsPost(){
	sysFreeSpriteData();
}

void renderSystems(){
	sysDrawSprites();

	//TEST CODE
	ComponentQuery* q = ecsQuery(1, HITBOX);
	renderSetColor(255, 255, 0, 255);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		Hitbox* hb = entGet(q->list[i], HITBOX);
		drawRectV4(hb->rect, OUTLINE);
	}
}

// SPRITES AND ANIMATIONS

void sysAnimateSprites(){
	ComponentQuery* q = ecsQuery(1, SPRITE);
	uint32_t i;
	for(i = 0;i<q->count;++i){
		Sprite* spr = entGet(q->list[i], SPRITE);
		incrementAnimationFrame(spr);
	}
}

void sysDrawSprites(){
	ComponentQuery* q = ecsQuery(2, POS2D, SPRITE);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		Sprite* spr = entGet(q->list[i], SPRITE);
		v2* pos = entGet(q->list[i], POS2D);
		drawSprite(spr, pos);
	}
}

void sysFreeSpriteData(){
	ComponentQuery* q = ecsQueryAlive(false, 1, SPRITE);
	uint32_t i;
	for(i = 0;i<q->count;++i){
		Sprite* spr = entGet(q->list[i], SPRITE);
		SDL_DestroyTexture(spr->texture);
		spr->texture = NULL;

	}
}

