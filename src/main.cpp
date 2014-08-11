#include "gfx/Texture.hpp"
#include "gfx/drawingOperations.hpp"
#include "level/Level.hpp"
#include "globalResources.hpp"
#include "Screen.hpp"
#include "entity/Player.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <stdexcept>
#include <iostream>

int main() {
    using namespace Screen;
    using namespace drawingOperations;

    Level level;
    {
        Level kek_lvl("kek.lvl");
        kek_lvl.add(new Player(300, 300, 1.5));
        level = kek_lvl;
    }

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        "Zordzman v0.0.1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
        return 1;
    }

    initGL(800, 600);

    globalResources::init();

    for (;;) {
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            continue;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        level.render();

        glColor3f(0.2, 0.2, 0.2);
        drawRectangle(get_xOffset(), get_yOffset() + HEIGHT - 32, WIDTH, 32,
                      true);
        glColor3f(0.7, 0.7, 0.7);
        drawText("HP: 23", get_xOffset(), get_yOffset() + HEIGHT - 32, 16, 16);
        drawText("WEP:", get_xOffset(), get_yOffset() + HEIGHT - 32 + 16, 16,
                 16);
        glColor3f(0, 1, 0);
        drawText("Zord", get_xOffset() + 64, get_yOffset() + HEIGHT - 32 + 16,
                 8, 8);
        glColor3f(0.6, 0.6, 0.6);
        drawText("Chicken", get_xOffset() + 64,
                 get_yOffset() + HEIGHT - 32 + 24, 8, 8);

        glColor3f(0, 0, 0);
        drawLine(get_xOffset(), get_yOffset() + HEIGHT - 32,
                 get_xOffset() + WIDTH, get_yOffset() + HEIGHT - 32);
        drawLine(get_xOffset(), get_yOffset() + HEIGHT - 33,
                 get_xOffset() + WIDTH, get_yOffset() + HEIGHT - 33);

        glColor3f(1, 1, 1);

        SDL_GL_SwapWindow(window);
    }

    globalResources::free();
    IMG_Quit();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
