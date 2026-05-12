// ============================================================
// camera.cpp
// Mengelola posisi kamera, arah pandang, rotasi, dan pergerakan
// ============================================================

#include <math.h>
#include <GL/glut.h>
#include "collision.cpp"  // butuh checkCollision

// ======= State kamera =======
float camAngle    = 0.0f;
float deltaAngle  = 0.0f;
float camX        = 0.0f;
float camY        = 1.75f;
float camZ        = 15.0f;
float lookX       = 0.0f;
float lookY       = 0.0f;
float lookZ       = -1.0f;

int moveFB = 0; // forward / backward
int moveLR = 0; // strafe kiri / kanan

// ======= Update arah pandang =======
void orientMe(float ang)
{
    lookX = sin(ang);
    lookZ = -cos(ang);
}

// ======= Maju / mundur =======
void moveMeFlat(int dir)
{
    float speed = 0.15f;
    float nextX = camX + dir * lookX * speed;
    float nextZ = camZ + dir * lookZ * speed;

    if (!checkCollision(nextX, camZ)) camX = nextX;
    if (!checkCollision(camX, nextZ)) camZ = nextZ;
}

// ======= Strafe =======
void strafeMe(int dir)
{
    float speed = 0.10f;
    float nextX = camX + dir * lookZ * speed;
    float nextZ = camZ - dir * lookX * speed;

    if (!checkCollision(nextX, camZ)) camX = nextX;
    if (!checkCollision(camX, nextZ)) camZ = nextZ;
}

// ======= Dipanggil tiap frame =======
void updateCamera()
{
    if (deltaAngle)
    {
        camAngle += deltaAngle;
        orientMe(camAngle);
    }
    if (moveFB) moveMeFlat(moveFB);
    if (moveLR) strafeMe(moveLR);
}

// ======= Set view matrix =======
void applyCamera()
{
    gluLookAt(
        camX, camY, camZ,
        camX + lookX, camY + lookY, camZ + lookZ,
        0.0f, 1.0f, 0.0f
    );
}

// ======= Reshape / proyeksi =======
void Reshape(int w, int h)
{
    if (h == 0) h = 1;
    float ratio = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

// ======= Input keyboard =======
void pressKey(int key, int xx, int yy)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:  deltaAngle = -0.03f; break;
        case GLUT_KEY_RIGHT: deltaAngle =  0.03f; break;
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
        case 'w': case 'W': moveFB =  1; break;
        case 's': case 'S': moveFB = -1; break;
        case 'a': case 'A': moveLR =  1; break;
        case 'd': case 'D': moveLR = -1; break;
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
