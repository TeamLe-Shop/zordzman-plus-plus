#include "Screen.hpp"
#include "SDL_opengl.h"

namespace Screen {

float xOffs = 0, yOffs = 0;

void push(float x, float y) {
    glTranslatef(-x, -y, 0);
    xOffs += x;
    yOffs += y;
}

float get_xOffset() { return xOffs; }

float get_yOffset() { return yOffs; }
}
