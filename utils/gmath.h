#ifndef GMATH_H
#define GMATH_H

#include <inttypes.h>

extern const float PI;

typedef struct v2{
	float x;
	float y;
}v2;

typedef struct v3{
	float x;
	float y;
	float z;
}v3;

typedef struct v4{
	float x;
	float y;
	float w;
	float h;
}v4;

typedef struct v4B{
	float left;
	float top;
	float right;
	float bottom;
}v4B;

float lenDirX(float len, int32_t dir);

float lenDirY(float len, int32_t dir);

int32_t sign(int32_t val);

float distancePoint(v2, v2);

uint32_t pointInRect(v2, v4);

uint32_t pointInRectB(v2, v4B);

#endif
