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

tetris_game_state* tetris_state_lose(tetris_game* pGame){
    printf("You've lost...\n");
    if(pGame->command != CMD_NONE){
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