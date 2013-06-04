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
    
    if(pGame->level.score / pGame->settings.level_step > pGame->level.score){
        pGame->level.number++;
        pGame->level.speed_multiplier += 0.1F;
        pGame->level.score_multiplier += 0.5F;
    }
    
    return game_state[(TETRIS_STATE_TYPE)Play];
}
