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

#define TETRIS_PIECE_COLOURISE 1;
#define TETRIS_PIECE_DIMENSION 3

typedef struct tetris_piece tetris_piece;

typedef enum TETRIS_PIECE_TYPE {
    PIECE_TYPE_NONE,
    PIECE_TYPE_BASE,
    PIECE_TYPE_DEFAULT
} TETRIS_PIECE_TYPE;

typedef enum TETRIS_PIECE_SIDE {
    SIDE_TOP,
    SIDE_BOTTOM,
    SIDE_LEFT,
    SIDE_RIGHT
} TETRIS_PIECE_SIDE;

typedef enum TETRIS_PIECE TETRIS_PIECE;

struct tetris_piece {
    TETRIS_PIECE_TYPE type;
    uint8_t width;
    uint8_t height;
    char* value;
};

tetris_piece* TETRIS_GLYPH_PAUSE;
tetris_piece* TETRIS_PIECE_BASE;

tetris_piece* TETRIS_PIECES[6];

canvas_element* tetris_glyph_to_element(tetris_piece* pTetrisPiece);
int tetris_element_edge(canvas_element* pPieceElement, TETRIS_PIECE_SIDE side);

#endif
