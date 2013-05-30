//
//  tetris_state_splash.c
//  Tetris
//
//  Created by Blake on 29/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "timer2.h"
#include "tetris.h"
#include "tetris_input.h"

static char splash_screen_printed;

tetris_game_state* tetris_state_splash(tetris_game* pGame){
    if(!splash_screen_printed){
        printf("Splash screen");
        splash_screen_printed = 1;
    }
    
    if(tetris_input_read() != 0){
        splash_screen_printed = 0;
        srand(get_clock_ticks());
        return game_state[(TETRIS_STATE_TYPE)Play];
    }
    
    return NULL;
}