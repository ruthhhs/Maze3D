#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "imageloader.h"

// ============================================================
// Camera
// ============================================================
float angle = 0.0, deltaAngle = 0.0, ratio;
float x = 0.0f, y = 1.75f, z = 15.0f;
float lx = 0.0f, ly = 0.0f, lz = -1.0f;
bool topView = false;
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
// Texture
// ============================================================
GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        image->width,
        image->height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        image->pixels
    );

    return textureId;
}

#define TEX_PATH "tex/"

// kamus texture
GLuint texWall;

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);

    // masukin img ke kamus texture
    Image* img;

	img = loadBMP(TEX_PATH "bush.bmp");
	texWall = loadTexture(img);
	printf("texWall = %d\n", texWall);
	delete img;
	
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
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

// ========================================================
// HUD
// ========================================================

// ================== TEXT ==================
void renderChar(char c)
{
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
}

void renderText(int screenX, int screenY, const char *text)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Nonaktifkan lighting & depth test supaya teks selalu tampil di atas
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glRasterPos2i(screenX, screenY);

    for (int i = 0; text[i] != '\0'; i++)
    {
        renderChar(text[i]);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// ================== DISPLAY HUD ==================
void drawHUD()
{
    char buf[128];

    // Warna teks: kuning muda
    glColor3f(0.8f, 0.8f, 0.4f);

	// For development
    sprintf(buf, "X: %.2f  Y: %.2f  Z: %.2f", x, y, z);
    renderText(10, h - 20, buf);

    float angleDeg = angle * (180.0f / 3.14159265f);
    sprintf(buf, "Angle: %.1f deg", angleDeg);
    renderText(10, h - 40, buf);

    sprintf(buf, "View: %s", topView ? "TOP" : "FPS");
    renderText(10, h - 60, buf);
    
    renderText(10, 30, "WASD: Move | Arrow: Rotate | Space: Jump | Shift: Sprint | T: Toggle View");

	// For player
//    renderText(10, 30, "WASD: Move | Arrow: Rotate | Space: Jump | Shift: Sprint");
}


// ================== MAZE ==================
void drawMaze()
{
    //  --- Lorong lurus ---
    WallTex(-1, 0, -20,   1, 4,  5, texWall); // kiri
    WallTex( 3, 0, -14,   5, 4,  5, texWall); // kanan

    // --- Lorong belokan ---
    WallTex( 3, 0, -14,  18, 4, -12, texWall); // kanan
    WallTex( -1, 0, -22,  18, 4, -20, texWall); // kiri

	drawHUD();
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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texWall);
	glDisable(GL_TEXTURE_2D);
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
    initRendering();
    init();
    
    lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();
    return 0;
}
