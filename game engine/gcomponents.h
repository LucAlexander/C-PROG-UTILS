#ifndef GCOMPONENTS_H
#define GCOMPONENTS_H

#include "gmath.h"

// COMPONENT ID REGISTRATION
enum COMPONENT_ID{
	HITBOX=0,
	POS2D=1,
	POS3D=2,
	SPRITE=3
};

typedef struct Hitbox{
	v4 rect;
	v2 offset;
}Hitbox;

typedef struct Sprite{
	//TODO
}Sprite;

#endif
