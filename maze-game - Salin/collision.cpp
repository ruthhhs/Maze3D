// ============================================================
// collision.cpp
// Sistem AABB collision detection untuk dinding maze
// ============================================================

// ======= Struktur AABB =======
typedef struct {
    float minX, maxX;
    float minZ, maxZ;
} Box;

#define MAX_WALLS 200
Box walls[MAX_WALLS];
int  wallCount = 0;

float playerRadius = 0.4f;

// ======= Reset semua dinding (dipanggil tiap frame sebelum rebuild maze) =======
void resetWalls()
{
    wallCount = 0;
}

// ======= Tambah dinding berdasarkan titik awal dan akhir (sama dengan parameter Balok) =======
// x1,y1,z1 = titik minimum; x2,y2,z2 = titik maksimum
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
