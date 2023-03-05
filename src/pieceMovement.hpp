#pragma once

// Include libraries
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 
#include <SDL2/SDL_mixer.h>
#include "chessBoard.hpp"

// Prototype declarations
void kingSafety(SDL_Renderer *renderer, SDL_Window *window, chessBoard& a);
int legal(SDL_Renderer *renderer, SDL_Window *window, chessBoard& a, int tile_x, int ty, int n);
int swapForCheck(SDL_Renderer *renderer, SDL_Window *window, chessBoard& a, int tile_x, int tile_y, int n);
void mouseClick(SDL_Renderer *renderer, SDL_Window *window, chessBoard& a, int n, int& X, int& Y);
void capture(SDL_Renderer *renderer, SDL_Window *window, chessBoard& a, int n, int X, int Y);
void playAudio(bool capturePiece);
void highlight(SDL_Renderer *renderer, SDL_Window *window, chessBoard& a);
void identifyPiece(SDL_Renderer *renderer, SDL_Window *window, chessBoard& a, int& X, int& Y, bool white);
bool checkMate(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess);
void checkAnalasys(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess);

bool turn = true; // True for white's turn, false for black's turn
bool checking_king_safety = false; // True for when the program is checking whether a king is under attack
bool search_checkmate = false; // True for when the program is searching for checkmate
bool side_moves = true; // True for when the program is collecting all the other side's possible moves, false for current side moves
bool king_check[2] = {false, false}; // Variable that stores whether both kings are under check
int counter = 0; // Counter increments after each piece capture. Used in playAudio() function
int num_checks = 0; // Number of given checks
bool checkmate = false; 

SDL_Event click;
// Move 1 square up/down/left/right/diagonally
void king(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int& X, int& Y)
{
    // Find all legal moves for king

    // Rightwards
    int tile_x = chess.xPos[n]/100 + 1;// Create "tile-x" var
    int tile_y = chess.yPos[n]/100; // Create "tile-y" var
    if(tile_x < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    // Leftwards
    tile_x = chess.xPos[n]/100 - 1;
    tile_y = chess.yPos[n]/100;
    if(tile_x >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    // Downwards
    tile_x = chess.xPos[n]/100;
    tile_y = chess.yPos[n]/100 + 1;
    if(tile_y < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    // Upwards
    tile_x = chess.xPos[n]/100;
    tile_y = chess.yPos[n]/100 - 1;
    if(tile_y >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    // Descending rightwards
    tile_x = chess.xPos[n]/100 + 1;
    tile_y = chess.yPos[n]/100 + 1;
    if(tile_x < 8 && tile_y < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    // Ascending rightwards
    tile_x = chess.xPos[n]/100 + 1;
    tile_y = chess.yPos[n]/100 - 1;
    if(tile_x < 8 && tile_y >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    // Ascending leftwards
    tile_x = chess.xPos[n]/100 - 1;
    tile_y = chess.yPos[n]/100 - 1;
    if(tile_x >= 0 && tile_y >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    // Ascending leftwards
    tile_x = chess.xPos[n]/100 - 1;
    tile_y = chess.yPos[n]/100 + 1;
    if(tile_x >= 0 && tile_y < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }
    
    if((checking_king_safety == false) && (search_checkmate == false))
    {
        highlight(renderer, window, chess);
        SDL_RenderPresent(renderer);
        mouseClick(renderer, window, chess, n, X, Y);
    }
}

// Move 1 square up/diagonally (white) 1 square down/diagonally (black)
void pawn(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int& X, int& Y)
{
    // Find all legal moves for pawn

    int tile_x = chess.xPos[n]/100;// Create "tile-x" var
    int tile_y = chess.yPos[n]/100; // Create "tile-y" var
    int i = 1;
    // White
    if(n < 18)
    {
        // Highlight diagonals if there is an enemy piece
        tile_x = chess.xPos[n]/100 + 1;
        tile_y = chess.yPos[n]/100 - 1;
        if((tile_x <= 7) && (tile_y >= 0) && (chess.board[tile_y][tile_x] > 15) && (chess.board[tile_y][tile_x] < 32))
        {   
            if((search_checkmate == true) && (chess.other_side_moves[tile_y][tile_x] == 5))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
            (search_checkmate == false) ? legal(renderer, window, chess, tile_x, tile_y, n) : false;
        }

        tile_x = chess.xPos[n]/100 - 1;
        tile_y = chess.yPos[n]/100 - 1;
        if((tile_x >= 0) && (tile_y >= 0) && (chess.board[tile_y][tile_x] > 15) && (chess.board[tile_y][tile_x] < 32))
        {
            if((search_checkmate == true) && (chess.other_side_moves[tile_y][tile_x] == 5))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
            (search_checkmate == false) ? legal(renderer, window, chess, tile_x, tile_y, n) : false;
        }

        tile_x = chess.xPos[n]/100;
        tile_y = chess.yPos[n]/100;
        // Move two tiles if on the 7th rank
        if(tile_y == 6)
        {
            i = 0;
        }
        for(int j = i; j < 2; j++)
        {
            tile_y -= 1;
            if(tile_x < 0 || chess.board[tile_y][tile_x] != 50)
            {
                break;
            }
            if((checking_king_safety == false) && (search_checkmate == false))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
            else if((chess.other_side_moves[tile_y][tile_x] == 2) && (search_checkmate == true) && (num_checks == 1))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
        }
    }
    // Black
    else
    {
        // Highlight diagonals if there is an enemy piece
        tile_x = chess.xPos[n]/100 + 1;// Create "tile-x" var
        tile_y = chess.yPos[n]/100 + 1; // Create "tile-y" var
        if(tile_x <= 7 && tile_y <= 7 && chess.board[tile_y][tile_x] < 16)
        {
            if((search_checkmate == true) && (chess.other_side_moves[tile_y][tile_x] == 5))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
            (search_checkmate == false) ? legal(renderer, window, chess, tile_x, tile_y, n) : false;
        }

        tile_x = chess.xPos[n]/100 - 1;// Create "tile-x" var
        tile_y = chess.yPos[n]/100 + 1; // Create "tile-y" var
        if(tile_x >= 0 && tile_y <= 7 && chess.board[tile_y][tile_x] < 16)
        {
            if((search_checkmate == true) && (chess.other_side_moves[tile_y][tile_x] == 5))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
            (search_checkmate == false) ? legal(renderer, window, chess, tile_x, tile_y, n) : false;
        }

        tile_x = chess.xPos[n]/100;// Create "tile-x" var
        tile_y = chess.yPos[n]/100; // Create "tile-y" var
        // Move two tiles if on the 7th rank
        if(tile_y == 1)
        {
            i = 0;
        }
        for(int j = i; j < 2; j++)
        {
            tile_y += 1;
            if(tile_x >= 8 || chess.board[tile_y][tile_x] != 50)
            {
                break;
            }
            if((checking_king_safety == false) && (search_checkmate == false))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
            else if((chess.other_side_moves[tile_y][tile_x] == 2) && (search_checkmate == true) && (num_checks == 1))
            {
                legal(renderer, window, chess, tile_x, tile_y, n);
            }
        }
}

if((checking_king_safety == false) && (search_checkmate == false))
{
    highlight(renderer, window, chess);
    SDL_RenderPresent(renderer);
    mouseClick(renderer, window, chess, n, X, Y);
}
}

// Move diagonally in all directions
void bishop(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int& X, int& Y)
{
    // Find all legal moves for bishop

    // Descending rightwards
    for(int tile_y = chess.yPos[n]/100 + 1, tile_x = chess.xPos[n]/100 + 1; (tile_y < 8) && (tile_x < 8); tile_y++, tile_x++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Ascending leftwards
    for(int tile_y = chess.yPos[n]/100 - 1, tile_x = chess.xPos[n]/100 - 1; (tile_y >= 0) && (tile_x >= 0); tile_y--, tile_x--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Ascending rightwards
    for(int tile_y = chess.yPos[n]/100 - 1, tile_x = chess.xPos[n]/100 + 1; (tile_y >= 0) && (tile_x < 8); tile_y--, tile_x++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Descending leftwards
    for(int tile_y = chess.yPos[n]/100 + 1, tile_x = chess.xPos[n]/100 - 1; (tile_y < 8) && (tile_x >= 0); tile_y++, tile_x--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }

    if((checking_king_safety == false) && (search_checkmate == false))
    {
        highlight(renderer, window, chess);
        SDL_RenderPresent(renderer);
        mouseClick(renderer, window, chess, n, X, Y);
    }
}

// Move straight up/down/left/right
void rook(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int& X, int& Y)
{
    // Find all legal moves for rook

    // Rightwards
    for(int tile_y = chess.yPos[n]/100, tile_x = chess.xPos[n]/100 + 1; tile_x < 8; tile_x++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Leftwards
    for(int tile_y = chess.yPos[n]/100, tile_x = chess.xPos[n]/100 - 1; tile_x >= 0; tile_x--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Downwards
    for(int tile_y = chess.yPos[n]/100 + 1, tile_x = chess.xPos[n]/100; tile_y < 8; tile_y++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Upwards
    for(int tile_y = chess.yPos[n]/100 - 1, tile_x = chess.xPos[n]/100; tile_y >= 0; tile_y--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }

    if((checking_king_safety == false) && (search_checkmate == false))
    {
        highlight(renderer, window, chess);
        SDL_RenderPresent(renderer);
        mouseClick(renderer, window, chess, n, X, Y);
    }
}

//Combine rook and bishop functionalities
void queen(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int& X, int& Y)
{
    // Find all legal moves for queen

    // Rightwards
    for(int tile_y = chess.yPos[n]/100, tile_x = chess.xPos[n]/100 + 1; tile_x < 8; tile_x++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Leftwards
    for(int tile_y = chess.yPos[n]/100, tile_x = chess.xPos[n]/100 - 1; tile_x >= 0; tile_x--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Downwards
    for(int tile_y = chess.yPos[n]/100 + 1, tile_x = chess.xPos[n]/100; tile_y < 8; tile_y++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Upwards
    for(int tile_y = chess.yPos[n]/100 - 1, tile_x = chess.xPos[n]/100; tile_y >= 0; tile_y--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Descending rightwards
    for(int tile_y = chess.yPos[n]/100 + 1, tile_x = chess.xPos[n]/100 + 1; (tile_y < 8) && (tile_x < 8); tile_y++, tile_x++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Ascending leftwards
    for(int tile_y = chess.yPos[n]/100 - 1, tile_x = chess.xPos[n]/100 - 1; (tile_y >= 0) && (tile_x >= 0); tile_y--, tile_x--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Ascending rightwards
    for(int tile_y = chess.yPos[n]/100 - 1, tile_x = chess.xPos[n]/100 + 1; (tile_y >= 0) && (tile_x < 8); tile_y--, tile_x++)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }
    // Descending leftwards
    for(int tile_y = chess.yPos[n]/100 + 1, tile_x = chess.xPos[n]/100 - 1; (tile_y < 8) && (tile_x >= 0); tile_y++, tile_x--)
    {
        if(legal(renderer, window, chess, tile_x, tile_y, n) == 0)
        {
            break;
        }
    }

    if((checking_king_safety == false) && (search_checkmate == false))
    {
        highlight(renderer, window, chess);
        SDL_RenderPresent(renderer);
        mouseClick(renderer, window, chess, n, X, Y);
    }
}

// Move in an "L"-like shape
void knight(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int& X, int& Y)
{
    // Find all legal positions for knight

    int tile_x = chess.xPos[n]/100 + 1;// Create "tile-x" var
    int tile_y = chess.yPos[n]/100 - 2; // Create "tile-y" var
    if(tile_x < 8 && tile_y >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    tile_x = chess.xPos[n]/100 - 1;
    tile_y = chess.yPos[n]/100 - 2;
    if(tile_x >= 0 && tile_y >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    tile_x = chess.xPos[n]/100 - 2;
    tile_y = chess.yPos[n]/100 + 1;
    if(tile_x >= 0 && tile_y < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    tile_x = chess.xPos[n]/100 - 2;
    tile_y = chess.yPos[n]/100 - 1;
    if(tile_x >= 0 && tile_y >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    tile_x = chess.xPos[n]/100 + 2;
    tile_y = chess.yPos[n]/100 + 1;
    if(tile_x < 8 && tile_y < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    tile_x = chess.xPos[n]/100 + 2;
    tile_y = chess.yPos[n]/100 - 1;
    if(tile_x < 8 && tile_y >= 0)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    tile_x = chess.xPos[n]/100 + 1;
    tile_y = chess.yPos[n]/100 + 2;
    if(tile_x < 8 && tile_y < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    tile_x = chess.xPos[n]/100 - 1;
    tile_y = chess.yPos[n]/100 + 2;
    if(tile_x >= 0 && tile_y < 8)
    {
        legal(renderer, window, chess, tile_x, tile_y, n);
    }

    if((checking_king_safety == false) && (search_checkmate == false))
    {
        highlight(renderer, window, chess);
        SDL_RenderPresent(renderer);
        mouseClick(renderer, window, chess, n, X, Y);
    }
}

// Analyze the board and determine whether a king is under check
void kingSafety(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess)
{
    checking_king_safety = true;
    side_moves = true;
    chess.resetMoves();
    // Analyze all legal moves for white
    if(turn == false)
    {
        for(int i = 1; i < 16; i++)
        {
            identifyPiece(renderer, window, chess, chess.xPos[i], chess.yPos[i], true);
        }
        identifyPiece(renderer, window, chess, chess.xPos[0], chess.yPos[0], true);
        side_moves = false;
        for(int i = 17; i < 32; i++)
        {
            identifyPiece(renderer, window, chess, chess.xPos[i], chess.yPos[i], false);
        }
        identifyPiece(renderer, window, chess, chess.xPos[16], chess.yPos[16], false);
    } 
    // Analyze all legal moves for black
    else
    {
        for(int i = 17; i < 32; i++)
        {
            identifyPiece(renderer, window, chess, chess.xPos[i], chess.yPos[i], false);
        }
        identifyPiece(renderer, window, chess, chess.xPos[16], chess.yPos[16], true);
        side_moves = false;
        for(int i = 1; i < 16; i++)
        {
            identifyPiece(renderer, window, chess, chess.xPos[i], chess.yPos[i], true);
        }
        identifyPiece(renderer, window, chess, chess.xPos[0], chess.yPos[0], true);
    }

    // Set king_check var true if king is under check
    // For white
    if(turn == true)
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if(chess.board[i][j] == 0 && chess.other_side_moves[i][j] == 1)
                {
                    king_check[0] = true;
                    goto check; // Exit nested for loop
                }
                king_check[0] = false;
            }
        }
    }
    // For black
    else
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if(chess.board[i][j] == 16 && chess.other_side_moves[i][j] == 1)
                {
                    king_check[1] = true;
                    goto check; // Exit nested for loop
                }
                king_check[1] = false;
            }
        }
    }
    check:
    checking_king_safety = false;
}

bool checkMate(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess)
{
    num_checks = 0; // Reset number of given checks

    search_checkmate = true;
    if(turn == false)
    {
        for(int i = 1; i < 16; i++)
        {
            chess.resetLegalMoves();
            identifyPiece(renderer, window, chess, chess.xPos[i], chess.yPos[i], true);
            for(int j = 0; j < 8; j++)
            {
                for(int k = 0; k < 8; k++)
                {
                    if((chess.legal_moves[j][k] == 1) && (chess.board[j][k] == 16))
                    {
                        chess.other_side_moves[chess.yPos[i]/100][chess.xPos[i]/100] = 5; // Lable pieces that are threatening the king with a "5"
                    }
                }
            }
        }
    }
    else if(turn == true)
    {
        for(int i = 17; i < 32; i++)
        {
            chess.resetLegalMoves();
            identifyPiece(renderer, window, chess, chess.xPos[i], chess.yPos[i], false);
            for(int j = 0; j < 8; j++)
            {
                for(int k = 0; k < 8; k++)
                {
                    if((chess.legal_moves[j][k] == 1) && (chess.board[j][k] == 0))
                    {
                        chess.other_side_moves[chess.yPos[i]/100][chess.xPos[i]/100] = 5; // Lable pieces that are threatening the king with a "5"
                    }
                }
            }
        }
    }
    chess.resetLegalMoves();
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(chess.current_side_moves[i][j] == 4)
            {
                std::cout << "chess.current_side_moves[i][j] == 4: " << i << ", " << j << " chess.board[i][j]: "<< chess.board[i][j] << std::endl;
                if(chess.board[i][j] > 15)
                {
                    std::cout << "black" << std::endl;
                    for(int k = 1; k < 16; k++)
                    {
                        chess.resetLegalMoves();
                        identifyPiece(renderer, window, chess, chess.xPos[k], chess.yPos[k], true);
                        if(chess.legal_moves[i][j] == 1)
                        {
                            std::cout << "666666666" << std::endl;
                            chess.current_side_moves[i][j] = 6;
                        }
                    }
                }
                else if(chess.board[i][j] < 16)
                {
                    std::cout << "white" << std::endl;
                    for(int k = 17; k < 32; k++)
                    {
                        chess.resetLegalMoves();
                        identifyPiece(renderer, window, chess, chess.xPos[k], chess.yPos[k], false);
                        if(chess.legal_moves[i][j] == 1)
                        {
                            chess.current_side_moves[i][j] = 6;
                            std::cout << "666666666" << std::endl;
                        }
                    }
                }
            }
        }
    }

    chess.resetLegalMoves();
    
    search_checkmate = false;

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if((chess.current_side_moves[i][j] == 3) && (chess.other_side_moves[i][j] == 2))
            {
                chess.current_side_moves[i][j] = 0;
            }
            if(chess.other_side_moves[i][j] == 5)
            {
                num_checks++;
            }
        }
    }
    std::cout << "num_checks: " << num_checks << std::endl;

    // No checkmate if piece can be captured
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(chess.current_side_moves[i][j] == 6)
            {
                std::cout << "n. 0" << std::endl;
                return false;
            }
        }
    }

    // No checkmate if king has legal moves
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(((chess.current_side_moves[i][j] == 3) || (chess.current_side_moves[i][j] == 2)) && ((chess.other_side_moves[i][j] != 2) && (chess.other_side_moves[i][j] != 1)))
            {
                std::cout << "n. 2" << std::endl;
                return false;
            }
        }
    }

    if(num_checks == 0)
    {
        std::cout << "n. 3" << std::endl;
        return false;
    }

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(chess.current_side_moves[i][j] == 4)
            {
                std::cout << "n. 4" << std::endl;
                return true;
            }
        }
    }

    // No checkmate if piece can block check
    if(num_checks == 1)
    {
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                if(((chess.current_side_moves[i][j] == 1) || (chess.current_side_moves[i][j] == 2)) && (chess.other_side_moves[i][j] == 2))
                {
                    std::cout << "n. 1" << std::endl;
                    return false;
                }
            }
        }
    }

    std::cout << "n. 5" << std::endl;
    return true;
}

// Highlight all legal moves for a piece
void highlight(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess)
{
    // Draw board
    SDL_Rect point;
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = NULL;

    point.w = 100;
    point.h = 100;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            //std::cout << chess.legal_moves[i][j] << ":";
            if(chess.legal_moves[i][j] == 1)
            {
                point.x = j*100;
                point.y = i*100;
                IMG_Init(IMG_INIT_PNG);
                if(chess.board[i][j] == 50)
                {
                    surface = IMG_Load("../assets/pieces/point_30%.png");
                } 
                // Highlight differently when highlighting pieces
                else
                {
                    surface = IMG_Load("../assets/pieces/point_capture_30%.png");
                }
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                SDL_RenderCopy(renderer, texture, NULL, &point);
            }
        }
        //std::cout << std::endl;
    }
}

// Check whether the tile in question is free or has an enemy piece,
// hence determining whether the move is legal or not
int legal(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int tile_x, int tile_y, int n)
{
    // Only allow certain moves while king is under check
    if((((king_check[0] == true) && (turn == true)) || ((king_check[1] == true) && (turn == false))) && ((checking_king_safety == false) && (search_checkmate == false)))
    {
        // Legal move only if king is no longer under check after moving piece to tile
        if(chess.board[tile_y][tile_x] == 50)
        {
            return swapForCheck(renderer, window, chess, tile_x, tile_y, n);
        }
        // Legal move only if king is no longer under check after piece capturing
        else if(((chess.board[tile_y][tile_x] > 15 && n < 16) || (chess.board[tile_y][tile_x] < 16 && n > 15)) && (checking_king_safety == false))
        {
            return swapForCheck(renderer, window, chess, tile_x, tile_y, n);
        }    
    }
    // Mark legal if tile is free
    if(chess.board[tile_y][tile_x] == 50)
    {
        if(search_checkmate == true)
        {
            chess.legal_moves[tile_y][tile_x] = 1;
            return 1;
        }
        if(checking_king_safety == true)
        {
            if(side_moves == true)
            {
                chess.other_side_moves[tile_y][tile_x] = 1; 
                return 1;
            }
            if((n == 0) || (n == 16))
            {
                if(chess.current_side_moves[tile_y][tile_x] == 0)
                {
                    chess.current_side_moves[tile_y][tile_x] = 3;
                }
                else if(chess.current_side_moves[tile_y][tile_x] == 1)
                {
                    chess.current_side_moves[tile_y][tile_x] = 2;
                }
                return 1;
            }
            chess.current_side_moves[tile_y][tile_x] = 1;
            return 1;
        }
        swapForCheck(renderer, window, chess, tile_x, tile_y, n);  
        return 1;
    }
    // Mark legal if tile holds enemy piece and break out of loop
    else if((chess.board[tile_y][tile_x] > 15 && n < 16) || (chess.board[tile_y][tile_x] < 16 && n > 15))
    {
        if(search_checkmate == true)
        {
            chess.legal_moves[tile_y][tile_x] = 1;
            if((chess.board[tile_y][tile_x] == 0 && n < 16) || (chess.board[tile_y][tile_x] == 16 && n > 15))
            {
                return 1;
            }
            return 0;
        }
        if(checking_king_safety == true)
        {
            if(side_moves == true)
            {
                chess.other_side_moves[tile_y][tile_x] = 1; 
                return 0;
            }
            if((n == 0) || (n == 16))
            {
                if(chess.current_side_moves[tile_y][tile_x] == 0)
                {
                    chess.current_side_moves[tile_y][tile_x] = 3;
                }
                else if(chess.current_side_moves[tile_y][tile_x] == 1)
                {
                    chess.current_side_moves[tile_y][tile_x] = 2;
                }
                return 0;
            }
            chess.current_side_moves[tile_y][tile_x] = 1;
            return 0;
        }
        swapForCheck(renderer, window, chess, tile_x, tile_y, n);
        return 0;
    }
    else if((chess.board[tile_y][tile_x] > 15 && n > 15) || (chess.board[tile_y][tile_x] < 16 && n < 16))
    {
        if(search_checkmate == true)
        {
            if((((turn == true) && (chess.board[tile_y][tile_x] > 15 && n > 15)) || ((turn == false) && (chess.board[tile_y][tile_x] < 16 && n < 16))) && (chess.other_side_moves[tile_y][tile_x] == 5))
            {
                chess.current_side_moves[tile_y][tile_x] = 4; // The piece threatening check is protected
            }
        }
        return 0;
    }
    return 1;
}

// Temporarly move current piece to another tile to see whether king is under check
int swapForCheck(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int tile_x, int tile_y, int n)
{
    // Save king check data to a temporary variable 
    bool buffer_check[2] = {false, false};
    ((king_check[0] == true)) ? buffer_check[0] = true : buffer_check[0] = false;
    ((king_check[1] == true)) ? buffer_check[1] = true : buffer_check[1] = false;

    // Initialize temporary variables
    int buffer_x = chess.xPos[n]/100;
    int buffer_y = chess.yPos[n]/100;
    int buffer_tile = chess.board[tile_y][tile_x];

    // Swap
    chess.board[buffer_y][buffer_x] = 50;
    chess.board[tile_y][tile_x] = n;
    kingSafety(renderer, window, chess); 
    chess.board[buffer_y][buffer_x] = n;
    chess.board[tile_y][tile_x] = buffer_tile;

    if(((king_check[0] == false) && (turn == true)) || ((king_check[1] == false) && (turn == false)))
    {
        chess.legal_moves[tile_y][tile_x] = 1; 
        // Reset king check status
        for(int i = 0; i < 2; i++)
        {
            king_check[i] = buffer_check[i];
        }
        return 0;
    }  
    // Reset king check status
    for(int i = 0; i < 2; i++)
    {
        king_check[i] = buffer_check[i];
    }
    return 1;
}

// Analyze how and from where the king is being checked
// Highlight the king check trails with a "2"
void checkAnalasys(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess)
{
    int king;
    (turn == true) ? king = 0 : king = 16;
    chess.other_side_moves[chess.yPos[king]/100][chess.xPos[king]/100] = 2;

    // Rightwards
    for(int tile_y = chess.yPos[king]/100, tile_x = chess.xPos[king]/100 + 1, i = 0; tile_x < 8; tile_x++, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
    // Leftwards
    for(int tile_y = chess.yPos[king]/100, tile_x = chess.xPos[king]/100 - 1, i = 0; tile_x >= 0; tile_x--, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
    // Upwards
    for(int tile_y = chess.yPos[king]/100 - 1, tile_x = chess.xPos[king]/100, i = 0; tile_y >= 0; tile_y--, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
    // Downwards
    for(int tile_y = chess.yPos[king]/100 + 1, tile_x = chess.xPos[king]/100, i = 0; tile_y < 8; tile_y++, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
    // Descending rightwards
    for(int tile_y = chess.yPos[king]/100 + 1, tile_x = chess.xPos[king]/100 + 1, i = 0; tile_y < 8, tile_x < 8; tile_y++, tile_x++, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
    // Descending leftwards
    for(int tile_y = chess.yPos[king]/100 + 1, tile_x = chess.xPos[king]/100 - 1, i = 0; tile_y < 8, tile_x >= 0; tile_y++, tile_x--, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
    // Ascending rightwards
    for(int tile_y = chess.yPos[king]/100 - 1, tile_x = chess.xPos[king]/100 + 1, i = 0; tile_y >= 0, tile_x < 8; tile_y--, tile_x++, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
    // Ascending leftwards
    for(int tile_y = chess.yPos[king]/100 - 1, tile_x = chess.xPos[king]/100 - 1, i = 0; tile_y >= 0, tile_x >= 0; tile_y--, tile_x--, i++)
    {
        if((chess.other_side_moves[tile_y][tile_x] != 1) || (chess.board[tile_y][tile_x] != 50))
        {
            break;
        }
        chess.other_side_moves[tile_y][tile_x] = 2;
        //(i == 0) ? num_checks++ : false;
    }
}

// Check if move is legal. If it is, move piece
void mouseClick(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int& X, int& Y)
{
    while(true)
    {
        if(SDL_PollEvent(&click))
        {
            if(SDL_MOUSEBUTTONDOWN == click.type)
            {
                if(SDL_BUTTON_LEFT == click.button.button)
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    X = x - (x%100);
                    Y = y - (y%100);
                    if(chess.legal_moves[Y/100][X/100] == 1)
                    {
                        capture(renderer, window, chess, n, X, Y);
                        break;
                    }
                    chess.resetLegalMoves();
                    chess.drawFrame(renderer, window);
                    break;
                }
            }
        }
    }
}

void capture(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int n, int X, int Y)
{
    int x1 = (X - chess.xPos[n])/10;
    int y1 = (Y - chess.yPos[n])/10;
    // Disallow like-color piece capturing
    if((chess.board[Y/100][X/100] == 50))
    {
        // Smoothly move piece to the target tile and play sound
        chess.board[chess.yPos[n]/100][chess.xPos[n]/100] = 50;
        for(int i = 0; i < 10; i++)
        {
            chess.board[chess.yPos[n]/100][chess.xPos[n]/100] = 50;
            chess.xPos[n] += x1;
            chess.yPos[n] += y1;
            chess.drawFrame(renderer, window);
        }
        chess.resetLegalMoves();
        chess.resetMoves();
        turn = !turn; // Change turns
        kingSafety(renderer, window, chess);
        checkAnalasys(renderer, window, chess);
        checkmate = checkMate(renderer, window, chess);
        playAudio(false);
        //------DEBUG------
        std::cout << std::endl;
        std::cout << "other side moves: " << std::endl;
        std::cout << std::endl;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                std::cout << chess.other_side_moves[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "current side moves: " << std::endl;
        std::cout << std::endl;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                std::cout << chess.current_side_moves[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else if((chess.board[Y/100][X/100] < 16 && n > 15) || (chess.board[Y/100][X/100] > 15 && n < 16))
    {
        // Move captured piece outside of the board
        chess.xPos[chess.board[Y/100][X/100]] = -10000;
        chess.yPos[chess.board[Y/100][X/100]] = -10000;
        // Smoothly move piece to the target piece and play sound
        chess.board[chess.yPos[n]/100][chess.xPos[n]/100] = 50;
        for(int i = 0; i < 10; i++)
        {
            chess.board[chess.yPos[n]/100][chess.xPos[n]/100] = 50;
            chess.xPos[n] += x1;
            chess.yPos[n] += y1;
            chess.drawFrame(renderer, window);
        }
        chess.resetLegalMoves();
        chess.resetMoves();
        turn = !turn; // Change turns
        kingSafety(renderer, window, chess);
        checkAnalasys(renderer, window, chess);
        checkmate = checkMate(renderer, window, chess);
        playAudio(true);
        //------DEBUG------
        std::cout << std::endl;
        std::cout << "other side moves: " << std::endl;
        std::cout << std::endl;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                std::cout << chess.other_side_moves[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        std::cout << "current side moves: " << std::endl;
        std::cout << std::endl;
        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                std::cout << chess.current_side_moves[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

void playAudio(bool capturePiece)
{
    // Set up audio
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_Chunk* move_1 = Mix_LoadWAV("../assets/audios/move_1.wav");
    Mix_Chunk* move_2 = Mix_LoadWAV("../assets/audios/move_2.wav");
    Mix_Chunk* capture_1 = Mix_LoadWAV("../assets/audios/capture_1.wav");
    Mix_Chunk* capture_2 = Mix_LoadWAV("../assets/audios/capture_2.wav");
    Mix_Chunk* capture_3 = Mix_LoadWAV("../assets/audios/capture_3.wav");
    Mix_Chunk* capture_4 = Mix_LoadWAV("../assets/audios/capture_4.wav");
    Mix_Chunk* capture_5 = Mix_LoadWAV("../assets/audios/capture_5.wav");
    Mix_Chunk* capture_6 = Mix_LoadWAV("../assets/audios/capture_6.wav");
    Mix_Chunk* capture_7 = Mix_LoadWAV("../assets/audios/capture_7.wav");
    Mix_Chunk* capture_8 = Mix_LoadWAV("../assets/audios/capture_8.wav");
    Mix_Chunk* check_1 = Mix_LoadWAV("../assets/audios/check_1.wav");
    Mix_Chunk* checkmate_1 = Mix_LoadWAV("../assets/audios/checkmate_1.wav");
    int rand_num = 1 + (rand() % 2); // Randomly pick which soundtrack to play

    if(((king_check[0] == true) && (turn == true)) || ((king_check[1] == true) && (turn == false)))
    {
        counter = 0;
        if(checkmate == true)
        {
            Mix_PlayChannel(-1, checkmate_1, 0);
            std::cout << "CHECKMATE" << std::endl;
            return;
        }
        Mix_PlayChannel(-1, check_1, 0);
        return;
    }

    if(capturePiece == true)
    {
        switch(counter)
        {
            case 0:
                Mix_PlayChannel(-1, capture_1, 0);
                break;
            case 1:
                Mix_PlayChannel(-1, capture_2, 0);
                break;
            case 2:
                Mix_PlayChannel(-1, capture_3, 0);
                break;
            case 3: 
                Mix_PlayChannel(-1, capture_4, 0);
                break;
            case 4:
                Mix_PlayChannel(-1, capture_5, 0);
                break;
            case 5:
                Mix_PlayChannel(-1, capture_6, 0);
                break;
            case 6:
                Mix_PlayChannel(-1, capture_7, 0);
                break;
            case 7:
                Mix_PlayChannel(-1, capture_8, 0);
                break;
        }
        counter++;
    }  
    else
    {
        (rand_num == 1) ? Mix_PlayChannel(-1, move_1, 0) : Mix_PlayChannel(-1, move_2, 0);
        counter = 0;
    }
    
    (counter > 7) ? counter = 0 : false;
}

void identifyPiece(SDL_Renderer *renderer, SDL_Window *window, chessBoard& chess, int& X, int& Y, bool white)
{
    if(white == true) // Allow white pieces to move only if its white's turn
    {
        switch(chess.board[Y/100][X/100])
        {
            case 0:
                //std::cout << "w_king" << std::endl;
                king(renderer, window, chess, 0, X, Y);
                break;
            case 1:
                //std::cout << "w_queen" << std::endl;
                queen(renderer, window, chess, 1, X, Y);
                break;
            case 2:
                //std::cout << "w_bishop_1" << std::endl;
                bishop(renderer, window, chess, 2, X, Y);
                break;
            case 3:
                //std::cout << "w_bishop_2" << std::endl;
                bishop(renderer, window, chess, 3, X, Y);
                break;
            case 4:
                //std::cout << "w_knight_1" << std::endl;
                knight(renderer, window, chess, 4, X, Y);
                break;
            case 5:
                //std::cout << "w_knight_2" << std::endl;
                knight(renderer, window, chess, 5, X, Y);
                break;
            case 6:
                //std::cout << "w_rook_1" << std::endl;
                rook(renderer, window, chess, 6, X, Y);
                break;
            case 7:
                //std::cout << "w_rook_2" << std::endl;
                rook(renderer, window, chess, 7, X, Y);
                break;
            case 8:
                //std::cout << "w_pawn" << std::endl;
                pawn(renderer, window, chess, 8, X, Y);
                break;
            case 9:
                //std::cout << "w_pawn_2" << std::endl;
                pawn(renderer, window, chess, 9, X, Y);
                break;
            case 10:
                //std::cout << "w_pawn_3" << std::endl;
                pawn(renderer, window, chess, 10, X, Y);
                break;
            case 11:
                //std::cout << "w_pawn_4" << std::endl;
                pawn(renderer, window, chess, 11, X, Y);
                break;
            case 12:
                //std::cout << "w_pawn_5" << std::endl;
                pawn(renderer, window, chess, 12, X, Y);
                break;
            case 13:
                //std::cout << "w_pawn_6" << std::endl;
                pawn(renderer, window, chess, 13, X, Y);
                break;
            case 14:
                //std::cout << "w_pawn_7" << std::endl;
                pawn(renderer, window, chess, 14, X, Y);
                break;
            case 15:
                //std::cout << "w_pawn_8" << std::endl;
                pawn(renderer, window, chess, 15, X, Y);
                break;
        }
    }
    else // Allow black pieces to move only if its black's turn
    {
        switch (chess.board[Y/100][X/100])
        {
            case 16:
                //std::cout << "b_king" << std::endl;
                king(renderer, window, chess, 16, X, Y);
                break;
            case 17:
                //std::cout << "b_queen" << std::endl;
                queen(renderer, window, chess, 17, X, Y);
                break;
            case 18:
                //std::cout << "b_bishop_1" << std::endl;
                bishop(renderer, window, chess, 18, X, Y);
                break;
            case 19:
                //std::cout << "b_bishop_2" << std::endl;
                bishop(renderer, window, chess, 19, X, Y);
                break;
            case 20:
                //std::cout << "b_knight_1" << std::endl;
                knight(renderer, window, chess, 20, X, Y);
                break;
            case 21:
                //std::cout << "b_knight_2" << std::endl;
                knight(renderer, window, chess, 21, X, Y);
                break;
            case 22:
                //std::cout << "b_rook_1" << std::endl;
                rook(renderer, window, chess, 22, X, Y);
                break;
            case 23:
                //std::cout << "b_rook_2" << std::endl;
                rook(renderer, window, chess, 23, X, Y);
                break;
            case 24:
                //std::cout << "b_pawn_1" << std::endl;
                pawn(renderer, window, chess, 24, X, Y);
                break;
            case 25:
                //std::cout << "b_pawn_2" << std::endl;
                pawn(renderer, window, chess, 25, X, Y);
                break;
            case 26:
                //std::cout << "b_pawn_3" << std::endl;
                pawn(renderer, window, chess, 26, X, Y);
                break;
            case 27:
                //std::cout << "b_pawn_4" << std::endl;
                pawn(renderer, window, chess, 27, X, Y);
                break;
            case 28:
                //std::cout << "b_pawn_5" << std::endl;
                pawn(renderer, window, chess, 28, X, Y);
                break;
            case 29:
                //std::cout << "b_pawn_6" << std::endl;
                pawn(renderer, window, chess, 29, X, Y);
                break;
            case 30:
                //std::cout << "b_pawn_7" << std::endl;
                pawn(renderer, window, chess, 30, X, Y);
                break;
            case 31:
                //std::cout << "b_pawn_8" << std::endl;
                pawn(renderer, window, chess, 31, X, Y);
                break;
        }
    }
}