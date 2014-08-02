#include "gfx/Texture.hpp"
#include "gfx/drawingOperations.hpp"
#include "level/Level.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

namespace {

void initGL(int width, int height) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

}

int main() {
    srand(time(0));
    Level level("kek.lvl");

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window =
        SDL_CreateWindow("Zordzman v0.0.1", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    int const imgflags = IMG_INIT_PNG;

    if (IMG_Init(imgflags) != imgflags) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
        return 1;
    }

    initGL(800, 600);

    SpriteSheet sheet("resources/spritesheet.png");

    bool quit = false;

    while (!quit) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        using namespace drawingOperations;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_QUADS);
        for (int x = 0; x < 25; x++) {
            for (int y = 0; y < 19; y++) {
                drawSprite(sheet, level.tileAt(x, y), 0, x * 32, y * 32, 32, 32);
            }
        }

        drawRectangle(64, 64, 32, 32);

        glEnd();

        glBegin(GL_LINES);
        drawRectangle(32, 32, 32, 32, false);
        glEnd();
        SDL_GL_SwapWindow(window);
    }

    IMG_Quit();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
