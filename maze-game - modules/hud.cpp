#include <math.h>
#include <GL/glut.h>
#include <stdio.h>

#include "hud.h"
#include "cammove.h"
#include "coltex.h"

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
	float angleDeg = angle * (180.0f / 3.14159265f);
    sprintf(buf, "X: %.2f  Y: %.2f  Z: %.2f", x, y, z);	renderText(10, h - 20, buf);
    sprintf(buf, "Angle: %.1f deg", angleDeg);			renderText(10, h - 40, buf);
    sprintf(buf, "View: %s", topView ? "TOP" : "FPS");	renderText(10, h - 60, buf);
    
    renderText(w/2, h/2, "+");
    
    renderText(10, 30, "WASD: Move | Arrow: Rotate | Space: Jump | Shift: Sprint | T: Toggle View");

	// For player
//    renderText(10, 30, "WASD: Move | Arrow: Rotate | Space: Jump | Shift: Sprint");
}

