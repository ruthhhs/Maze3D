// ========================================================
// RUMPUT
// ========================================================
#ifndef RUMPUT_H_INCLUDED
#define RUMPUT_H_INCLUDED

#include "geometri.h"
#include "lighting.h"

#define MAX_WALLS 200

struct Rumput {
    float x, y, z;
    float radius;
};

extern Rumput rumput[MAX_WALLS];
extern int rumputCount;
extern float playerRadius;

void addRumput(float x, float z, float radius);
bool checkRumputCollision(float newX, float newY, float newZ);
void drawRumput(float tx, float tz);

#endif
