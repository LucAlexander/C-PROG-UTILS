#ifndef GSYSTEMS_H
#define GSYSTEMS_H

void gmain();

void logicSystemsPre();
void logicSystems();
void logicSystemsPost();

void renderSystems();

// ENGINE ESSENTIAL SYSTEMS

void sysApplyForces();
void sysApplyForceFriction();

// SPRITE AND ANIMATION

void sysAnimateSprites();
void sysDrawSprites();
void sysFreeSpriteData();

// GUI

void sysPressButtons();
void sysDrawGuiNodes();
void sysFreeGuiNodeData();

// PARTICLE SYSTEM

void sysCreateParticles();
void sysDestroyParticles();

#endif
