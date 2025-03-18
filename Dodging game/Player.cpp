#include "player.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

// Initialize static member
const float Player::PLAYER_VEL = 1.0f;

Player::Player(SDL_Renderer* renderer)
    : mPosX(0), mPosY(GROUND_LEVEL), mVelX(0), mVelY(0), mJumpSpeed(7.0f), mGravity(0.1f), mMovingToTarget(false), mRenderer(renderer) {
    mTexture = loadTexture("player.png");
    if (mTexture == nullptr) {
        std::cerr << "Failed to load player texture!" << std::endl;
    }
}

SDL_Texture* Player::loadTexture(std::string path) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
        case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
        case SDLK_SPACE: if (mPosY == GROUND_LEVEL) mVelY = -mJumpSpeed; break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        switch (e.key.keysym.sym) {
        case SDLK_LEFT: mVelX += PLAYER_VEL; break;
        case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
        }
    }
}

void Player::move() {
    mPosX += static_cast<int>(mVelX);
    mPosY += static_cast<int>(mVelY);

    if (mPosX < 0) {
        mPosX = 0;
    }
    else if (mPosX + PLAYER_WIDTH > SCREEN_WIDTH) {
        mPosX = SCREEN_WIDTH - PLAYER_WIDTH;
    }

    if (mPosY < 0) {
        mPosY = 0;
    }
    else if (mPosY > GROUND_LEVEL) {
        mPosY = GROUND_LEVEL;
        mVelY = 0;
    }

    if (mPosY < GROUND_LEVEL) {
        applyGravity();
    }
}

void Player::applyGravity() {
    mVelY += mGravity;
}

void Player::render() {
    SDL_Rect renderQuad = { mPosX, mPosY, PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderQuad);
}