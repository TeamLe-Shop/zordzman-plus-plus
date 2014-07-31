#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>

int main() {
	srand(time(0));

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
	SDL_Rect img_rect;
    while (!quit) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        img_rect = {100, 100, 600, 400};
        
        img_rect.x += (rand() % 12) - 6;
        img_rect.y += (rand() % 12) - 6;
                
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &img_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
