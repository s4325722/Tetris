//
//  tetris_piece.c
//  Tetris
//
//  Created by Blake on 28/05/13.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "canvas.h"
#include "tetris.h"
#include "tetris_piece.h"

uint8_t tetris_piece_colourise(canvas_element* pPieceElement);

char TETRIS_CHAR_SINGLE[][3] = {
    {'\0', '\0', '\0'},
    {'\0' , ' ' ,'\0'},
    {'\0', '\0', '\0'},
};

char TETRIS_CHAR_DOUBLE[][3] = {
    {'\0',  ' ', '\0'},
    {'\0' , ' ' ,'\0'},
    {'\0', '\0', '\0'},
};

char TETRIS_CHAR_TRIPLE[][3] = {
    {'\0', '\0', '\0'},
    {' ' , ' ' ,  ' '},
    {'\0', '\0', '\0'},
};

char TETRIS_CHAR_SQUARE[][2] = {
    {' ', ' '},
    {' ', ' '}
};

const char TETRIS_CHAR_T[][3] = {
    {' ' , ' ',  ' '},
    {'\0', ' ', '\0'},
    {'\0', '\0','\0'},
};

char TETRIS_CHAR_L[][3] = {
    {' ' , ' ', '\0'},
    {'\0', ' ', '\0'},
    {'\0', ' ', '\0'},
};

char TETRIS_CHAR_PLUS[][3] = {
    {'\0', ' ', '\0'},
    {' ',  ' ',  ' '},
    {'\0', ' ', '\0'}
};

char TETRIS_CHAR_PAUSE[][5] = {
    {' ',' ','\0',' ',' '},
    {' ',' ','\0',' ',' '},
    {' ',' ','\0',' ',' '},
    {' ',' ','\0',' ',' '},
    {' ',' ','\0',' ',' '},
    {' ',' ','\0',' ',' '},
    {' ',' ','\0',' ',' '}
};

char TETRIS_CHAR_BASE[TETRIS_GAME_HEIGHT][TETRIS_GAME_WIDTH] = {};

tetris_piece* TETRIS_GLYPH_PAUSE = &(tetris_piece){PIECE_TYPE_NONE, 5, 7, (char*)&TETRIS_CHAR_PAUSE};

tetris_piece* TETRIS_PIECE_BASE = &(tetris_piece){
    PIECE_TYPE_BASE,
    TETRIS_GAME_WIDTH,
    TETRIS_GAME_HEIGHT,
    (char*)&TETRIS_CHAR_BASE};

tetris_piece* TETRIS_PIECES[6] = {
    &(tetris_piece){PIECE_TYPE_DEFAULT, 3, 3, (char*)&TETRIS_CHAR_PLUS},
    &(tetris_piece){PIECE_TYPE_DEFAULT, 3, 3, (char*)&TETRIS_CHAR_T},
    &(tetris_piece){PIECE_TYPE_DEFAULT, 3, 3, (char*)&TETRIS_CHAR_TRIPLE},
    &(tetris_piece){PIECE_TYPE_DEFAULT, 3, 3, (char*)&TETRIS_CHAR_L},
    &(tetris_piece){PIECE_TYPE_DEFAULT, 2, 2, (char*)&TETRIS_CHAR_SQUARE},
    &(tetris_piece){PIECE_TYPE_DEFAULT, 3, 3, (char*)&TETRIS_CHAR_DOUBLE},
};

enum TETRIS_PIECE {
    Point = 0,
    Line2 = 1,
    Line3 = 2,
    Square = 3,
    T = 4,
    Plus = 5
};

int tetris_element_edge(canvas_element* pPieceElement, TETRIS_PIECE_SIDE side){
    char (*pValue)[pPieceElement->width] = (char(*)[pPieceElement->width])pPieceElement->value;
    int edge = -1;

    for(int y = 0; y < pPieceElement->width; y++){
        for(int x = 0; x < pPieceElement->width; x++){
            if(pValue[y][x] != '\0'){
                switch(side){
                    case SIDE_TOP:
                        edge = y;
                        goto found;
                    case SIDE_LEFT:
                        edge = x;
                        goto found;
                    case SIDE_RIGHT:
                        if(x > edge)
                            edge = x;
                        break;
                    case SIDE_BOTTOM:
                        if(y > edge)
                            edge = y;
                        break;
                }
            }
        }
    }
    
found:
    return edge;
}

canvas_element* tetris_glyph_to_element(tetris_piece* pTetrisPiece){
    int size = pTetrisPiece->width * pTetrisPiece->height * (int)sizeof(char);
    canvas_element* pPieceElement = (canvas_element*)calloc(1, sizeof(canvas_element));
    pPieceElement->width = pTetrisPiece->width;
    pPieceElement->height = pTetrisPiece->height;
    pPieceElement->type = pTetrisPiece->type;
    pPieceElement->position.x = 0;
    pPieceElement->position.y = 0;
    pPieceElement->visible = 1;
    pPieceElement->value = malloc(size);
    memcpy(pPieceElement->value, pTetrisPiece->value, size);
    
#ifndef AVR
    char colourise = TETRIS_PIECE_COLOURISE;
    if(colourise)
        tetris_piece_colourise(pPieceElement);
#endif
    
    return pPieceElement;
}

uint8_t tetris_piece_colourise(canvas_element* pPieceElement){
    char colour = 1 + ((double)rand() / (double)RAND_MAX) * 126;
    char (*pValue)[pPieceElement->width] = (char(*)[pPieceElement->width])pPieceElement->value;
    
    for(int y = 0; y < pPieceElement->height; y++)
        for(int x = 0; x < pPieceElement->width; x++)
            if(pValue[y][x] == ' ')
                pValue[y][x] = colour;
    
    return colour;
}