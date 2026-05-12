// ============================================================
// maze.cpp
// Mendefinisikan layout maze: menggambar dinding dan
// mendaftarkan AABB collision-nya secara bersamaan.
//
// Setiap dinding didefinisikan dengan titik awal (x1,y1,z1)
// dan titik akhir (x2,y2,z2) — konsisten dengan Balok baru.
// ============================================================

#include <GL/glut.h>
#include "balok.cpp"
#include "collision.cpp"

// Tekstur dinding (diisi dari main setelah loadBMP)
extern GLuint texWall;
extern GLuint texFloor;
extern GLuint texCeiling;

// ======= Helper: gambar dinding + daftarkan collision =======
// Gambar balok bertekstur sekaligus tambah collision box-nya
static void Wall(
    float x1, float y1, float z1,
    float x2, float y2, float z2)
{
    BalokTekstur(x1, y1, z1, x2, y2, z2, texWall);
    addWallFromPoints(x1, z1, x2, z2);
}

// Versi warna (untuk lantai/langit-langit / debug)
static void WallColor(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float r, float g, float b)
{
    BalokWarna(x1, y1, z1, x2, y2, z2, r, g, b);
    addWallFromPoints(x1, z1, x2, z2);
}

// ======= Grid lantai (opsional, tanpa lighting) =======
void Grid()
{
    glColor3f(0.35f, 0.35f, 0.35f);
    glBegin(GL_LINES);
    for (float i = -50; i <= 50; i += 1.5f)
    {
        glVertex3f(i,  0, -50);  glVertex3f(i,  0, 50);
        glVertex3f(-50, 0,  i);  glVertex3f(50,  0, i);
    }
    glEnd();
}

// ======= Lantai =======
void drawFloor()
{
    BalokTekstur(-20, -0.1f, -30, 20, 0.0f, 10, texFloor);
}

// ======= Langit-langit =======
void drawCeiling()
{
    BalokTekstur(-20, 4.0f, -30, 20, 4.1f, 10, texCeiling);
}

// ============================================================
// drawMaze()
// Panggil di dalam display() setelah resetWalls().
// Koordinat: (x1, y1, z1) ? (x2, y2, z2)
// y1 = 0 (lantai), y2 = 4 (tinggi dinding)
// ============================================================
void drawMaze()
{
    // --- Lorong tengah vertikal (Z) ---
    //  Dinding kiri lorong utama
    Wall(-1, 0, -20,   1, 4,  8);   // kolom tengah kiri

    // Dinding kanan lorong utama
    Wall( 3, 0, -20,   5, 4,  2);

    // --- Lorong horizontal ---
    Wall( 3, 0, -14,  18, 4, -12);  // lorong atas kanan
    Wall( 3, 0, -22,  18, 4, -20);  // lorong bawah kanan

    // --- Dinding batas luar ---
    Wall(-8, 0, -24,  20, 4, -22);  // batas atas
    Wall(-8, 0,   8,  20, 4,  10);  // batas bawah
    Wall(-8, 0, -24,  -6, 4,  10);  // batas kiri
    Wall(18, 0, -24,  20, 4,  10);  // batas kanan

    // --- Pillar / tiang dekorasi ---
    WallColor( 6, 0, -5,   8, 4, -3,   0.6f, 0.3f, 0.1f);
    WallColor( 6, 0, -10,  8, 4, -8,   0.6f, 0.3f, 0.1f);
    WallColor(12, 0, -5,  14, 4, -3,   0.6f, 0.3f, 0.1f);
    WallColor(12, 0, -10, 14, 4, -8,   0.6f, 0.3f, 0.1f);

    // --- Lantai & langit-langit ---
    drawFloor();
    drawCeiling();
}
