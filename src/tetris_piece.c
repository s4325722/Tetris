//
//  tetris_piece.c
//  Tetris
//
//  Created by Blake on 28/05/13.
//
//

#include <stdlib.h>
#include <stdio.h>
#include "tetris_piece.h"
#include "canvas.h"

char TETRIS_CHAR_PLUS[] = {
    '\0', ' ', '\0',
    ' ' , ' ', '\0',
    '\0', ' ', '\0'
};


canvas_element* tetris_piece_to_canvas_element(char* pTetrisPiece){
    canvas_element* pCanvasElement = malloc(sizeof(canvas_element));
    pCanvasElement->width = TETRIS_PIECE_DIMENSION;
    pCanvasElement->height = TETRIS_PIECE_DIMENSION;
    pCanvasElement->type = 0;
    pCanvasElement->position.x = 0;
    pCanvasElement->position.y = 0;
    pCanvasElement->value = pTetrisPiece;
    
    return pCanvasElement;
}