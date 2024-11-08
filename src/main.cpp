#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
} Coordinates;

int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Get the display mode to retrieve the screen resolution
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        printf("SDL_GetCurrentDisplayMode Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create an SDL window that is fullscreen
    SDL_Window *window = SDL_CreateWindow("Fullscreen SDL2 Window",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          displayMode.w,
                                          displayMode.h,
                                          SDL_WINDOW_FULLSCREEN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Initialize coordinates
    Coordinates coords = {0, 0};

    // Set up a timer to run at 60 Hz
    const int frameDelay = 1000 / 60;
    Uint32 frameStart;
    int frameTime;

    // Main loop to keep the window open
    SDL_Event event;
    int running = 1;
    while (running) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    coords.y -= 1;
                    printf("Arrow Up Pressed: Coordinates (%d, %d)\n",
                           coords.x,
                           coords.y);
                    break;
                case SDLK_DOWN:
                    coords.y += 1;
                    printf("Arrow Down Pressed: Coordinates (%d, %d)\n",
                           coords.x,
                           coords.y);
                    break;
                case SDLK_LEFT:
                    coords.x -= 1;
                    printf("Arrow Left Pressed: Coordinates (%d, %d)\n",
                           coords.x,
                           coords.y);
                    break;
                case SDLK_RIGHT:
                    coords.x += 1;
                    printf("Arrow Right Pressed: Coordinates (%d, %d)\n",
                           coords.x,
                           coords.y);
                    break;
                default:
                    break;
                }
            }
        }

        // Get the current mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Update the mouse position based on coordinates
        mouseX += coords.x;
        mouseY += coords.y;
        SDL_WarpMouseInWindow(window, mouseX, mouseY);

        // Cap the frame rate to 60 Hz
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up and close the SDL window
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
