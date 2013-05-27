//
//  sprite_display.c
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "sprite_display.h"

sprite_t* board_tmp;
sprite_t* sprite;

uint8_t sprite_paused[SPRITE_HEIGHT][SPRITE_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

sprite_t* sprite_from_array(uint8_t** ppRawSprite){
    sprite_t* sprite = (sprite_t*)malloc(sizeof(sprite_t));
    
    for(int i = 0; i < SPRITE_HEIGHT; i++){
        sprite->row[i] = 0;
        
        uint8_t* row = ppRawSprite[i];
        uint8_t col = row[0];
        for(int j = 0; j < SPRITE_WIDTH; j++)
            sprite->row[i] |= ppRawSprite[i][j] << j;
    }
    
    return sprite;
}

void sprite_show(uint8_t** pRawSprite){
    board_tmp = (sprite_t*)board;
    sprite = sprite_from_array(pRawSprite);
    *board = *(uint8_t*)sprite;
}

void sprite_clear(){
    *board = *(rowtype*)board_tmp;
    free(sprite);
}
