//
//  tetris_state_lose.c
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#include <stdio.h>
#include <string.h>
#include "tetris.h"
#include "tetris_piece.h"


void tetris_game_reset(tetris_game* pGame);
static uint8_t lost_printed = 0;

tetris_game_state* tetris_state_lose(tetris_game* pGame){
    if(!lost_printed){
        //printf("You've lost...\n");
        lost_printed = 1;
    }
    
    if(pGame->command != CMD_NONE){
        lost_printed = 0;
        tetris_game_reset(pGame);
        return game_state[(TETRIS_STATE_TYPE)Play];
    }

    return NULL;
}

void tetris_game_reset(tetris_game* pGame){
    int size = pGame->base_element->width * pGame->base_element->height * (int)sizeof(char);
    memcpy(pGame->base_element->value, TETRIS_PIECE_BASE->value, size);
    canvas_render(pGame->canvas);
    pGame->updated = 1;
}