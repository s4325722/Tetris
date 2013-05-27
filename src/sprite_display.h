//
//  sprite_display.h
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdint.h>
#include "board.h"

#ifndef Tetris_sprite_display_h
#define Tetris_sprite_display_h

#define SPRITE_WIDTH 7
#define SPRITE_HEIGHT 15

uint8_t sprite_paused[SPRITE_HEIGHT][SPRITE_WIDTH];

typedef struct sprite_t {
    uint8_t row[SPRITE_HEIGHT];
} sprite_t;

void sprite_show(uint8_t** pRawSprite);

void sprite_clear();

#endif
