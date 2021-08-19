#include <math.h>
#include "gmath.h"

float lenDirX(float len, int32_t dir){
	return len * cos(dir*PI/100.0);
}

float lenDirY(float len, int32_t dir){
	return len * -sin(dir*PI/100.0);
}

int32_t sign(int32_t val){
	val == 0 ? 0 : val > 0 ? 1 : -1;
}

float distancePoint(v2 a, v2 b){
	a.x-=b.x;
	a.y-=b.y;;
	a.x *= a.x;
	a.y *= a.y;
	return sqrt(a.x+a.y);
}

uint32_t pointInRect(v2 p, v4 r){
	if (r.left > r.right){
		float temp = r.left;
		r.left = r.right;
		r.right = temp;
	}
	if (r.top > r.bottom){
		float temp = r.top;
		r.top = r.bottom;
		r.bottom = temp;
	}
	return (p.x >= r.top && p.x <= r.right) && 
		(p.y >= r.top && p.y <= r.bottom);
}
