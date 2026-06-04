#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "rumput.h"

// ================== COLLISION RUMPUT ==================
Rumput rumput[MAX_WALLS];
int rumputCount = 0;

void addRumput(float x, float y, float z, float radius)
{
    if (rumputCount >= MAX_WALLS) return;

    rumput[rumputCount].x = x;
    rumput[rumputCount].y = y;
    rumput[rumputCount].z = z;
    rumput[rumputCount].radius = radius;

    rumputCount++;
}

bool checkRumputCollision(float newX, float newY, float newZ)
{
    for (int i = 0; i < rumputCount; i++)
    {
        float dx = newX - rumput[i].x;
        float dy = newY - rumput[i].y;
        float dz = newZ - rumput[i].z;
        float distSq = dx * dx + dz * dz + dy * dy;
        float totalRadius =
            playerRadius + rumput[i].radius;
        if (distSq < totalRadius * totalRadius)
        {
            return true;
        }
    }

    return false;
}


// ==================== RUMPUT ====================
void drawRumput(float tx, float tz) {
	//shadow
	drawCircleShadow(tx, tz, 0.5f);
	
    //collision
	addRumput(tx - 0.2, 4, tz + 0.2, 0.9f);

    // daun
    glColor3f(daun.r, daun.g, daun.b);
    glPushMatrix();
    glTranslatef(tx, 0.0f, tz);
    glutSolidSphere(0.3f, 8, 7);
    glPopMatrix();
}
