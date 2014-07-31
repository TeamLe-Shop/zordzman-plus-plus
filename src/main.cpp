#include "Texture.hpp"
#include "Sprite.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

int main() {
    srand(time(0));

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window =
        SDL_CreateWindow("Hello zordzman++ world!", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    Texture texture;
    texture.loadFromFile("resources/lel.png");
    Sprite sprite(texture);

    bool quit = false;
    SDL_Rect img_rect;
    while (!quit) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        img_rect = { 100, 100, 600, 400 };

        img_rect.x += (rand() % 12) - 6;
        img_rect.y += (rand() % 12) - 6;

        glClear(GL_COLOR_BUFFER_BIT);
        sprite.setPosition(img_rect.x, img_rect.y);
        sprite.draw();
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
