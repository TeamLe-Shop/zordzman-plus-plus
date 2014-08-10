#include "Screen.hpp"
#include "SDL_opengl.h"

namespace Screen {

float xOffs = 0, yOffs = 0;

void initGL(int width, int height) {

    glEnable(GL_BLEND);				
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);	/* enabled second time in the same function... */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 	/* enabled second time in the same function... */

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D); /* enabled second time in the same function... */

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void push(float x, float y) {
    glTranslatef(-x, -y, 0);
    xOffs += x;
    yOffs += y;
}

float get_xOffset() { return xOffs; }

float get_yOffset() { return yOffs; }
}
