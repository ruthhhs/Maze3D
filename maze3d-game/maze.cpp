#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "maze.h"

void drawMaze()
{
	WallTex(0, 0, 0,   2, 4,  -20, texWall); // 1
    WallTex( 0, 0, 0,   7.5, 4,  -2, texWall); // 2
    WallTex( 18, 0, 0,  20, 4, -20, texWall); // 3
    WallTex( 12.5, 0, 0,  20, 4, -2, texWall); // 4
    WallTex( 0, 0, -18,  20, 4, -20, texWall); // 5
    
    WallTex( 10, 0, -25,  20, 4, -27, texWall); //6
    WallTex( 10, 0, -25,  12, 4, -35, texWall); //7
    WallTex( 5, 0, -25,  -5, 4, -27, texWall); //8
    WallTex( -5, 0, -15, -7, 4, -27, texWall); //9
    WallTex( 3, 0, -40,  24, 4, -42, texWall); //10
    
    WallTex( 5, 0, -32, 3, 4, -42, texWall); //11
    WallTex( 22, 0, -42,  24, 4, -47, texWall); //12
    WallTex( 22, 0, -45, 39, 4, -47, texWall); //13
    WallTex( -2, 0, -32,  -16, 4, -34, texWall); //14
    WallTex( -14, 0, -32, -16, 4, -47, texWall); //15
    
    WallTex( -10, 0, -45,  -16, 4, -47, texWall); //16
	WallTex( -2, 0, -39,  -9, 4, -41, texWall); //17
    WallTex( -2, 0, -41, -4, 4, -47, texWall); //18
    WallTex( -2, 0, -45,  5, 4, -47, texWall); //19
    WallTex( 10, 0, -45,  17, 4, -47, texWall); //20
    
    WallTex( 10, 0, -47, 12, 4, -54, texWall); //21
    WallTex( 10, 0, -52,  -10, 4, -54, texWall); //22
    WallTex( -10, 0, -52,  -12, 4, -59, texWall); //23
    WallTex( 7, 0, -59, -43, 4, -61, texWall); //24
    WallTex( -28, 0, -52,  -30, 4, -59, texWall); //25
    
    WallTex( -21, 0, -45, -23, 4, -54, texWall); //26
    WallTex( -15, 0, -52,  -21, 4, -54, texWall); //27
    WallTex( -41, 0, -15,  -43, 4, -61, texWall); //28
    WallTex( -37, 0, -51,  -42, 4, -53, texWall); //29 
	WallTex( -35, 0, -47, -37, 4, -53, texWall); //30
	
    WallTex( -28, 0, -45,  -37, 4, -47, texWall); //31
    WallTex( -28, 0, -34, -30, 4, -45, texWall); //32
    WallTex( -21, 0, -38,  -30, 4, -40, texWall); //33
    WallTex( -36, 0, -34, -41, 4, -36, texWall); //34
    WallTex( -34, 0, -40,  -36, 4, -27, texWall); //35
    
    WallTex( -21, 0, -27, -34, 4, -29, texWall); //36
    WallTex( -36, 0, -15, -41, 4, -17, texWall); //37
//    WallTex( -36, 0, -34, -41, 4, -36, texWall); //38
    WallTex( -34, 0, 8,  -36, 4, -22, texWall); //39
    WallTex( -27, 0, -10, -34, 4, -12, texWall); //40
    
    WallTex( -27, 0, -17,  -29, 4, -22, texWall); //41
    WallTex( -21, 0, -20,  -27, 4, -22, texWall); //42
    WallTex( -21, 0, -1,   -23, 4, -20, texWall); //43
    WallTex( -12, 0, -1,  -21, 4, -3, texWall); //44
    WallTex( -12, 0, -8,  -14, 4, -27, texWall); //45
    
    WallTex( -5, 0, -8, -12, 4, -10, texWall); //46
    WallTex( -5, 0, -8,  -7, 4, 15, texWall); //47
	WallTex( -21, 0, 13,  -7, 4, 15, texWall); //48
    WallTex( -12, 0, 4,  -28, 4, 6, texWall); //49
	WallTex( -27, 0, -5,  -29, 4, 15, texWall); //50
	
	WallTex( -12, 0, 20,  -28, 4, 22, texWall); //51
	WallTex( -26, 0, 22,  -28, 4, 29, texWall); //52
	WallTex( -12, 0, 27,  -28, 4, 29, texWall); //53	
	WallTex( -5, 0, 20,  -7, 4, 29, texWall); //54
	WallTex( 7, 0, 27,  -5, 4, 29, texWall); //55
	
	WallTex( -24, 0, 34,  -26, 4, 43, texWall); //56
	WallTex( -17, 0, 43,  -31, 4, 45, texWall); //57
	WallTex( -34, 0, 15,  -36, 4, 21, texWall); //58
	WallTex( -34, 0, 13,  -41, 4, 15, texWall); //59
	WallTex( -41, 0, -5,  -43, 4, 50, texWall); //60
    
    WallTex( -34, 0, 26,  -41, 4, 28, texWall); //61
	WallTex( -34, 0, 28,  -36, 4, 34, texWall); //62
	WallTex( -31, 0, 32,  -36, 4, 34, texWall); //63	
	WallTex( -36, 0, 43,  -41, 4, 45, texWall); //64
    WallTex( -43, 0, 50,  14, 4, 52, texWall); //65
    
    WallTex( -10, 0, 43,  -12, 4, 50, texWall); //66
	WallTex( -10, 0, 43,  9, 4, 45, texWall); //67
	WallTex( 7, 0, 45,    9, 4, 47, texWall); //68	
    WallTex( 21, 0, 38,   23, 4, 52, texWall); //69
	WallTex( -21, 0, 36,  27, 4, 38, texWall); //70
	
	WallTex( 14, 0, 5,  16, 4, 36, texWall); //71
	WallTex( 9, 0, 5,   14, 4, 7, texWall); //72
    WallTex( 7, 0, 5,   9, 4, 22, texWall); //73
	WallTex( 2, 0, 20,  7, 4, 22, texWall); //74
	WallTex( 0, 0, 5,   2, 4, 22, texWall); //75
    
    WallTex( 23, 0, 50,  67, 4, 52, texWall); //76
	WallTex( 46, 0, 50,  48, 4, 42, texWall); //77
	WallTex( 46, 0, 40,  55, 4, 42, texWall); //78
	WallTex( 67, 0, 52,  69, 4, -7, texWall); //79
	WallTex( 60, 0, 32,  62, 4, 42, texWall); //80
	
	WallTex( 62, 0, 32,  67, 4, 34, texWall); //81
	WallTex( 67, 0, 25,  53, 4, 27, texWall); //82
	WallTex( 53, 0, 27,  55, 4, 33, texWall); //83
	WallTex( 55, 0, 33,  41, 4, 35, texWall); //84
	WallTex( 39, 0, 33,  41, 4, 43, texWall); //85
	
	WallTex( 28, 0, 41,  39, 4, 43, texWall); //86
	WallTex( 32, 0, 27,  34, 4, 43, texWall); //87
	WallTex( 21, 0, 25,  48, 4, 27, texWall); //88
	WallTex( 46, 0, 18,  48, 4, 25, texWall); //89
	WallTex( 48, 0, 18,  55, 4, 20, texWall); //90
	
	WallTex( 21, 0, 25,  23, 4, 5, texWall); //91
	WallTex( 21, 0, 10,	 34, 4, 12, texWall); //92
	WallTex( 32, 0, 5,   34, 4, 12, texWall); //93
	WallTex( 67, 0, 9,   62, 4, 7, texWall); //94
	WallTex( 60, 0, 20,  62, 4, -2, texWall); //95
	
	WallTex( 67, 0, -7,  53, 4, -5, texWall); //96
	WallTex( 53, 0, -5,  55, 4, 5, texWall); //9
	WallTex( 55, 0, 5,   39, 4, 7, texWall); //97
	WallTex( 39, 0, 17,  41, 4, -6, texWall); //98
	WallTex( 27, 0, 17,  41, 4, 19, texWall); //99
	WallTex( 34, 0, -6,  39, 4, -4, texWall); //100
	WallTex( 32, 0, -6,  34, 4, 0, texWall); //101
	
	WallTex( 46, 0, 0,	  48, 4, -13, texWall); //102
	WallTex( 30, 0, -11,  48, 4, -13, texWall); //103
	WallTex( 30, 0, -13,  32, 4, -27, texWall); //104
	WallTex( 32, 0, -25,  41, 4, -27, texWall); //105
	
	WallTex( 17, 0, -32,  48, 4, -34, texWall); //106
	WallTex( 46, 0, -18,  48, 4, -34, texWall); //107
	WallTex( 37, 0, -18,  46, 4, -20, texWall); //108
	WallTex( 30, 0, -39,  46, 4, -41, texWall); //109
	WallTex( 44, 0, -41,  46, 4, -53, texWall); //110
	
	WallTex( 44, 0, -51,  54, 4, -53, texWall); //111
	WallTex( 17, 0, -52,  19, 4, -61, texWall); //112
	WallTex( 14, 0, -59,  69, 4, -61, texWall); //113
	WallTex( 37, 0, -52,  39, 4, -59, texWall); //114
	WallTex( 24, 0, -52,  39, 4, -54, texWall); //115
	
	WallTex( 60, 0, -48,  62, 4, -59, texWall); //116
	WallTex( 67, 0, -12,  69, 4, -61, texWall); //117
	WallTex( 60, 0, -41,  69, 4, -43, texWall); //118
	WallTex( 60, 0, -33,  62, 4, -43, texWall); //119
	WallTex( 62, 0, -12,  67, 4, -14, texWall); //120
	
	WallTex( 60, 0, -19,  62, 4, -26, texWall); //121
	WallTex( 55, 0, -26,  62, 4, -28, texWall); //122
	WallTex( 55, 0, -26,  53, 4, -45, texWall); //123
	WallTex( 53, 0, -12,  55, 4, -20, texWall); //124
	WallTex( 53, 0, -12,  64, 4, -14, texWall); //125
	
    // ================= POHON =================
    drawSmallTree2(25, 1);
    drawSmallTree(27, 1);
    drawTree(27.0, -9);
    drawTree(10, -10);
    
    // ================= HOLE =================
    drawHole(18, 10, 2.0f);
	drawHUD();
}
