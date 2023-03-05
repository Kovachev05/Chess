#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 

class chessBoard
{
    private:
        int tiles[8][8];
    public:
        int board[8][8]; // Track where all pieces are located
        int legal_moves[8][8]; // Track all possible locations a piece can move to
        int other_side_moves[8][8]; // Track all possible locations that pieces from the other side can move to
        int current_side_moves[8][8]; // Track all possible locations that pieces from the current side can move to
        int xPos[32] = {100}; // X position of each piece
        int yPos[32] = {100}; // Y position of each piece
    public:
        // Initialize all variables
        chessBoard()
        {
            // Initialize tile matrix to contain 1s and 0s 
            // (0s representing a dark tile and 1s a light tile)
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    if(i % 2 == 0)
                    {
                        (j % 2 == 0)? tiles[i][j] = 1 : tiles[i][j] = 0;
                    }
                    else
                    {
                        (j % 2 != 0)? tiles[i][j] = 1 : tiles[i][j] = 0;
                    }
                }
            }

            // Initialize board to contain only free tiles 
            // (A tile with a value of 50 is considered unused)
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    board[i][j] = 50;
                }
            }

            resetLegalMoves();
            resetMoves();

            // Set the initial coordinates for all pieces

            // White king
            xPos[0] = 400;
            yPos[0] = 700;

            // White queen
            xPos[1] = 300;
            yPos[1] = 700;

            // White bishop 1
            xPos[2] = 200;
            yPos[2] = 700;

            // White bishop 2
            xPos[3] = 500;
            yPos[3] = 700;

            // White knight 1
            xPos[4] = 100;
            yPos[4] = 700;

            // White knight 2
            xPos[5] = 600;
            yPos[5] = 700;

            // White rook 1
            xPos[6] = 0;
            yPos[6] = 700;

            // White rook 2
            xPos[7] = 700;
            yPos[7] = 700;

            // White pawn 1
            xPos[8] = 0;
            yPos[8] = 600;

            // White pawn 2
            xPos[9] = 100;
            yPos[9] = 600;

            // White pawn 3
            xPos[10] = 200;
            yPos[10] = 600;

            // White pawn 4
            xPos[11] = 300;
            yPos[11] = 600;

            // White pawn 5
            xPos[12] = 400;
            yPos[12] = 600;

            // White pawn 6
            xPos[13] = 500;
            yPos[13] = 600;

            // White pawn 7
            xPos[14] = 600;
            yPos[14] = 600;

            // White pawn 8
            xPos[15] = 700;
            yPos[15] = 600;

            // Black king
            xPos[16] = 400;
            yPos[16] = 0;

            // Black queen
            xPos[17] = 300;
            yPos[17] = 0;

            // Black bishop 1
            xPos[18] = 200;
            yPos[18] = 0;

            // Black bishop 2
            xPos[19] = 500;
            yPos[19] = 0;

            // Black knight 1
            xPos[20] = 100;
            yPos[20] = 0;

            // Black knight 1
            xPos[21] = 600;
            yPos[21] = 0;

            // White rook 1
            xPos[22] = 0;
            yPos[22] = 0;

            // White rook 1
            xPos[23] = 700;
            yPos[23] = 0;

            // Black pawn 1
            xPos[24] = 0;
            yPos[24] = 100;

            // Black pawn 2
            xPos[25] = 100;
            yPos[25] = 100;

            // Black pawn 3
            xPos[26] = 200;
            yPos[26] = 100;

            // Black pawn 4
            xPos[27] = 300;
            yPos[27] = 100;

            // Black pawn 5
            xPos[28] = 400;
            yPos[28] = 100;

            // Black pawn 6
            xPos[29] = 500;
            yPos[29] = 100;

            // Black pawn 7
            xPos[30] = 600;
            yPos[30] = 100;

            // Black pawn 8
            xPos[31] = 700;
            yPos[31] = 100;
        }

        void drawFrame(SDL_Renderer *renderer, SDL_Window *window)
        {
            // Draw tiles
            SDL_Rect tile;
            tile.w = 100;
            tile.h = 100;

            int x = 0;
            int y = 0;
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    tile.x = x;
                    tile.y = y;
                    if(tiles[i][j] == 0)
                    {
                        SDL_SetRenderDrawColor(renderer, 118, 150, 86, 0);//160, 116, 97, 0); // #A07461 - 160, 116, 97, 0 (DARK TILE)
                        SDL_RenderFillRect(renderer, &tile);
                    }
                    else
                    {
                        SDL_SetRenderDrawColor(renderer, 238, 238, 210, 0);//226, 198, 174, 0); // #E2C6AE - 226, 198, 174, 0 (LIGHT TILE)
                        SDL_RenderFillRect(renderer, &tile);
                    }
                    x += 100;
                }
                y += 100;
                x = 0;
            }

            // Draw pieces
            SDL_Rect piece[32];
            for(int i = 0; i < 32; i++)
            {
                piece[i].x = xPos[i];
                piece[i].y = yPos[i];
                piece[i].w = 100;
                piece[i].h = 100;
            }

            IMG_Init(IMG_INIT_PNG);

            // White king
            SDL_Surface* surface = IMG_Load("../assets/pieces/w_king.png"); 
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[0]/100][xPos[0]/100] = 0;
            SDL_RenderCopy(renderer, texture, NULL, &piece[0]);

            // White queen
            surface = IMG_Load("../assets/pieces/w_queen.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[1]/100][xPos[1]/100] = 1;
            SDL_RenderCopy(renderer, texture, NULL, &piece[1]);

            // White bishop 1
            surface = IMG_Load("../assets/pieces/w_bishop.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[2]/100][xPos[2]/100] = 2;
            SDL_RenderCopy(renderer, texture, NULL, &piece[2]);

            // White bishop 2
            surface = IMG_Load("../assets/pieces/w_bishop.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[3]/100][xPos[3]/100] = 3;
            SDL_RenderCopy(renderer, texture, NULL, &piece[3]);

            // White knight 1
            surface = IMG_Load("../assets/pieces/w_knight.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[4]/100][xPos[4]/100] = 4;
            SDL_RenderCopy(renderer, texture, NULL, &piece[4]);

            // White knight 2
            surface = IMG_Load("../assets/pieces/w_knight.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[5]/100][xPos[5]/100] = 5;
            SDL_RenderCopy(renderer, texture, NULL, &piece[5]);

            // White rook 1
            surface = IMG_Load("../assets/pieces/w_rook.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[6]/100][xPos[6]/100] = 6;
            SDL_RenderCopy(renderer, texture, NULL, &piece[6]);

            // White rook 2
            surface = IMG_Load("../assets/pieces/w_rook.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[7]/100][xPos[7]/100] = 7;
            SDL_RenderCopy(renderer, texture, NULL, &piece[7]);

            // White pawn 1
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[8]/100][xPos[8]/100] = 8;
            SDL_RenderCopy(renderer, texture, NULL, &piece[8]);

            // White pawn 2
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[9]/100][xPos[9]/100] = 9;
            SDL_RenderCopy(renderer, texture, NULL, &piece[9]);

            // White pawn 3
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[10]/100][xPos[10]/100] = 10;
            SDL_RenderCopy(renderer, texture, NULL, &piece[10]);

            // White pawn 4
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[11]/100][xPos[11]/100] = 11;
            SDL_RenderCopy(renderer, texture, NULL, &piece[11]);

            // White pawn 5
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[12]/100][xPos[12]/100] = 12;
            SDL_RenderCopy(renderer, texture, NULL, &piece[12]);

            // White pawn 6
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[13]/100][xPos[13]/100] = 13;
            SDL_RenderCopy(renderer, texture, NULL, &piece[13]);

            // White pawn 7
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[14]/100][xPos[14]/100] = 14;
            SDL_RenderCopy(renderer, texture, NULL, &piece[14]);

            // White pawn 8
            surface = IMG_Load("../assets/pieces/w_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[15]/100][xPos[15]/100] = 15;
            SDL_RenderCopy(renderer, texture, NULL, &piece[15]);

            // Black king
            surface = IMG_Load("../assets/pieces/b_king.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[16]/100][xPos[16]/100] = 16;
            SDL_RenderCopy(renderer, texture, NULL, &piece[16]);

            // Black queen
            surface = IMG_Load("../assets/pieces/b_queen.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[17]/100][xPos[17]/100] = 17;
            SDL_RenderCopy(renderer, texture, NULL, &piece[17]);

            // Black bishop 1
            surface = IMG_Load("../assets/pieces/b_bishop.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[18]/100][xPos[18]/100] = 18;
            SDL_RenderCopy(renderer, texture, NULL, &piece[18]);

            // Black bishop 2
            surface = IMG_Load("../assets/pieces/b_bishop.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[19]/100][xPos[19]/100] = 19;
            SDL_RenderCopy(renderer, texture, NULL, &piece[19]);

            // Black knight 1
            surface = IMG_Load("../assets/pieces/b_knight.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[20]/100][xPos[20]/100] = 20;
            SDL_RenderCopy(renderer, texture, NULL, &piece[20]);

            // Black knight 2
            surface = IMG_Load("../assets/pieces/b_knight.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[21]/100][xPos[21]/100] = 21;
            SDL_RenderCopy(renderer, texture, NULL, &piece[21]);

            // Black rook 1
            surface = IMG_Load("../assets/pieces/b_rook.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[22]/100][xPos[22]/100] = 22;
            SDL_RenderCopy(renderer, texture, NULL, &piece[22]);

            // Black rook 2
            surface = IMG_Load("../assets/pieces/b_rook.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[23]/100][xPos[23]/100] = 23;
            SDL_RenderCopy(renderer, texture, NULL, &piece[23]);

            // Black pawn 1
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[24]/100][xPos[24]/100] = 24;
            SDL_RenderCopy(renderer, texture, NULL, &piece[24]);

            // Black pawn 2
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[25]/100][xPos[25]/100] = 25;
            SDL_RenderCopy(renderer, texture, NULL, &piece[25]);

            // Black pawn 3
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[26]/100][xPos[26]/100] = 26;
            SDL_RenderCopy(renderer, texture, NULL, &piece[26]);

            // Black pawn 4
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[27]/100][xPos[27]/100] = 27;
            SDL_RenderCopy(renderer, texture, NULL, &piece[27]);

            // Black pawn 5
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[28]/100][xPos[28]/100] = 28;
            SDL_RenderCopy(renderer, texture, NULL, &piece[28]);

            // Black pawn 6
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[29]/100][xPos[29]/100] = 29;
            SDL_RenderCopy(renderer, texture, NULL, &piece[29]);

            // Black pawn 7
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[30]/100][xPos[30]/100] = 30;
            SDL_RenderCopy(renderer, texture, NULL, &piece[30]);

            // Black pawn 8
            surface = IMG_Load("../assets/pieces/b_pawn.png"); 
            texture = SDL_CreateTextureFromSurface(renderer, surface); 
            SDL_FreeSurface(surface);
            board[yPos[31]/100][xPos[31]/100] = 31;
            SDL_RenderCopy(renderer, texture, NULL, &piece[31]);

            resetLegalMoves();

            // Draw frame on the screen
            SDL_RenderPresent(renderer);
        }

        // Reset legal_moves matrix
        void resetLegalMoves()
        {
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    legal_moves[i][j] = 0;
                    //std::cout << legal_moves[i][j] << " ";
                }
                //std::cout << std::endl;
            }
        }
        // Reset other_side_moves and current_side_moves matrices
        void resetMoves()
        {
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    other_side_moves[i][j] = 0;
                    current_side_moves[i][j] = 0;
                    //std::cout << check_mate[i][j] << " ";
                }
                //std::cout << std::endl;
            }
        }
};

/*
0 - w_king
1 - w_queen
2 - w_bishop_1
3 - w_bishop_2
4 - w_knight_1
5 - w_knight_2
6 - w_rook_1
7 - w_rook_2
8 - w_pawn_1
.
.
.
15 - w_pawn_8


16 - b_king
17 - b_queen
18 - b_bishop_1
19 - b_bishop_2
20 - b_knight_1
21 - b_knight_2
22 - b_rook_1
23 - b_rook_2
24 - b_pawn_1
.
.
.
31 - b_pawn_8
*/
