#include <math.h>
#include <stdlib.h>
#include "gmath.h"

const float PI = 3.1415926;

float lenDirX(float len, int32_t dir){
	return len * cos(dir*PI/100.0);
}

float lenDirY(float len, int32_t dir){
	return len * -sin(dir*PI/100.0);
}

int32_t sign(int32_t val){
	return val == 0 ? 0 : val > 0 ? 1 : -1;
}

float distancePoint(v2 a, v2 b){
	a.x-=b.x;
	a.y-=b.y;;
	a.x *= a.x;
	a.y *= a.y;
	return sqrt(a.x+a.y);
}

uint32_t pointInRect(v2 p, v4 r){
	if (r.w < 0){
		r.w *= -1;
		r.x -= r.w;
	}
	if (r.h < 0){
		r.h *= -1;
		r.y -= r.w;
	}
	return (p.x >= r.x && p.x <= r.x+r.w) && 
		(p.y >= r.y && p.y <= r.y+r.h);

}

uint32_t pointInRectB(v2 p, v4B r){
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
	return (p.x >= r.left && p.x <= r.right) && 
		(p.y >= r.top && p.y <= r.bottom);
}

void approachZero(int32_t* val, int32_t amount){
	uint8_t s = (*val>0);
	int8_t flip = s-(s==0);
	*val += flip * (amount*(*val!=0));
	*val *= (s == (*val>0));
}

void approachZeroF(float* val, float amount){
	uint8_t s = (*val>0);
	int8_t flip = s-(s==0);
	*val -= flip * (amount*(*val!=0));
	*val *= (s == (*val>0));
}

