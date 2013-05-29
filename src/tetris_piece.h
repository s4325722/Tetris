//
//  tetris_piece.h
//  Tetris
//
//  Created by Blake on 28/05/13.
//
//

#ifndef Tetris_tetris_piece_h
#define Tetris_tetris_piece_h

#include <stdint.h>
#include "canvas.h"

#define TETRIS_PIECE_DIMENSION 3

typedef struct tetris_piece tetris_piece;

typedef enum TETRIS_PIECE_TYPE {
    Base,
    Any
} TETRIS_PIECE_TYPE;

struct tetris_piece {
    TETRIS_PIECE_TYPE type;
    uint8_t width;
    uint8_t height;
    char* value;
};

tetris_piece* TETRIS_PIECE_BASE;

tetris_piece* TETRIS_PIECE_ANY;

canvas_element* tetris_piece_to_element(tetris_piece* pTetrisPiece);

#endif
