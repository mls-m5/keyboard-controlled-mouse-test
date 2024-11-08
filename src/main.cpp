#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr int NUM_BOXES = 10;
constexpr int BOX_SIZE = 20;
constexpr int SLOW_SPEED = 1;
constexpr int NORMAL_SPEED = 4 * SLOW_SPEED;
constexpr int FAST_SPEED = 40 * SLOW_SPEED;

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

    // Create an SDL renderer
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize coordinates
    Coordinates coords = {0, 0};

    // Initialize random seed
    srand(time(NULL));

    // Create random positions for boxes
    Coordinates boxes[NUM_BOXES];
    for (int i = 0; i < NUM_BOXES; i++) {
        boxes[i].x = rand() % (displayMode.w - BOX_SIZE);
        boxes[i].y = rand() % (displayMode.h - BOX_SIZE);
    }

    // Set up a timer to run at 60 Hz
    const int frameDelay = 1000 / 60;
    Uint32 frameStart;
    int frameTime;

    // Main loop to keep the window open
    int running = 1;
    while (running) {
        frameStart = SDL_GetTicks();

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Get the state of the keyboard
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        int speedMultiplier = NORMAL_SPEED;
        if (state[SDL_SCANCODE_LSHIFT]) {
            speedMultiplier = FAST_SPEED;
        }
        else if (state[SDL_SCANCODE_LCTRL]) {
            speedMultiplier = SLOW_SPEED;
        }

        if (state[SDL_SCANCODE_UP]) {
            coords.y -= speedMultiplier;
        }
        if (state[SDL_SCANCODE_DOWN]) {
            coords.y += speedMultiplier;
        }
        if (state[SDL_SCANCODE_LEFT]) {
            coords.x -= speedMultiplier;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            coords.x += speedMultiplier;
        }

        // Get the current mouse position
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Update the mouse position based on coordinates
        mouseX += coords.x;
        mouseY += coords.y;
        coords.x = 0;
        coords.y = 0;
        SDL_WarpMouseInWindow(window, mouseX, mouseY);

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the boxes
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (int i = 0; i < NUM_BOXES; i++) {
            SDL_Rect box = {boxes[i].x, boxes[i].y, BOX_SIZE, BOX_SIZE};
            SDL_RenderFillRect(renderer, &box);
        }

        // Present the renderer
        SDL_RenderPresent(renderer);

        // Cap the frame rate to 60 Hz
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up and close the SDL window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
