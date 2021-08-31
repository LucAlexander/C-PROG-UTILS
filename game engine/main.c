#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#include "graphics.h"
#include "input.h"
#include "gsystems.h"
#include "gmath.h"
#include "gtime.h"

int main(int argc, char** argv){
	SDL_Init( SDL_INIT_EVERYTHING );
	graphicsInit(640, 480, "Test Window");
	inputInit();
	gameTimeInit(128);
	gmain();
	SDL_Event event;
	bool run = true;
	while(run){
		if(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					run = false;
				break;
				case SDL_KEYDOWN:
					if (event.key.repeat==0){
						keyDownEvent(event);
					}
				break;
				case SDL_KEYUP:
					keyUpEvent(event);
				break;
				case SDL_MOUSEBUTTONDOWN:
					if (!mouseHeld(event.button.button)){
						mouseDownEvent(event);
					}
				break;
				case SDL_MOUSEBUTTONUP:
					mouseUpEvent(event);
				break;
				case SDL_MOUSEWHEEL:
					mouseScrollEvent(event.wheel.y);
				break;
				case SDL_MOUSEMOTION:
					mouseMoveEvent(event.motion.x, event.motion.y);
				break;
			}
		}
		gameTimeUpdate();
		if (gameTimeTick()){
			gameTimeReset();
			logicSystemsPre();
			logicSystems();
			logicSystemsPost();
			newInputFrame();
			renderClear();
			renderSystems();
			renderSetColor(0, 0, 0, 255);
			renderFlip();
		}
	}
	graphicsClose();
	printf("0\n");
	return 0;    
}
