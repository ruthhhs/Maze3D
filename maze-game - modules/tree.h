#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

struct CircleWall {
    float x, z;
    float radius;
};

#define MAX_WALLS 200
extern CircleWall circleWalls[MAX_WALLS];
extern int circleWallCount;
extern float playerRadius;

// ================== POHON ==================
void addCircleWall(float x, float z, float radius);
bool checkCircleCollision(float newX, float newZ);
void initTreeCollision();
void drawTree(float tx, float tz);
void drawSmallTree(float tx, float tz);

#endif
