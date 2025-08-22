#include "../includes/framer.h"

int main() {
    x_eleven(true);
    init();

    SDL_Window* win = nullptr;
    createWindow("TextBox Demo", 640, 480, &win);

    TextBox box(100, 200, 440, 40);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            box.handleEvent(e);
        }

        SDL_SetRenderDrawColor(gRenderer, 50,50,50,255);
        SDL_RenderClear(gRenderer);

        box.draw();

        SDL_RenderPresent(gRenderer);
    }

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
