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

/* if compiling with Visual Studio */
#if defined(_MSC_VER )

/* if main is defined */
#ifdef main

/* undefine main */
#undef main
#endif
#endif



int main() {

	
    using namespace Screen;
    using namespace drawingOperations;
	/* why no spaces between two different blocks of code? */
    Level kek_lvl("kek.lvl");
    Level level = kek_lvl;

    Player *player = new Player(300, 300);
    level.add(player);

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

    const Uint8 *keys;

	bool is_runnning = true;

	/* replaced for(;;) with a while because : "use right tool for the job" ... */
    while (is_runnning) {

        SDL_Event event;

		/* added while because fuck you! */
        while(SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {

                is_runnning = false;
            }
        }

        float speed = 1;

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

		/* removed GL_DEPTH_BUFFER_BIT because depth test is disabled, thus it will always be cleared */
        glClear(GL_COLOR_BUFFER_BIT);

<<<<<<< HEAD
			level.render(); 
	
		/* just why? why every draw function could not have its own glBegin and glEnd? */
        glBegin(GL_QUADS);

		/* all this code bellow relies on the fact that the level.render() left right texture binded */

		/* if not, all text rendering will fail, because text rendering functions does not bind its own texture */

			glColor3f(0.7f, 0.7f, 0.7f);
			drawText("HP: 23", get_xOffset(), get_yOffset() + HEIGHT - 32, 16, 16);
			drawText("WEP:", get_xOffset(), get_yOffset() + HEIGHT - 32 + 16, 16, 16);

			glColor3f(0, 1, 0);
			drawText("Zord", get_xOffset() + 64, get_yOffset() + HEIGHT - 32 + 16, 8, 8);

			glColor3f(0.6, 0.6, 0.6);
			drawText("Chicken", get_xOffset() + 64, get_yOffset() + HEIGHT - 32 + 24, 8, 8);

		/* still why... */
		glEnd();
=======
        level.render();

        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
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
        glColor3f(1, 1, 1);
        glEnd();
>>>>>>> origin/master

		/* removed line rendering, pointles */

        SDL_GL_SwapWindow(window);
    }

    globalResources::free();
    IMG_Quit();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
