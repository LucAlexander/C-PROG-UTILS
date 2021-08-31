#include "gsystems.h"
#include "gcomponents.h"
#include "ecs.h"
#include "gtime.h"

#include "graphics.h"

#include <stdio.h>

void gmain(){
	ecsInit(4, sizeof(Hitbox), sizeof(v2), sizeof(v3), sizeof(Sprite));
	// TEST CODE
	view v = {
		0, 0,
		0, 0, 
		640, 480
	};
	renderSetView(v);
	uint32_t entity = entCreate();
	v2 pos = {64, 64};
	Hitbox hb = {
		{0, 0, 64, 64}, 
		{32, 32}
	};
	entAdd(entity, POS2D, &pos);
	entAdd(entity, HITBOX, &hb);
}

void logicSystemsPre(){}

void logicSystems(){
	ComponentQuery* q = ecsQuery(2, POS2D, HITBOX);
	uint32_t i;
	for(i = 0;i<q->count;++i){
		v2* pos = entGet(q->list[i], POS2D);
		Hitbox* hb = entGet(q->list[i], HITBOX);
		pos->x += 1 * gTimeDelta();
		pos->y += 1 * gTimeDelta();
		hb->rect.x = pos->x - hb->offset.x;
		hb->rect.y = pos->y - hb->offset.y;
	}
}

void logicSystemsPost(){}

void renderSystems(){
	ComponentQuery* q = ecsQuery(1, HITBOX);
	renderSetColor(255, 255, 0, 255);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		Hitbox* hb = entGet(q->list[i], HITBOX);
		drawRectV4(hb->rect, OUTLINE);
	}
}
