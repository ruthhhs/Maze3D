// ============================================================
// Geometri
// ============================================================
#ifndef GEOMETRI_H_INCLUDED
#define GEOMETRI_H_INCLUDED

#include "coltex.h"

// ================== GRID ==================

#define LUAS_GRID 65
void Grid();

// ================== COLLISION ==================
#define MAX_WALLS 200

typedef struct {
    float minX, maxX;
    float minZ, maxZ;
} Box;

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
    
// ================== LUBANG ==================
#define MAX_HOLES 50

typedef struct {
    float minX, maxX;
    float minZ, maxZ;
} Hole;

extern Hole holes[MAX_HOLES];
extern int holeCount;

void addHole(float x1, float z1, float x2, float z2);
int checkHole(float px, float pz);
void drawHole(float cx, float cz, float radius);

#endif
