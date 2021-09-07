#include <strutils.h>

void substr(const char* src, char* output, uint32_t position, uint32_t length){
	position += 1;
	uint32_t c = 0;
	while(c < length){
		output[c] = src[position+c-1];
		c++;
	}
	output[c] = '\0';
}
