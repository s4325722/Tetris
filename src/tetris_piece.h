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

char* TETRIS_PIECE_PLUS;

struct canvas_element* tetris_piece_to_canvas_element(char* pTetrisPiece);

#endif
