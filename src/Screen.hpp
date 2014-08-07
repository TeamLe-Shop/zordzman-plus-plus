#pragma once

namespace Screen {

const short int WIDTH = 800;
const short int HEIGHT = 600;

/// @brief Initialize OpenGL for 2D
/// @param width The width of the screen
/// @param height The height of the screen

void initGL(int width, int height);

/// @brief Uses glTranslatef to "push the camera"

void push(float x, float y);

float get_xOffset();

float get_yOffset();

}