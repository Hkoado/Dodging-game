#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "player.h"
#include "constants.h"

bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(std::string path);

// The window we'll be rendering to
SDL_Window* gWindow = nullptr;

// The window renderer
SDL_Renderer* gRenderer = nullptr;

// The player
Player* gPlayer = nullptr;

// The background texture
SDL_Texture* gBackgroundTexture = nullptr;

// Background positions
float bgX1 = 0;
float bgX2 = SCREEN_WIDTH;

bool init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else {
        // Create window
        gWindow = SDL_CreateWindow("Dodging game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
                std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Create the player
    gPlayer = new Player(gRenderer);
    if (gPlayer == nullptr) {
        std::cerr << "Failed to create player!" << std::endl;
        success = false;
    }

    // Load background texture
    gBackgroundTexture = loadTexture("background.png");
    if (gBackgroundTexture == nullptr) {
        std::cerr << "Failed to load background texture!" << std::endl;
        success = false;
    }

    return success;
}

SDL_Texture* loadTexture(std::string path) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void close() {
    // Free loaded images
    SDL_DestroyTexture(gBackgroundTexture);
    gBackgroundTexture = nullptr;

    delete gPlayer;
    gPlayer = nullptr;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void updateBackground() {
    // Move the background
    bgX1 -= 0.7;
    bgX2 -= 0.7;

    // Reset background positions if they go off screen
    if (bgX1 <= -SCREEN_WIDTH) {
        bgX1 = SCREEN_WIDTH;
    }
    if (bgX2 <= -SCREEN_WIDTH) {
        bgX2 = SCREEN_WIDTH;
    }
}

void renderBackground() {
    // Render the background
    SDL_Rect bgRect1 = { bgX1, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect bgRect2 = { bgX2, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, &bgRect1);
    SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, &bgRect2);
}

int main(int argc, char* args[]) {
    // Start up SDL and create window
    if (!init()) {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else {
        // Load media
        if (!loadMedia()) {
            std::cerr << "Failed to load media!" << std::endl;
        }
        else {
            // Main loop flag
            bool quit = false;

            // Event handler
            SDL_Event e;

            // While application is running
            while (!quit) {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0) {
                    // User requests quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }

                    // Handle input for the player
                    gPlayer->handleEvent(e);
                }

                // Move the player
                gPlayer->move();

                // Update the background
                updateBackground();

                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render background
                renderBackground();

                // Render player
                gPlayer->render();

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}