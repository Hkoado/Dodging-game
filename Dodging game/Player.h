#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include "constants.h"

class Player {
public:
    static const int PLAYER_WIDTH = 100;
    static const int PLAYER_HEIGHT = 100;
    static const float PLAYER_VEL;
    Player(SDL_Renderer* renderer);
    ~Player();
    void handleEvent(SDL_Event& e);
    void move();
    void render();

    SDL_Rect getCollider() const;



private:
    int mPosX, mPosY;
    float mVelX, mVelY;
    float mJumpSpeed;
    float mGravity;
    bool mIsJumping;
    SDL_Texture* mTexture;
    SDL_Texture* loadTexture(std::string path);
    SDL_Renderer* mRenderer;
    static const int GROUND_LEVEL = SCREEN_HEIGHT - PLAYER_HEIGHT - 100;
    SDL_Rect mCollider;
    Mix_Chunk* mJumpSound;
};

#endif
