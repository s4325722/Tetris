//
//  tetris.h
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdint.h>
#include "canvas.h"

#ifndef Tetris_tetris_h
#define Tetris_tetris_h

struct tetris_t;
struct tetris_piece;

void tetris_init();
void tetris_run();
void tetris_display();

typedef struct tetris_t {
    struct canvas* canvas;
} tetris_t;


typedef enum tetris_element_type {
    Background,
    Piece
} tetris_element_type;




#endif
