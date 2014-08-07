#include "gfx/Texture.hpp"
#include "gfx/drawingOperations.hpp"
#include "level/Level.hpp"
#include "globalResources.hpp"
#include "Screen.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <stdexcept>
#include <iostream>

int main() {
	using namespace Screen;
    Level kek_lvl("kek.lvl");
    Level level = kek_lvl;
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window =
        SDL_CreateWindow("Zordzman v0.0.1", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                         SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    int const imgflags = IMG_INIT_PNG;

    if (IMG_Init(imgflags) != imgflags) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
        return 1;
    }

    initGL(800, 600);

    globalResources::init();

    bool quit = false;
    const Uint8 *keys;

    while (!quit) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        float speed = 3;
        
        keys = SDL_GetKeyboardState(nullptr);
        
        if (keys[SDL_SCANCODE_LEFT]) {
            push(-speed, 0);
        } else if (keys[SDL_SCANCODE_RIGHT]) {
        	push(speed, 0);
        } else if (keys[SDL_SCANCODE_UP]) {
        	push(0, -speed);
        } else if (keys[SDL_SCANCODE_DOWN]) {
        	push(0, speed);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_QUADS);
        	level.render();
    		glColor4f(0.2f, 0.2f, 0.2f, 0.6f);
        	drawingOperations::drawRectangle(get_xOffset(), get_yOffset(),
        		WIDTH, 32);
        	glColor3f(0.5, 0.5, 0.5);
        	drawingOperations::drawText("HP: 23", get_xOffset(), get_yOffset(),
        		16, 16);
        	drawingOperations::drawText("WEP:", get_xOffset(),
        		get_yOffset()+16, 16, 16);
        	glColor3f(0, 1, 0);
        	drawingOperations::drawText("Zord", get_xOffset()+64,
        		get_yOffset()+16, 8, 8);
        	glColor3f(0.6, 0.6, 0.6);
        	drawingOperations::drawText("Chicken", get_xOffset()+64,
        		get_yOffset()+24, 8, 8);
        	glColor3f(1, 1, 1);
        glEnd();
        
        glBegin(GL_LINES);
        	glColor3f(1, 1, 1);
       		drawingOperations::drawLine(get_xOffset(), get_yOffset()+32,
       			WIDTH, 32);
        glEnd();
        
        SDL_GL_SwapWindow(window);
    }

    globalResources::free();
    IMG_Quit();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
