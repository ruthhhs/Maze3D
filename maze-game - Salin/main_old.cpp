#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

// ================== CAMERA ==================
float angle = 0.0, deltaAngle = 0.0, ratio;
float x = 0.0f, y = 1.75f, z = 15.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;

int h, w;

// ================== MOVEMENT ==================
int moveFB = 0; // forward/back
int moveLR = 0; // strafe

// ================== COLLISION ==================
typedef struct {
    float minX, maxX;
    float minZ, maxZ;
} Box;

Box walls[50];
int wallCount = 0;

float radius = 0.5f;

// ================== TAMBAH WALL ==================
void addWall(float px, float pz, float panjang, float lebar)
{
    float halfX = panjang / 2.0f;
    float halfZ = lebar / 2.0f;

    walls[wallCount].minX = px - halfX;
    walls[wallCount].maxX = px + halfX;
    walls[wallCount].minZ = pz - halfZ;
    walls[wallCount].maxZ = pz + halfZ;

    wallCount++;
}

// ================== CEK COLLISION ==================
int checkCollision(float nextX, float nextZ)
{
    for (int i = 0; i < wallCount; i++)
    {
        if (nextX + radius > walls[i].minX &&
            nextX - radius < walls[i].maxX &&
            nextZ + radius > walls[i].minZ &&
            nextZ - radius < walls[i].maxZ)
        {
            return 1;
        }
    }
    return 0;
}

// ================== GERAK MAJU ==================
void moveMeFlat(int i)
{
    float speed = 0.02f;

    float nextX = x + i * lx * speed;
    float nextZ = z + i * lz * speed;

    if (!checkCollision(nextX, z)) x = nextX;
    if (!checkCollision(x, nextZ)) z = nextZ;
}

// ================== STRAFE ==================
void strafeMe(int i)
{
    float speed = 0.01f;

    float nextX = x + i * lz * speed;
    float nextZ = z - i * lx * speed;

    if (!checkCollision(nextX, z)) x = nextX;
    if (!checkCollision(x, nextZ)) z = nextZ;
}

// ================== ROTASI ==================
void orientMe(float ang)
{
    lx = sin(ang);
    lz = -cos(ang);
}

// ================== RESHAPE ==================
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

// ================== GRID ==================
void Grid()
{
    glColor3f(0.5, 0.5, 0.5);

    glBegin(GL_LINES);
    for (float i = -50; i <= 50; i += 1.5)
    {
        glVertex3f(i, 0, -50);
        glVertex3f(i, 0, 50);

        glVertex3f(-50, 0, i);
        glVertex3f(50, 0, i);
    }
    glEnd();
}

// ================== BALOK ==================
void Balok(float px, float py, float pz, float panjang, float tinggi, float lebar)
{
    float x = panjang / 2.0f;
    float y = tinggi / 2.0f;
    float z = lebar / 2.0f;

    glPushMatrix();
    glTranslatef(px, py, pz);

    glBegin(GL_QUADS);

    // DEPAN
    glNormal3f(0, 0, 1);
    glVertex3f(-x, -y,  z);
    glVertex3f( x, -y,  z);
    glVertex3f( x,  y,  z);
    glVertex3f(-x,  y,  z);

    // BELAKANG
    glNormal3f(0, 0, -1);
    glVertex3f(-x, -y, -z);
    glVertex3f(-x,  y, -z);
    glVertex3f( x,  y, -z);
    glVertex3f( x, -y, -z);

    // ATAS
    glNormal3f(0, 1, 0);
    glVertex3f(-x,  y, -z);
    glVertex3f(-x,  y,  z);
    glVertex3f( x,  y,  z);
    glVertex3f( x,  y, -z);

    // BAWAH
    glNormal3f(0, -1, 0);
    glVertex3f(-x, -y, -z);
    glVertex3f( x, -y, -z);
    glVertex3f( x, -y,  z);
    glVertex3f(-x, -y,  z);

    // KANAN
    glNormal3f(1, 0, 0);
    glVertex3f( x, -y, -z);
    glVertex3f( x,  y, -z);
    glVertex3f( x,  y,  z);
    glVertex3f( x, -y,  z);

    // KIRI
    glNormal3f(-1, 0, 0);
    glVertex3f(-x, -y, -z);
    glVertex3f(-x, -y,  z);
    glVertex3f(-x,  y,  z);
    glVertex3f(-x,  y, -z);

    glEnd();
    glPopMatrix();
}

// ================== DISPLAY ==================
void display()
{
    if (deltaAngle)
    {
        angle += deltaAngle;
        orientMe(angle);
    }

    if (moveFB) moveMeFlat(moveFB);
    if (moveLR) strafeMe(moveLR);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(x, y, z,
              x + lx, y + ly, z + lz,
              0.0f, 1.0f, 0.0f);
    
    glDisable(GL_LIGHTING);
	Grid();
	glEnable(GL_LIGHTING);

    // RESET COLLISION
    wallCount = 0;

    // ===== LORONG =====
    Balok(0, 2, -5, 2, 4, 22);
    addWall(0, -5, 2, 22);

    Balok(5, 2, -5, 2, 4, 15);
    addWall(5, -5, 2, 15);

    Balok(12, 2, -12, 15, 4, 2);
    addWall(12, -12, 15, 2);

    Balok(7, 2, -17, 15, 4, 2);
    addWall(8, -17, 15, 2);

    glutSwapBuffers();
}

// ================== INPUT PANAH ==================
void pressKey(int key, int xx, int yy)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:  deltaAngle = -0.003f; break;
        case GLUT_KEY_RIGHT: deltaAngle =  0.003f; break;
    }
}

void releaseKey(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            deltaAngle = 0.0f;
            break;
    }
}

// ================== INPUT WASD ==================
void keyboard(unsigned char key, int xx, int yy)
{
    switch (key)
    {
        case 'w': moveFB = 1; break;
        case 's': moveFB = -1; break;
        case 'a': moveLR = 1; break;
        case 'd': moveLR = -1; break;
        case 'W': moveFB = 1; break;
        case 'S': moveFB = -1; break;
        case 'A': moveLR = 1; break;
        case 'D': moveLR = -1; break;
    }
}

void keyboardUp(unsigned char key, int xx, int yy)
{
    switch (key)
    {
        case 'w':
        case 's': moveFB = 0; break;

        case 'a':
        case 'd': moveLR = 0; break;
    }
}

//------------------------------------------------------------------
// Lighting
//------------------------------------------------------------------
const GLfloat light_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
const GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat light_position[] = {0.0f, 20.0f, 10.0f, 1.0f};

const GLfloat mat_ambient[]    = {0.7f, 0.7f, 0.7f, 1.0f};
const GLfloat mat_diffuse[]    = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat mat_specular[]   = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat high_shininess[] = {100.0f};

void lighting()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

// ================== INIT ==================
void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
}

// ================== MAIN ==================
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(800, 600);
    glutCreateWindow("3D FPS Mini (Collision)");

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);

    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);

	lighting();
    init();
    
    glutMainLoop();
    return 0;
}
