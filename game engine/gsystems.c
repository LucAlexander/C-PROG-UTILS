#include "gsystems.h"
#include "gcomponents.h"
#include "ecs.h"
#include "gtime.h"

#include "graphics.h"
#include "input.h"

#include <stdio.h>
#include <string.h>

void func(GuiNode* n){
	printf("Button Content Signiture: %u\n", n->registry);
}

void gmain(){
	ecsInit(8, 
		sizeof(Hitbox), 
		sizeof(v2), 
		sizeof(v3), 
		sizeof(Sprite), 
		sizeof(GuiNode), 
		sizeof(Forces), 
		sizeof(ParticleSystem), 
		sizeof(Particle)
	);
	// TEST CODE
	view v = {
		0, 0,
		0, 0, 
		640, 480
	};
	loadFont("default.ttf");
	ecsStats();
	setFont("default.ttf");
	ecsStats();
	renderSetView(v);
	for (uint32_t i = 0;i<0;++i){
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
	if (false){
		uint32_t greenSqr = entCreate();
		v3 pos = {128, 128, 32};
		Hitbox hb = {{0, 0, 32, 32}, {16, 16}};
		Sprite s;
		spriteInit(&s, getTexture("green.png"), 1, 32, 32);
		s.angle = -405;
		GuiNode node;
		Text txt = {"Text Content", {160, 160}, {128, 256, 0, 200}};
		v4 dim = {160, 160, 200, 200};
		v4 col = {100, 100, 100, 255};
		guiNodeTextContent(&node, txt);
		guiNodeTextContent(&node, txt);
		//guiNodeRemoveTextContent(&node);
		guiNodePanel(&node, dim, col);
		guiNodePanel(&node, dim, col);
		//guiNodeRemovePanel(&node);
		guiNodeVisible(&node, true);
		guiNodePressable(&node, true);
		guiNodeSetPressBound(&node, dim);
		node.action = &func;
		Sprite ts;
		spriteInit(&ts, getTexture("green.png"), 1, 32, 32);
		v2 tpos = {200, 200};
		guiNodeTexture(&node, ts, tpos);
		guiNodeTexture(&node, ts, tpos);
		//guiNodeRemoveTexture(&node);
		entAdd(greenSqr, POS3D, &pos);
		entAdd(greenSqr, HITBOX, &hb);
		entAdd(greenSqr, SPRITE, &s);
		entAdd(greenSqr, GUINODE, &node);
	}
	// set sprite scale
	renderSetSpriteScale(1, 1);
}

void logicSystemsPre(){}

void logicSystems(){
	sysAnimateSprites();
	sysCreateParticles();
	sysDestroyParticles();
	if(mousePressed(1)){
		sysPressButtons();
	}
	sysApplyForces();
	sysApplyForceFriction();


	// TEST CODE
	ComponentQuery* q = ecsQuery(2, POS2D, HITBOX);
	uint32_t i;
	for(i = 0;i<q->count;++i){
		v2* pos = entGet(q->list[i], POS2D);
		Hitbox* hb = entGet(q->list[i], HITBOX);
		hb->rect.x = pos->x - hb->offset.x;
		hb->rect.y = pos->y - hb->offset.y;
	}
	q = ecsQuery(2, POS3D, HITBOX);
	for(i = 0;i<q->count;++i){
		v3* pos = entGet(q->list[i], POS3D);
		Hitbox* hb = entGet(q->list[i], HITBOX);
		hb->rect.x = pos->x - hb->offset.x;
		hb->rect.y = pos->y - hb->offset.y;
	}
	if (keyPressed("F")){
		toggleFullscreen();
	}
	if (keyPressed("A")){
		for (i = 0;i<512;++i){
			entDestroy(i);
		}
	}
	if (keyPressed("D")){
		ecsPrint();
	}
	if (keyPressed("S")){
		for (i = 0;i<512;++i){
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
	if (keyPressed("P")){
		uint32_t entity = entCreate();
		v2 mp = mousePos();
		ParticleSystem ps = {50, 50, 8, 4, 0};
		printf("Created a particle system\n");
		entAdd(entity, POS2D, &mp);
		printf("added position\n");
		entAdd(entity, PARTICLESYSTEM, &ps);
		printf("Created a system\n");
	}
	if (keyPressed("O")){
		ecsStats();
	}
}

void logicSystemsPost(){
	sysFreeSpriteData();
	sysFreeGuiNodeData();
}

void renderSystems(){
	sysDrawSprites();
	sysDrawGuiNodes();

	//TEST CODE
	ComponentQuery* q = ecsQuery(1, HITBOX);
	renderSetColor(255, 255, 0, 255);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		Hitbox* hb = entGet(q->list[i], HITBOX);
		drawRectV4(hb->rect, OUTLINE);
	}
}

// SYSTEM IMPLEMENTATIONS

void sysApplyForces(){
	ComponentQuery* q = ecsQuery(2, POS2D, FORCES);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		v2* pos = entGet(q->list[i], POS2D);
		Forces* f = entGet(q->list[i], FORCES);
		pos->x += f->h + lenDirX(f->directional, f->direction);
		pos->y += f->v + lenDirY(f->directional, f->direction);
	}
}

void sysApplyForceFriction(){
	ComponentQuery* q = ecsQuery(1, FORCES);
	uint32_t i;
	uint32_t time = gTimeDelta();
	for (i = 0;i<q->count;++i){
		Forces* f = entGet(q->list[i], FORCES);
		f->h -= time;
		f->v -= time;
		f->directional -= time;
		f->direction -= time;
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
	ComponentQuery* q2 = ecsQuery(2, POS2D, SPRITE);
	uint32_t i;
	for (i = 0;i<q2->count;++i){
		Sprite* spr = entGet(q2->list[i], SPRITE);
		v2* pos = entGet(q2->list[i], POS2D);
		drawSprite(spr, pos);
	}
	ComponentQuery* q3 = ecsQuery(2, POS3D, SPRITE);
	for (i = 0;i<q3->count;++i){
		Sprite* spr = entGet(q3->list[i], SPRITE);
		v3* pos = entGet(q3->list[i], POS3D);
		drawSpriteV3(spr, pos);
	}
}

void sysFreeSpriteData(){
	ComponentQuery* q = ecsQueryAlive(false, 1, SPRITE);
	uint32_t i;
	for(i = 0;i<q->count;++i){
		Sprite* spr = entGet(q->list[i], SPRITE);
		freeSpriteData(spr);
	}
}

// GUI

void sysPressButtons(){
	ComponentQuery* q = ecsQuery(1, GUINODE);
	uint32_t i;
	v2 mp = mousePos();
	for (i = 0;i<q->count;++i){
		GuiNode* n = entGet(q->list[i], GUINODE);
		if (pointInRect(worldToView(mp.x, mp.y), n->pressBound)){
			(*n->action)(n);
			mouseInterrupt(1);
			return;
		}
	}
}

void sysDrawGuiNodes(){
	ComponentQuery* q = ecsQuery(1, GUINODE);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		GuiNode* n = entGet(q->list[i], GUINODE);
		guiNodeDraw(n);
	}
}

void sysFreeGuiNodeData(){
	ComponentQuery* q = ecsQueryAlive(false, 1, GUINODE);
	uint32_t i;
	for(i = 0;i<q->count;++i){
		GuiNode* n = entGet(q->list[i], GUINODE);
		freeGuiNodeData(n);
	}
}

// PARTICLE SYSTEM

void sysCreateParticles(){
	ComponentQuery* q = ecsQuery(2, POS2D, PARTICLESYSTEM);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		uint32_t id = q->list[i];
		ParticleSystem* psys = entGet(id, PARTICLESYSTEM);
		v2* pos = entGet(id, POS2D);
		partSysTryEmit(psys, pos, id);
	}
}

void sysDestroyParticles(){
	ComponentQuery* q = ecsQuery(1, PARTICLE);
	uint32_t i;
	for (i = 0;i<q->count;++i){
		uint32_t id = q->list[i];
		Particle* p = entGet(id, PARTICLE);
		p->life -= gTimeDelta();
		if (p->life <= 0){
			entDestroy(id);
		}
	}
}
