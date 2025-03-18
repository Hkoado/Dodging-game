#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "constants.h"

class Player {
public:
    // The dimensions of the player
    static const int PLAYER_WIDTH = 150;
    static const int PLAYER_HEIGHT = 150;

    // Maximum axis velocity of the player
    static const float PLAYER_VEL;

    // Initializes the variables
    Player(SDL_Renderer* renderer);

    // Handles key press
    void handleEvent(SDL_Event& e);

    // Moves the player
    void move();

    // Shows the player on the screen
    void render();

private:
    // The X and Y offsets of the player
    int mPosX, mPosY;

    // The velocity of the player
    float mVelX, mVelY;

    // The velocity and acceleration for jumping
    float mJumpSpeed;
    float mGravity;

    // The texture of the player
    SDL_Texture* mTexture;

    // Loads individual image as texture
    SDL_Texture* loadTexture(std::string path);

    // Renderer
    SDL_Renderer* mRenderer;

    // Ground level
    static const int GROUND_LEVEL = SCREEN_HEIGHT - PLAYER_HEIGHT - 80;

    // Method to apply gravity
    void applyGravity();

    // Target position for player movement
    int mTargetX;
    bool mMovingToTarget;
};

#endif