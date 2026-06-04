#include <math.h>
#include <GL/glut.h>

#include "chest.h"

bool nearChest = false;
bool chestOpened = false;

float chestAngle = 0.0f;

// ================= CHECK =================

void checkChest(float playerX, float playerZ,
                float chestX, float chestZ)
{
    float dx = playerX - chestX;
    float dz = playerZ - chestZ;

    float distSq = dx * dx + dz * dz;

    nearChest = (distSq < 3.0f * 3.0f);
}

// ================= OPEN =================

void openChest()
{
    if (nearChest)
    {
        chestOpened = true;
    }
}

// ================= UPDATE =================

void updateChest()
{
    if (chestOpened)
    {
        // buka pelan
        if (chestAngle < 110.0f)
        {
            chestAngle += 1.0f;
        }
    }
}

// ================= DRAW =================

void drawChest(float x, float y, float z)
{
    glPushMatrix();

    glTranslatef(x, y, z);

    // ================= bawah =================

    glColor3f(0.55f, 0.27f, 0.07f);

    glPushMatrix();
    glScalef(2.0f, 1.0f, 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ================= tutup =================

    glPushMatrix();

    // pivot belakang
    glTranslatef(0.0f, 0.5f, -0.6f);

    glRotatef(-chestAngle, 1.0f, 0.0f, 0.0f);

    glTranslatef(0.0f, 0.2f, 0.6f);

    glColor3f(0.65f, 0.35f, 0.10f);

    glPushMatrix();
    glScalef(2.0f, 0.4f, 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}
