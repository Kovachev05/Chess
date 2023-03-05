#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "chessBoard.hpp"
#include "pieceMovement.hpp"

const int WIDTH = 800, HEIGHT = 800; // Screen resolution

int X, Y; // The rounded down coordinates of a user's mouse click

int main(int argc, char *argv[])
{
    // Create window and renderer
    SDL_Window *window = SDL_CreateWindow("chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    if(NULL == window)
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_Init(0); // Initialize sound
    SDL_RenderClear(renderer);

    // Create new object
    chessBoard chess;

    // Draw board and pieces
    chess.drawFrame(renderer, window);

    SDL_Event event;

    // Game loop      
    while(true)
    {
        if(SDL_PollEvent(&event))
        {
            if(SDL_QUIT == event.type)
            {
                break;
            }
            // Triggered when clicking on a piece
            if(SDL_MOUSEBUTTONDOWN == event.type)
            {
                if(SDL_BUTTON_LEFT == event.button.button)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    X = x - (x%100);
                    Y = y - (y%100);

                    for(int i = 0; i < 32; i++)
                    {
                        if(X == chess.xPos[i] && Y == chess.yPos[i])
                        {
                            chess.drawFrame(renderer, window);
                            // Determine which piece is clicked
                            if(turn == true) // Allow white pieces to move only if its white's turn
                            {
                                identifyPiece(renderer, window, chess, X, Y, true); // "true" for white
                            }
                            else
                            {
                                identifyPiece(renderer, window, chess, X, Y, false); // "false" for black
                            }
                        }
                    }
                }
            }
            // Clear the screen
            SDL_RenderClear(renderer);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

