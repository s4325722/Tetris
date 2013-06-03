//
//  tetris_hud.h
//  Tetris
//
//  Created by Blake on 31/05/13.
//
//

#ifndef Tetris_tetris_hud_h
#define Tetris_tetris_hud_h

#include "canvas.h"

typedef struct tetris_hud tetris_hud;

struct tetris_hud {
    canvas* canvas;
    canvas_element* background;
};

#endif
