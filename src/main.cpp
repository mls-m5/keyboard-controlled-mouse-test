#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

constexpr int numBoxes = 10;
constexpr int boxSize = 20;
constexpr int slowSpeed = 2;
constexpr int normalSpeed = 4 * slowSpeed;
constexpr int fastSpeed = 20 * slowSpeed;
constexpr float alpha = 0.3f; // Smoothing factor for IIR filter

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
    Coordinates boxes[numBoxes];
    for (int i = 0; i < numBoxes; i++) {
        boxes[i].x = rand() % (displayMode.w - boxSize);
        boxes[i].y = rand() % (displayMode.h - boxSize);
    }

    // Set up a timer to run at 60 Hz
    const int frameDelay = 1000 / 60;
    Uint32 frameStart;
    int frameTime;

    // Initialize the speed multiplier and target speed
    float speedMultiplier = normalSpeed;
    float targetSpeedMultiplier = normalSpeed;

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
        if (state[SDL_SCANCODE_LSHIFT]) {
            targetSpeedMultiplier = fastSpeed;
        }
        else if (state[SDL_SCANCODE_LCTRL]) {
            targetSpeedMultiplier = slowSpeed;
        }
        else {
            targetSpeedMultiplier = normalSpeed;
        }

        // Apply IIR filter to smooth the speed multiplier
        speedMultiplier =
            alpha * targetSpeedMultiplier + (1 - alpha) * speedMultiplier;

        // Update coordinates based on filtered speed multiplier
        if (state[SDL_SCANCODE_UP]) {
            coords.y -= static_cast<int>(speedMultiplier);
        }
        if (state[SDL_SCANCODE_DOWN]) {
            coords.y += static_cast<int>(speedMultiplier);
        }
        if (state[SDL_SCANCODE_LEFT]) {
            coords.x -= static_cast<int>(speedMultiplier);
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            coords.x += static_cast<int>(speedMultiplier);
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
        for (int i = 0; i < numBoxes; i++) {
            SDL_Rect box = {boxes[i].x, boxes[i].y, boxSize, boxSize};
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
