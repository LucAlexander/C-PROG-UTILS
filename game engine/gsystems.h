#ifndef GSYSTEMS_H
#define GSYSTEMS_H

void gmain();

void logicSystemsPre();
void logicSystems();
void logicSystemsPost();

void renderSystems();

// ENGINE ESSENTIAL SYSTEMS

// SPRITE AND ANIMATION

void sysAnimateSprites();
void sysDrawSprites();
void sysFreeSpriteData();

// GUI

void sysPressButtons();
void sysDrawGuiNodes();
void sysFreeGuiNodeData();

#endif
