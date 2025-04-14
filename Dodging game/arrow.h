#ifndef ARROW_H
#define ARROW_H

#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include "constants.h"

class Arrow {
public:
    Arrow(SDL_Renderer* renderer, bool falling = false);
    ~Arrow();
    void move();
    void render();
    SDL_Rect getCollider();

private:
    SDL_Renderer* gRenderer;
    SDL_Texture* gArrowTexture;
    SDL_Rect mCollider;
    int speed;
    bool isFalling;
    double rotationAngle;
    static const int ARROW_WIDTH = 80; 
    static const int ARROW_HEIGHT = 15; 
};

#endif