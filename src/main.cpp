#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "Hello zordzman++ world!");

    auto texture = IMG_LoadTexture(renderer, "resources/lel.png");
    if (!texture) {
        printf("Whoopsie, here we go..\n");
        printf("Failed to load media!\n%s\n", IMG_GetError());
        return 1;
    }

    bool quit = false;

    while (!quit) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_RenderClear(renderer);
        /* Do I put muh rendering here? */
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        /* ^ */
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
