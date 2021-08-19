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
}v4;

float lenDirX(float len, int32_t dir);

float lenDirY(float len, int32_t dir);

int32_t sign(int32_t val);

float distancePoint(v2, v2);

uint32_t pointInRect(v2, v4);

#endif
