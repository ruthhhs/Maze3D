// ============================================================
// main.cpp
// Entry point game maze 3D — Dev-C++ + GLUT
//
// Struktur file:
//   main.cpp      — entry point, display loop, init
//   camera.cpp    — kamera, pergerakan, input keyboard
//   collision.cpp — AABB collision detection
//   balok.cpp     — BalokWarna() dan BalokTekstur(), loadBMP()
//   lighting.cpp  — setup GL_LIGHT0
//   maze.cpp      — layout maze (dinding + collision)
//
// Cara compile di Dev-C++ (MinGW):
//   Tambahkan semua .cpp ke project, link: -lopengl32 -lglu32 -lglut32
//   TIDAK perlu .h — semua #include antar .cpp langsung
// ============================================================

#include <GL/glut.h>

// Include modul-modul (tanpa .h, semua inline)
#include "lighting.cpp"
#include "collision.cpp"
#include "balok.cpp"
#include "camera.cpp"
#include "maze.cpp"

// ======= Tekstur global (dipakai di maze.cpp) =======
GLuint texWall    = 0;
GLuint texFloor   = 0;
GLuint texCeiling = 0;

// ======= Init =======
void init()
{
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    // Load tekstur — ganti nama file sesuai aset kamu.
    // Jika file tidak ada / NULL, balok tetap tampil (putih).
//    Image* imgWall    = loadBMP("wall.bmp");
//    Image* imgFloor   = loadBMP("floor.bmp");
//    Image* imgCeiling = loadBMP("ceiling.bmp");

//    texWall    = loadTexture(imgWall);
//    texFloor   = loadTexture(imgFloor);
//    texCeiling = loadTexture(imgCeiling);

//    delete imgWall;
//    delete imgFloor;
//    delete imgCeiling;
}

// ======= Display callback =======
void display()
{
    // Update posisi & arah kamera
    updateCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Terapkan view matrix
    applyCamera();

    // Grid lantai (tanpa lighting agar tetap terlihat)
    disableLighting();
    Grid();
    enableLighting();

    // Reset collision, lalu gambar maze
    resetWalls();
    drawMaze();

    glutSwapBuffers();
}

// ======= Main =======
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 600);
    glutCreateWindow("Maze 3D");

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(Reshape);

    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);

    initLighting();
    init();

    glutMainLoop();
    return 0;
}
