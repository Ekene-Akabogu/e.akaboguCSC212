#include "test.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <string>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time() function

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
int speed = 4.0;
int CameraX = 0;

struct Ball {
    int x, y;
    int dx, dy;
};

const int BALL_RADIUS = 72;
const int GRAVITY = 1;
Ball ball;

SDL_Texture* loadTexture(const std::string& path) {
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Creates texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool initSDL() 
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create Window
    gWindow = SDL_CreateWindow("Ball Rotation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create Renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Set Renderer Color
    SDL_SetRenderDrawColor(gRenderer, 0xDD, 0xDD, 0xDD, 0xFF);

    // Allows for (Initializes) Image Loading abilities using SDL_image library
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void updateBall() {
    ball.x += ball.dx;
    ball.y += ball.dy;

    // Check collision with window edges
    if (ball.x < 0) {
        ball.x = 0;
        ball.dx = rand() % 5 + 1; // Random positive x speed
    }
    else if (ball.x + BALL_RADIUS > SCREEN_WIDTH) {
        ball.x = SCREEN_WIDTH - BALL_RADIUS;
        ball.dx = -(rand() % 5 + 1); // Random negative x speed
    }

    if (ball.y < 0) {
        ball.y = 0;
        ball.dy = rand() % 5 + 1; // Random positive y speed
    }
    else if (ball.y + BALL_RADIUS > SCREEN_HEIGHT) {
        ball.y = SCREEN_HEIGHT - BALL_RADIUS;
        ball.dy = -(rand() % 5 + 1); // Random negative y speed
    }
}

int main(int argc, char* argv[]) {

    std::srand(std::time(0)); // Seed the random number generator

    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    SDL_Texture* ballText = loadTexture("./Assets/ball.png");
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.dx = rand() % 11 - 5; // Random x speed between -5 and 5
    ball.dy = rand() % 11 - 5; // Random y speed between -5 and 5

    // Main loop flag
    bool quit = false;

    // Event Handler
    SDL_Event e;

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_RenderClear(gRenderer);

        updateBall();

        SDL_Rect ballRect = { ball.x, ball.y, BALL_RADIUS, BALL_RADIUS };
        SDL_RenderCopy(gRenderer, ballText, NULL, &ballRect);

        // Update screen
        SDL_RenderPresent(gRenderer);
        SDL_Delay(10);
    }

    // Destroy textures
    SDL_DestroyTexture(ballText);

    // Close SDL
    closeSDL();

    return 0;
}
