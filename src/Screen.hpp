#pragma once

namespace Screen {

unsigned int const WIDTH = 800;
unsigned int const HEIGHT = 600;

/// @brief Initialize OpenGL for 2D
/// @param width The width of the screen
/// @param height The height of the screen

void initGL(int width, int height);

/// @brief Uses glTranslatef to "push the camera"

void push(float x, float y);

float get_xOffset();

float get_yOffset();
}
