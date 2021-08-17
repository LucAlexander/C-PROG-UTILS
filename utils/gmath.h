#ifndef GMATH_H
#define GMATH_H

#include <inttypes.h>

const float PI = 3.141592;

typedef struct{
	float x;
	float y;
}v2;

typedef struct{
	float x;
	float y;
	float z;
}v3;

typedef struct{
	float left;
	float top;
	float right;
	float bottom;
}Rect;

typedef struct{
	v3 a;
	v3 b;
	v3 c;
	v3 d;
	v3 e;
	v3 f;
	v3 g;
	v3 h;
}Cube;


float lenDirX(float len, uint32_t dir);

float lenDirY(float len, uint32_t dir);

uint32_t sign(uint32_t val);

float v2DistancePoint(v2, v2);

float v3DistancePoint(v3, v3);

uint32_t pointInRect(v2, Rect);

#endif
