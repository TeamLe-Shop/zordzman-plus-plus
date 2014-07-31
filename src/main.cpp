#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

SDL_Surface* img;
SDL_Texture* img_tex;
const char* path = "resources/lel.png";

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer);
    SDL_SetWindowTitle(window, "Hello zordzman++ world!");

	img = IMG_Load ( path );
	if (!img)
	{
		printf("Whoopsie, here we go..\n");
		printf("Failed to load media!\n%s\n", IMG_GetError());
		return 1;
	}
	img_tex = SDL_CreateTextureFromSurface(renderer, img);

	SDL_Rect srcrect = {0, 0, 800, 600};
	SDL_Rect destrect = {0, 0, 800, 600};
	

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
        SDL_RenderCopy(renderer, img_tex, NULL, NULL);
        /* ^ */
        SDL_RenderPresent(renderer);
    }

	SDL_FreeSurface(img);
	SDL_DestroyTexture(img_tex);
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}