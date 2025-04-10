#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include "constants.h"

class Player {
public:
    // The dimensions of the player
    static const int PLAYER_WIDTH = 100;
    static const int PLAYER_HEIGHT = 100;

    // Maximum axis velocity of the player
    static const float PLAYER_VEL;

    // Initializes the variables
    Player(SDL_Renderer* renderer);
    ~Player();
    // Handles key press
    void handleEvent(SDL_Event& e);

    // Moves the player
    void move();

    // Shows the player on the screen
    void render();

    SDL_Rect getCollider() const;



private:
    // The X and Y offsets of the player
    int mPosX, mPosY;

    // The velocity of the player
    float mVelX, mVelY;

    // The velocity and acceleration for jumping
    float mJumpSpeed;
    float mGravity;

    // Whether the player is jumping
    bool mIsJumping;

    // The texture of the player
    SDL_Texture* mTexture;

    // Loads individual image as texture
    SDL_Texture* loadTexture(std::string path);

    // Renderer
    SDL_Renderer* mRenderer;

    // Ground level
    static const int GROUND_LEVEL = SCREEN_HEIGHT - PLAYER_HEIGHT - 100;

    // Collider for the player
    SDL_Rect mCollider; // Add this member

    // Method to apply gravity
    void applyGravity();

    // Target position for player movement
    int mTargetX;
    bool mMovingToTarget;

    Mix_Chunk* mJumpSound;
};

#endif