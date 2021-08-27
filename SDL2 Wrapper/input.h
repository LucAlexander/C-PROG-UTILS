#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

#include <stdbool.h>
#include <inttypes.h>


typedef struct input{
	bool heldKeys[256];
	bool pressedKeys[256];
	bool releasedKeys[256];
	bool heldMouseButtons[5];
	bool pressedMouseButtons[5];
	bool releasedMouseButtons[5];
	int8_t scrollDir;
}input;

void inputInit();

void newInputFrame();

void mouseScrollEvent(int8_t);

void mouseUpEvent(const SDL_Event event);
void mouseDownEvent(const SDL_Event event);

void keyUpEvent(const SDL_Event event);
void keyDownEvent(const SDL_Event event);

bool mouseScrolled(int8_t);

bool mouseHeld(uint8_t);
bool mousePressed(uint8_t);
bool mouseReleased(uint8_t);

bool keyHeld(const char*);
bool keyPressed(const char*);
bool keyReleased(const char*);

#endif
