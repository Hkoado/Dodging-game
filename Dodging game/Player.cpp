#include "player.h"
#include <SDL_image.h>
#include <iostream>

// Initialize static member
const float Player::PLAYER_VEL = 1.0f;

Player::Player(SDL_Renderer* renderer)
    : mPosX(0), mPosY(GROUND_LEVEL), mVelX(0), mVelY(0),
    mJumpSpeed(-6.0f), mGravity(0.05f), mIsJumping(false),
    mRenderer(renderer), mTexture(nullptr), mJumpSound(nullptr) {

    mTexture = loadTexture("player.png");
    if (mTexture == nullptr) {
        std::cerr << "Failed to load player texture!" << std::endl;
    }

  
    mJumpSound = Mix_LoadWAV("jump_sound.wav");
    if (mJumpSound == nullptr) {
        std::cerr << "Failed to load jump sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }

  
    mCollider = { mPosX, mPosY, PLAYER_WIDTH, PLAYER_HEIGHT };
}

Player::~Player() {
   
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
    }

    
    if (mJumpSound != nullptr) {
        Mix_FreeChunk(mJumpSound);
        mJumpSound = nullptr;
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
        case SDLK_SPACE:
            if (!mIsJumping) {
                mVelY = mJumpSpeed;
                mIsJumping = true;

               
                if (mJumpSound != nullptr) {
                    Mix_PlayChannel(-1, mJumpSound, 0);  
                }
            }
            break;
        case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
        case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
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

    if (mIsJumping) {
        mVelY += mGravity;
    }

    if ((mPosX < 0) || (mPosX + PLAYER_WIDTH > SCREEN_WIDTH)) {
        mPosX -= static_cast<int>(mVelX);
    }

    if (mPosY >= GROUND_LEVEL) {
        mPosY = GROUND_LEVEL;
        mVelY = 0;
        mIsJumping = false;
    }

    mCollider.x = mPosX;
    mCollider.y = mPosY;
}

void Player::render() {
    SDL_Rect renderQuad = { mPosX, mPosY, PLAYER_WIDTH, PLAYER_HEIGHT };
    SDL_RenderCopy(mRenderer, mTexture, nullptr, &renderQuad);
}

SDL_Rect Player::getCollider() const {
    return mCollider;
}