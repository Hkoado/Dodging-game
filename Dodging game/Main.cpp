#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include "player.h"
#include "constants.h"
#include "arrow.h"
#include <vector>
#include <ctime>

void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, TTF_Font* font, SDL_Color color);

bool init();
bool loadMedia();
void close();
void showMenu();
bool isMuted = false; 
SDL_Texture* loadTexture(std::string path);
SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
Player* gPlayer = nullptr;
SDL_Texture* gBackgroundTexture = nullptr;
SDL_Texture* gStartScreenTexture = nullptr;
SDL_Texture* gGameOverScreenTexture = nullptr;

Mix_Music* gStartMusic = nullptr;
Mix_Music* gGameOverMusic = nullptr;
Mix_Music* gGameMusic = nullptr;

std::vector<Arrow*> arrows;
Uint32 lastArrowTime = 0;
Uint32 startTime = 0;
int score = 0;
int highScore = 0;
int arrowCount = 1;
int lastMilestone = 0;

TTF_Font* gFont = nullptr;
bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("Dodging game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
                std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
                    success = false;
                }

                if (TTF_Init() == -1) {
                    std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
                    success = false;
                }

                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    gGameMusic = Mix_LoadMUS("game_music.mp3");
    if (gGameMusic == nullptr) {
        std::cerr << "Failed to load game music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        success = false;
    } else{ Mix_VolumeMusic(64); 
    }

    gPlayer = new Player(gRenderer);
    if (gPlayer == nullptr) {
        std::cerr << "Failed to create player!" << std::endl;
        success = false;
    }

    gBackgroundTexture = loadTexture("background.png");
    if (gBackgroundTexture == nullptr) {
        std::cerr << "Failed to load background texture!" << std::endl;
        success = false;
    }

    gStartScreenTexture = loadTexture("start_screen.png");
    if (gStartScreenTexture == nullptr) {
        std::cerr << "Failed to load start screen texture!" << std::endl;
        success = false;
    }

    gGameOverScreenTexture = loadTexture("game_over_screen.png");
    if (gGameOverScreenTexture == nullptr) {
        std::cerr << "Failed to load game over screen texture!" << std::endl;
        success = false;
    }

    gStartMusic = Mix_LoadMUS("start_music.mp3");
    if (gStartMusic == nullptr) {
        std::cerr << "Failed to load start music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    gGameOverMusic = Mix_LoadMUS("game_over_music.mp3");
    if (gGameOverMusic == nullptr) {
        std::cerr << "Failed to load game over music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        success = false;
    }

    gFont = TTF_OpenFont("score.ttf", 28);
    if (gFont == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
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
    SDL_DestroyTexture(gBackgroundTexture);
    gBackgroundTexture = nullptr;

    SDL_DestroyTexture(gStartScreenTexture);
    gStartScreenTexture = nullptr;

    SDL_DestroyTexture(gGameOverScreenTexture);
    gGameOverScreenTexture = nullptr;

    Mix_FreeMusic(gStartMusic);
    gStartMusic = nullptr;

    Mix_FreeMusic(gGameOverMusic);
    gGameOverMusic = nullptr;

    delete gPlayer;
    gPlayer = nullptr;

    TTF_CloseFont(gFont);
    gFont = nullptr;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

void renderBackground() {
    SDL_Rect bgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(gRenderer, gBackgroundTexture, nullptr, &bgRect);
}


bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}


void gameOverScreen() {
    bool quit = false;
    SDL_Event e;

    if (score > highScore) {
        highScore = score;
    }

    Mix_PlayMusic(gGameOverMusic, -1);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_RETURN:
                    Mix_HaltMusic();
                    return;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_m: 
                    isMuted = !isMuted;
                    Mix_VolumeMusic(isMuted ? 0 : MIX_MAX_VOLUME);
                    Mix_Volume(-1, isMuted ? 0 : MIX_MAX_VOLUME);
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(gRenderer, gGameOverScreenTexture, nullptr, &renderQuad);
        SDL_Color textColor = { 204, 201, 194 };
        renderText(gRenderer, "   GAME OVER =((", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, gFont, textColor);
        renderText(gRenderer, "     Score: " + std::to_string(score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, gFont, textColor);
        renderText(gRenderer, "  High Score: " + std::to_string(highScore), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, gFont, textColor);
        renderText(gRenderer, "Press ENTER to restart or ESC to quit", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 50, gFont, textColor);
        renderText(gRenderer, "Press M to mute/unmute", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 100, gFont, textColor);
        SDL_RenderPresent(gRenderer);
    }

    close();
    exit(0);
}

void showMenu() {
    bool start = false;
    SDL_Event e;

    Mix_PlayMusic(gStartMusic, -1);

    while (!start) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                close();
                exit(0);
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_RETURN:
                    start = true;
                    break;
                case SDLK_ESCAPE:
                    close();
                    exit(0);
                case SDLK_m: 
                    isMuted = !isMuted;
                    Mix_VolumeMusic(isMuted ? 0 : MIX_MAX_VOLUME);
                    Mix_Volume(-1, isMuted ? 0 : MIX_MAX_VOLUME); 
                }
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(gRenderer, gStartScreenTexture, nullptr, &renderQuad);
        SDL_Color textColor = { 204, 201, 194 };
        renderText(gRenderer, "  Press ENTER to start", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, gFont, textColor);
        renderText(gRenderer, "    Press ESC to quit", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, gFont, textColor);
        renderText(gRenderer, "Press M to mute/unmute", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 50, gFont, textColor);
        SDL_RenderPresent(gRenderer);
    }
}

void resetGame() {
    arrowCount = 1;
    lastMilestone = 0;
    score = 0;
}int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    if (!loadMedia()) {
        std::cerr << "Failed to load media!" << std::endl;
        return -1;
    }
    showMenu();

    while (true) {
        bool quit = false;
        SDL_Event e;

       
        Mix_PlayMusic(gGameMusic, -1);

        srand(static_cast<unsigned int>(time(nullptr)));

        lastArrowTime = SDL_GetTicks();
        startTime = SDL_GetTicks();

        arrows.clear();

        delete gPlayer;
        gPlayer = new Player(gRenderer);

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                    close();
                    return 0;
                }
                gPlayer->handleEvent(e);
            }

            gPlayer->move();

            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastArrowTime >= (rand() % 1000 + 1000)) {
                for (int i = 0; i < arrowCount; ++i) {
                    bool falling = (rand() % 2 == 0);
                    arrows.push_back(new Arrow(gRenderer, falling));
                }
                lastArrowTime = currentTime;
            }

            SDL_Rect playerCollider = gPlayer->getCollider();
            for (auto& arrow : arrows) {
                if (checkCollision(playerCollider, arrow->getCollider())) {
                    std::cerr << "Player hit by an arrow!" << std::endl;

                    Mix_HaltMusic();
                    quit = true;
                }
            }
            score = (currentTime - startTime) / 200;
            if (score % 100 == 0 && score > lastMilestone) {
                arrowCount ++; 
                lastMilestone = score; 
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            renderBackground();
            gPlayer->render();

            for (auto& arrow : arrows) {
                arrow->render();
            }
            for (auto it = arrows.begin(); it != arrows.end(); ) {
                Arrow* arrow = *it;
                arrow->move();

                SDL_Rect collider = arrow->getCollider();
                if (collider.y > SCREEN_HEIGHT || collider.y + collider.h < 0 ||
                    collider.x > SCREEN_WIDTH || collider.x + collider.w < 0) {
                    delete arrow;
                    it = arrows.erase(it);
                }
                else {
                    ++it;
                }
            }
            SDL_Color textColor = { 0,0, 0, 255 };
            renderText(gRenderer, "Score: " + std::to_string(score), SCREEN_WIDTH - 150, 10, gFont, textColor);

            SDL_RenderPresent(gRenderer);
        }
        gameOverScreen();
        resetGame();
    }
    close();

    return 0;
}

void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, TTF_Font* font, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface != nullptr) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture != nullptr) {
            SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
    else {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
}


