//
//  tetris_state_level.c
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#include <stdio.h>
#include "tetris.h"

tetris_game_state* tetris_state_level(tetris_game* pGame){
    printf("Level");
    return game_state[(TETRIS_STATE_TYPE)Play];
}
