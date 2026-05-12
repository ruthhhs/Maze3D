#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

// ============================================================
// Camera
// ============================================================
float angle = 0.0, deltaAngle = 0.0, ratio;
float x = 0.0f, y = 1.75f, z = 15.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;

int lastTime = 0;
float deltaTime = 0.0f;

int h, w;

int moveFB = 0; // forward/back
int moveLR = 0; // strafe

// ======= Rotasi =======
void orientMe(float ang)
{
    lx = sin(ang);
    lz = -cos(ang);
}

// ======= Set view matrix =======
void applyCamera()
{
    gluLookAt(
        x, y, z,
        x + lx, y + ly, z + lz,
        0.0f, 1.0f, 0.0f
    );
}

// ============================================================
// Collosion
// ============================================================

typedef struct {
    float minX, maxX;
    float minZ, maxZ;
} Box;

#define MAX_WALLS 200
Box walls[MAX_WALLS];
int  wallCount = 0;

float playerRadius = 0.4f;

void resetWalls()
{
    wallCount = 0;
}

// ======= Tambah dinding berdasarkan titik awal dan akhir =======
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

// ======= Cek collision =======
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

// ============================================================
// Movement
// ============================================================

int sprint = 0;

float velocityY = 0.0f;
int isJumping = 0;

const float gravity = 9.8f;
const float jumpPower = 3.0f;
const float groundY = 1.75f;

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

// ============================================================
// Geometri
// ============================================================

// ================== WARNA ==================
typedef struct {
    float r, g, b;
} Color;

Color merah = {1.0f, 0.0f, 0.0f};
Color hijau = {0.0f, 1.0f, 0.0f};
Color biru  = {0.0f, 0.0f, 1.0f};
Color putih = {1.0f, 1.0f, 1.0f};
Color abu   = {0.5f, 0.5f, 0.5f};
Color kuning= {1.0f, 1.0f, 0.0f};

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
static void _drawBalokFaces(
    float x1, float y1, float z1,
    float x2, float y2, float z2)
{
    // Pastikan urutan min/max benar
    float minX = x1 < x2 ? x1 : x2;
    float maxX = x1 > x2 ? x1 : x2;
    float minY = y1 < y2 ? y1 : y2;
    float maxY = y1 > y2 ? y1 : y2;
    float minZ = z1 < z2 ? z1 : z2;
    float maxZ = z1 > z2 ? z1 : z2;

    glBegin(GL_QUADS);

    // DEPAN (+Z)
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(minX, minY, maxZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, maxZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, maxZ);

    // BELAKANG (-Z)
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, minZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, minZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, minZ);

    // ATAS (+Y)
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0); glVertex3f(minX, maxY, minZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, maxZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(1,0); glVertex3f(maxX, maxY, minZ);

    // BAWAH (-Y)
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, minZ);
    glTexCoord2f(1,1); glVertex3f(maxX, minY, maxZ);
    glTexCoord2f(0,1); glVertex3f(minX, minY, maxZ);

    // KANAN (+X)
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(maxX, minY, minZ);
    glTexCoord2f(0,1); glVertex3f(maxX, maxY, minZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, maxZ);

    // KIRI (-X)
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(1,0); glVertex3f(minX, minY, maxZ);
    glTexCoord2f(1,1); glVertex3f(minX, maxY, maxZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, minZ);

    glEnd();
}

// ================== BALOK WARNA ==================
void BalokWarna(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Color c)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(c.r, c.g, c.b);
    _drawBalokFaces(x1, y1, z1, x2, y2, z2);
}

// ================== DINDING ==================
static void Wall(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    Color c)
{
    BalokWarna(x1, y1, z1, x2, y2, z2, c);
    addWallFromPoints(x1, z1, x2, z2);
}

// ================== MAZE ==================
void drawMaze()
{
    //  --- Lorong lurus ---
    Wall(-1, 0, -20,   1, 4,  5, putih); // kiri
    Wall( 3, 0, -14,   5, 4,  5, putih); // kanan

    // --- Lorong belokan ---
    Wall( 3, 0, -14,  18, 4, -12, putih); // kanan
    Wall( -1, 0, -22,  18, 4, -20, putih); // kiri

    // --- Dinding batas luar ---
//    Wall(-8, 0, -24,  20, 4, -22, 1.0, 1.0, 1.0);
//    Wall(-8, 0,   8,  20, 4,  10, 1.0, 1.0, 1.0);
//    Wall(-8, 0, -24,  -6, 4,  10, 1.0, 1.0, 1.0);
//    Wall(18, 0, -24,  20, 4,  10, 1.0, 1.0, 1.0);
}

// ============================================================
// Lighting
// ============================================================
void initLighting()
{
    const GLfloat light_ambient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 80.0f };

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

// Matikan lighting sementara (misal untuk grid/HUD)
void disableLighting() { glDisable(GL_LIGHTING); }
void enableLighting()  { glEnable(GL_LIGHTING);  }


// ============================================================
// Display
// ============================================================
void display()
{
    keyboardSpecial();
	updateCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    applyCamera();

    disableLighting();
    Grid();
    enableLighting();

    resetWalls();
    drawMaze();

    glutSwapBuffers();
}

// ============================================================
// Init
// ============================================================
void init(void)
{
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
}

// ============================================================
// Main
// ============================================================
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 600);
    glutCreateWindow("Maze 3D");
    
    glutIgnoreKeyRepeat(1);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);

    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);

    initLighting();
    init();
    
    lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();
    return 0;
}
