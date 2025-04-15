#include "arrow.h"
#include <SDL_image.h>
#include <iostream>

Arrow::Arrow(SDL_Renderer* renderer, bool falling) : gRenderer(renderer), speed(1.0f), isFalling(falling), rotationAngle(0.0) {

    SDL_Surface* loadedSurface = IMG_Load("arrow.png");
    gArrowTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (isFalling) {
        mCollider = { rand() % (SCREEN_WIDTH - ARROW_WIDTH), 0, ARROW_WIDTH, ARROW_HEIGHT };
        rotationAngle = -90.0; 
    }
    else {
        int maxY = SCREEN_HEIGHT - ARROW_HEIGHT;
        int minY = SCREEN_HEIGHT - MAX_JUMP_HEIGHT - ARROW_HEIGHT;
        mCollider = { SCREEN_WIDTH, rand() % (maxY - minY) + minY, ARROW_WIDTH, ARROW_HEIGHT };
        rotationAngle = 0.0;
    }
}

Arrow::~Arrow() {
    SDL_DestroyTexture(gArrowTexture);
}

void Arrow::move() {
    if (isFalling) {
        mCollider.y += speed;
    }
    else {
        mCollider.x -= speed;
    }
}

void Arrow::render() {
    SDL_Point center = { mCollider.w / 2, mCollider.h / 2 }; 
    SDL_RenderCopyEx(gRenderer, gArrowTexture, nullptr, &mCollider, rotationAngle, &center, SDL_FLIP_NONE);
}

SDL_Rect Arrow::getCollider() {
    return mCollider;
}