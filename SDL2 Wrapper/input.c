#include "input.h"
#include "gmath.h"

#include <string.h>

static input inp;

void inputInit(){
	memset(inp.heldKeys, false, 256);
	memset(inp.pressedKeys, false, 256);
	memset(inp.releasedKeys, false, 256);
	memset(inp.heldMouseButtons, false, 5);
	memset(inp.pressedMouseButtons, false, 5);
	memset(inp.releasedMouseButtons, false, 5);
	inp.scrollDir = 0;
}

void newInputFrame(){
	memset(inp.pressedKeys, false, 256);
	memset(inp.releasedKeys, false, 256);
	memset(inp.pressedMouseButtons, false, 5);
	memset(inp.releasedMouseButtons, false, 5);
	inp.scrollDir = 0;
}

void mouseScrollEvent(int8_t dir){
	inp.scrollDir = dir;
}

void mouseMoveEvent(int32_t x, int32_t y){
	inp.mouseX = x;
	inp.mouseY = y;
}

int32_t mouseX(){
	return inp.mouseX;
}

int32_t mouseY(){
	return inp.mouseY;
}

v2 mousePos(){
	v2 a = {inp.mouseX, inp.mouseY};
	return a;
}

void mouseUpEvent(const SDL_Event event){
	inp.releasedMouseButtons[event.button.button-1] = true;
	inp.heldMouseButtons[event.button.button-1] = false;
}

void mouseDownEvent(const SDL_Event event){
	inp.pressedMouseButtons[event.button.button-1] = true;
	inp.heldMouseButtons[event.button.button-1] = true;
}

void keyUpEvent(const SDL_Event event){
	inp.releasedKeys[event.key.keysym.scancode] = true;
	inp.heldKeys[event.key.keysym.scancode] = false;
}

void keyDownEvent(const SDL_Event event){
	inp.pressedKeys[event.key.keysym.scancode] = true;
	inp.heldKeys[event.key.keysym.scancode] = true;
}

bool mouseScrolled(int8_t dir){
	return sign(dir)==sign(inp.scrollDir);
}

bool mouseHeld(uint8_t button){
	return inp.heldMouseButtons[button-1];
}

bool mousePressed(uint8_t button){
	return inp.pressedMouseButtons[button-1];
}

bool mouseReleased(uint8_t button){
	return inp.releasedMouseButtons[button-1];
}

bool keyHeld(const char* key){
	SDL_Scancode k = SDL_GetScancodeFromName(key);
	return inp.heldKeys[k];
}

bool keyPressed(const char* key){
	SDL_Scancode k = SDL_GetScancodeFromName(key);
	return inp.pressedKeys[k];
}

bool keyReleased(const char* key){
	SDL_Scancode k = SDL_GetScancodeFromName(key);
	return inp.releasedKeys[k];
}
