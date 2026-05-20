#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "maze.h"
#include "hud.h"
#include "geometri.h"

void drawMaze()
{
    //  --- Lorong lurus ---
    WallTex(-1, 0, -20, 1, 4,  5, texWall); // kiri
    WallTex( 3, 0, -14,   5, 4,  5, texWall); // kanan

    // --- Lorong belokan ---
    WallTex( 3, 0, -14,  18, 4, -12, texWall); // kanan
    WallTex( -1, 0, -22,  18, 4, -20, texWall); // kiri

    // ================= POHON =================
    drawSmallTree(2, -15);
    drawTree(10, -10);
    
    // ================= HOLE =================
    drawHole(14, -17, 2.0f);
	drawHUD();
}
