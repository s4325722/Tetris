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
#include "terminalio.h"

typedef struct tetris_hud tetris_hud;

struct tetris_hud {
    tetris_game* game;
    uint8_t updated;
    canvas* canvas;
    canvas_element* background;
};

tetris_hud* tetris_hud_create(tetris_game* pGame);
void tetris_hud_display(tetris_hud* pHud);


#endif
