#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "geometri.h"

// ================== GRID ==================
void Grid()
{
    double i;
    const float Z_MIN = -50;
    const float Z_MAX = 50;
    const float X_MIN = -50;
    const float X_MAX = 50;
    const float gap = 2;

    glColor3f(0.5, 0.5, 0.5);

    glBegin(GL_LINES);
    for (float i = X_MIN; i <= X_MAX; i += 1.5)
    {
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);

        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
    }
    glEnd();
}

// ================== COLLISION ==================
Box walls[MAX_WALLS];

int wallCount = 0;
float playerRadius = 0.4f;

void resetWalls()
{
    wallCount = 0;
}

void addWallFromPoints(float x1, float z1, float x2, float z2)
{
    if (wallCount >= MAX_WALLS) return;

    float minX = x1 < x2 ? x1 : x2;
    float maxX = x1 > x2 ? x1 : x2;
    float minZ = z1 < z2 ? z1 : z2;
    float maxZ = z1 > z2 ? z1 : z2;

    walls[wallCount].minX = minX;
    walls[wallCount].maxX = maxX;
    walls[wallCount].minZ = minZ;
    walls[wallCount].maxZ = maxZ;
    wallCount++;
}

int checkCollision(float nextX, float nextZ)
{
    for (int i = 0; i < wallCount; i++)
    {
        if (nextX + playerRadius > walls[i].minX &&
            nextX - playerRadius < walls[i].maxX &&
            nextZ + playerRadius > walls[i].minZ &&
            nextZ - playerRadius < walls[i].maxZ)
        {
            return 1;
        }
    }
    return 0;
}

// ================== BALOK ==================
static void _drawBalokFaces(
    float x1, float y1, float z1,
    float x2, float y2, float z2)
{
    float minX = x1 < x2 ? x1 : x2;
    float maxX = x1 > x2 ? x1 : x2;
    float minY = y1 < y2 ? y1 : y2;
    float maxY = y1 > y2 ? y1 : y2;
    float minZ = z1 < z2 ? z1 : z2;
    float maxZ = z1 > z2 ? z1 : z2;

    float sizeX = fabs(maxX - minX);
    float sizeY = fabs(maxY - minY);
    float sizeZ = fabs(maxZ - minZ);

    // skala repeat (ubah angka 2.0 sesuai kebutuhan)
    float repX = sizeX / 2.0f;
    float repY = sizeY / 2.0f;
    float repZ = sizeZ / 2.0f;

    glBegin(GL_QUADS);

    // DEPAN (+Z) -> pakai X,Y
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);         glVertex3f(minX,minY,maxZ);
    glTexCoord2f(repX,0);      glVertex3f(maxX,minY,maxZ);
    glTexCoord2f(repX,repY);   glVertex3f(maxX,maxY,maxZ);
    glTexCoord2f(0,repY);      glVertex3f(minX,maxY,maxZ);

    // BELAKANG (-Z) -> pakai X,Y
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);         glVertex3f(minX,minY,minZ);
    glTexCoord2f(repX,0);      glVertex3f(maxX,minY,minZ);
    glTexCoord2f(repX,repY);   glVertex3f(maxX,maxY,minZ);
    glTexCoord2f(0,repY);      glVertex3f(minX,maxY,minZ);

    // ATAS (+Y) -> pakai X,Z
    glNormal3f(0,1,0);
    glTexCoord2f(0,0);         glVertex3f(minX,maxY,minZ);
    glTexCoord2f(repX,0);      glVertex3f(maxX,maxY,minZ);
    glTexCoord2f(repX,repZ);   glVertex3f(maxX,maxY,maxZ);
    glTexCoord2f(0,repZ);      glVertex3f(minX,maxY,maxZ);

    // BAWAH (-Y) -> pakai X,Z
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0);         glVertex3f(minX,minY,minZ);
    glTexCoord2f(repX,0);      glVertex3f(maxX,minY,minZ);
    glTexCoord2f(repX,repZ);   glVertex3f(maxX,minY,maxZ);
    glTexCoord2f(0,repZ);      glVertex3f(minX,minY,maxZ);

    // KANAN (+X) -> pakai Z,Y
    glNormal3f(1,0,0);
    glTexCoord2f(0,0);         glVertex3f(maxX,minY,minZ);
    glTexCoord2f(repZ,0);      glVertex3f(maxX,minY,maxZ);
    glTexCoord2f(repZ,repY);   glVertex3f(maxX,maxY,maxZ);
    glTexCoord2f(0,repY);      glVertex3f(maxX,maxY,minZ);

    // KIRI (-X) -> pakai Z,Y
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0);         glVertex3f(minX,minY,minZ);
    glTexCoord2f(repZ,0);      glVertex3f(minX,minY,maxZ);
    glTexCoord2f(repZ,repY);   glVertex3f(minX,maxY,maxZ);
    glTexCoord2f(0,repY);      glVertex3f(minX,maxY,minZ);

    glEnd();
}

// ================== BALOK WARNA & TEXTURE ==================
void BalokWarna(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Color c)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(c.r, c.g, c.b);
    _drawBalokFaces(x1, y1, z1, x2, y2, z2);
}

void BalokTex(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    GLuint tex)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glColor3f(1,1,1); // putih
    _drawBalokFaces(x1, y1, z1, x2, y2, z2);

    glDisable(GL_TEXTURE_2D);
}

// ================== DINDING WARNA & TEXTURE ==================
static void Wall(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Color c)
{
    BalokWarna(x1, y1, z1, x2, y2, z2, c);
    addWallFromPoints(x1, z1, x2, z2);
}

void WallTex(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    GLuint tex)
{
    BalokTex(x1, y1, z1, x2, y2, z2, tex);
    addWallFromPoints(x1, z1, x2, z2);
}

// ==================== POHON BESAR ====================
void drawTree(float tx, float tz) {
    // collision
    addWallFromPoints(tx - 2.0f, tz - 2.0f, tx + 2.0f, tz + 2.0f);

    // batang
    glColor3f(0.40f, 0.25f, 0.08f);
    glPushMatrix();
    glTranslatef(tx, 0, tz);
    glRotatef(-90, 1, 0, 0);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, 0.35f, 0.25f, 4.0f, 8, 4);
    gluDeleteQuadric(q);
    glPopMatrix();

    // daun
    float leafColors[3][3] = {
        {0.15f, 0.50f, 0.08f},
        {0.20f, 0.58f, 0.12f},
        {0.25f, 0.65f, 0.15f}
    };

    float layerY[3] = {3.0f, 5.0f, 7.0f};
    float layerRad[3] = {3.2f, 2.4f, 1.5f};

    for (int i = 0; i < 3; i++) {
        glColor3f(
            leafColors[i][0],
            leafColors[i][1],
            leafColors[i][2]
        );
        glPushMatrix();
        glTranslatef(tx, layerY[i], tz);
        glutSolidSphere(layerRad[i], 10, 8);
        glPopMatrix();
    }
}

// ==================== POHON KECIL ====================
void drawSmallTree(float tx, float tz) {
    // collision
    addWallFromPoints(tx - 0.8f, tz - 0.8f, tx + 0.8f, tz + 0.8f);

    // batang
    glColor3f(0.35f, 0.20f, 0.07f);
    glPushMatrix();
    glTranslatef(tx, 0, tz);
    glRotatef(-90, 1, 0, 0);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, 0.10f, 0.05f, 1.5f, 4, 3);
    gluDeleteQuadric(q);
    glPopMatrix();

    // daun
    glColor3f(0.18f, 0.52f, 0.10f);
    glPushMatrix();
    glTranslatef(tx, 2.0f, tz);
    glutSolidSphere(1.2f, 8, 7);
    glPopMatrix();
}

// ==================== LUBANG ====================
Hole holes[MAX_HOLES];
int holeCount = 0;

void addHole(float x1, float z1, float x2, float z2) {
    if (holeCount >= MAX_HOLES)
        return;
	
    float minX = x1 < x2 ? x1 : x2;
    float maxX = x1 > x2 ? x1 : x2;

    float minZ = z1 < z2 ? z1 : z2;
    float maxZ = z1 > z2 ? z1 : z2;

    holes[holeCount].minX = minX;
    holes[holeCount].maxX = maxX;

    holes[holeCount].minZ = minZ;
    holes[holeCount].maxZ = maxZ;

    holeCount++;
}

int checkHole(float px, float pz) {
    for (int i = 0; i < holeCount; i++) {
        if (
            px > holes[i].minX &&
            px < holes[i].maxX &&
            pz > holes[i].minZ &&
            pz < holes[i].maxZ
        )
        {
            return 1;
        }
    }
    return 0;
}

void drawHole(float cx, float cz, float radius) {
    addHole(
        cx - radius,
        cz - radius,
        cx + radius,
        cz + radius
    );
    
    // warna lubang
    glColor3f(0.15f, 0.15f, 0.15f);
    glBegin(GL_TRIANGLE_FAN);

    // titik tengah
    glVertex3f(cx, 0.01f, cz);

    // lingkaran
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        float x = cx + cos(angle) * radius;
        float z = cz + sin(angle) * radius;
        glVertex3f(x, 0.01f, z);
    }
    glEnd();
}
