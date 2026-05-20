#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "imageloader.h"
#include "geometri.h"
#include "hud.h"
#include "cammove.h"
#include "coltex.h"
#include "maze.h"
#include "tree.h"

// ============================================================
// Lighting
// ============================================================
void initLighting()
{
    const GLfloat light_ambient[]  = { 0.08f, 0.08f, 0.10f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 0.95f, 0.85f, 1.0f };
    const GLfloat light_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat light_position[] = { 0.0f, 20.0f, 10.0f, 1.0f };

    const GLfloat mat_ambient[]    = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 0.9f, 0.9f, 0.9f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

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
    glMatrixMode(GL_MODELVIEW);
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
