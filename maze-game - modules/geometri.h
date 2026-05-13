// ============================================================
// Geometri
// ============================================================
#ifndef GEOMETRI_H_INCLUDED
#define GEOMETRI_H_INCLUDED

#include "coltex.h"

// ================== GRID ==================
void Grid();

// ================== COLLISION ==================
typedef struct {
    float minX, maxX;
    float minZ, maxZ;
} Box;

#define MAX_WALLS 200
extern Box walls[MAX_WALLS];
extern int wallCount;
extern float playerRadius;

void resetWalls();
void addWallFromPoints(float x1, float z1, float x2, float z2);
int checkCollision(float nextX, float nextZ);

// ================== BALOK ==================
static void _drawBalokFaces(
    float x1, float y1, float z1,
    float x2, float y2, float z2);
    
void BalokWarna(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Color c);

void BalokTex(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    GLuint tex);
    
static void Wall(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Color c);

void WallTex(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    GLuint tex);

#endif
