#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "cammove.h"
#include "geometri.h"

// ======= Variables =======
float angle = 0.0;
float deltaAngle = 0.0, ratio;
float x = 0.0f, y = 1.75f, z = 15.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
int h, w;
int moveFB = 0;
int moveLR = 0;

bool topView = false;
int lastTime = 0;
float deltaTime = 0.0f;

int sprint = 0;
float velocityY = 0.0f;
int isJumping = 0;
const float gravity = 15.0f;
const float jumpPower = 5.0f;
const float groundY = 1.75f;

// ======= Rotasi =======
void orientMe(float ang)
{
    lx = sin(ang);
    lz = -cos(ang);
}

// ======= Set view matrix =======
void applyCamera()
{
    if (topView)
    {
        gluLookAt(
            x, 40.0f, z,   
            x, 0.0f, z,
            0.0f, 0.0f, -1.0f
        );
    }
    else
    {
        gluLookAt(
            x, y, z,
            x + lx, y + ly, z + lz,
            0.0f, 1.0f, 0.0f
        );
    }
}

// ======= Maju / mundur =======
void moveMeFlat(int i)
{
    float baseSpeed = sprint ? 8.0f : 5.0f;
    float speed = baseSpeed * deltaTime;

    float nextX = x + i * lx * speed;
    float nextZ = z + i * lz * speed;

    if (!checkCollision(nextX, z)) x = nextX;
    if (!checkCollision(x, nextZ)) z = nextZ;
}

// ======= Strafe =======
void strafeMe(int i)
{
    float baseSpeed = sprint ? 7.0f : 4.0f;
    float speed = baseSpeed * deltaTime;

    float nextX = x + i * lz * speed;
    float nextZ = z - i * lx * speed;

    if (!checkCollision(nextX, z)) x = nextX;
    if (!checkCollision(x, nextZ)) z = nextZ;
}

// ======= Reshape =======
void Reshape(int w1, int h1)
{
    if (h1 == 0) h1 = 1;

    w = w1;
    h = h1;
    ratio = 1.0f * w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.1, 1000);

    glMatrixMode(GL_MODELVIEW);
}

// ======= Dipanggil tiap frame =======
void updateCamera()
{
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    angle += deltaAngle * deltaTime;
    orientMe(angle);

    if (moveFB) moveMeFlat(moveFB);
    if (moveLR) strafeMe(moveLR);

    // jump physics
    if (isJumping)
    {
        velocityY -= gravity * deltaTime;
        y += velocityY * deltaTime;

        if (y <= groundY)
        {
            y = groundY;
            velocityY = 0;
            isJumping = 0;
        }
    }
}

// ======= Input keyboard =======
void pressKey(int key, int xx, int yy)
{
    switch (key)
    {
        case GLUT_KEY_LEFT: deltaAngle = -2.0f; break;
        case GLUT_KEY_RIGHT: deltaAngle = 2.0f; break;
    }
}

void releaseKey(int key, int xx, int yy)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
    }
}

void keyboard(unsigned char key, int xx, int yy)
{
    switch (key)
    {
        case 'w': case 'W': moveFB =  2; break;
        case 's': case 'S': moveFB = -2; break;
        case 'a': case 'A': moveLR =  2; break;
        case 'd': case 'D': moveLR = -2; break;
        
        case ' ':
            if (!isJumping)
            {
                isJumping = 1;
                velocityY = jumpPower;
            }
            break;
            
        case 't': case 'T': topView = !topView; break;
        case 27: exit(0); break; // ESC untuk keluar
    }
}

void keyboardUp(unsigned char key, int xx, int yy)
{
    switch (key)
    {
        case 'w': case 'W':
        case 's': case 'S': moveFB = 0; break;
        case 'a': case 'A':
        case 'd': case 'D': moveLR = 0; break;
    }
}

void keyboardSpecial()
{
    int mod = glutGetModifiers();

    if (mod & GLUT_ACTIVE_SHIFT)
        sprint = 1;
    else
        sprint = 0;
}

