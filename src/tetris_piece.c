//
//  tetris_piece.c
//  Tetris
//
//  Created by Blake on 28/05/13.
//
//

#include <stdlib.h>
#include <stdio.h>
#include "canvas.h"
#include "tetris.h"
#include "tetris_piece.h"

char TETRIS_CHAR_SINGLE[][3] = {
    {' '}
};

char TETRIS_CHAR_DOUBLE[][3] = {
    {' ', ' '}
};

char TETRIS_CHAR_TRIPLE[][3] = {
    {' ', ' ', ' '}
};

char TETRIS_CHAR_SQUARE[][2] = {
    {' ', ' '},
    {' ', ' '}
};

char TETRIS_CHAR_T[][3] = {
    {' ' , ' ', ' '},
    {'\0', ' ', '\0'},
    {'\0', ' ', '\0'},
};

char TETRIS_CHAR_L[][3] = {
    {' ' , ' ', ' '},
    {'\0', ' ', '\0'},
    {'\0', ' ', '\0'},
};

char TETRIS_CHAR_PLUS[][3] = {
    {'\0', ' ', '\0'},
    {' ',  ' ',  ' '},
    {'\0', ' ', '\0'}
};

char TETRIS_CHAR_BASE[TETRIS_GAME_HEIGHT][TETRIS_GAME_WIDTH] = {'\0'};

tetris_piece* TETRIS_PIECE_BASE = &(tetris_piece){Base, TETRIS_GAME_HEIGHT, TETRIS_GAME_WIDTH, (char*)&TETRIS_PIECE_BASE};
tetris_piece* TETRIS_PIECE_SINGLE = &(tetris_piece){Any, 1, 1, (char*)&TETRIS_CHAR_SINGLE};
tetris_piece* TETRIS_PIECE_DOUBLE = &(tetris_piece){Any, 1, 1, (char*)&TETRIS_CHAR_DOUBLE};
tetris_piece* TETRIS_PIECE_TRIPLE = &(tetris_piece){Any, 1, 1, (char*)&TETRIS_CHAR_TRIPLE};
tetris_piece* TETRIS_PIECE_ANY = &(tetris_piece){Any, 3, 3, (char*)&TETRIS_CHAR_PLUS};



canvas_element* tetris_piece_to_element(tetris_piece* pTetrisPiece){
    canvas_element* pCanvasElement = (canvas_element*)malloc(sizeof(canvas_element));
    pCanvasElement->width = pTetrisPiece->width;
    pCanvasElement->height = pTetrisPiece->height;
    pCanvasElement->type = pTetrisPiece->type;
    pCanvasElement->position.x = 0;
    pCanvasElement->position.y = 0;
    pCanvasElement->value = pTetrisPiece->value;
    
    return pCanvasElement;
}