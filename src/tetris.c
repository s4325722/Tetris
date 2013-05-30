//
//  tetris.c
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "tetris.h"
#include "terminalio.h"
#include "tetris_piece.h"
#include "canvas.h"
#include "tetris_input.h"

tetris_game_state* game_state[4] = {
    &(tetris_game_state){Splash, &tetris_state_splash},
    &(tetris_game_state){Play, &tetris_state_play},
    &(tetris_game_state){Pause, &tetris_state_pause},
    &(tetris_game_state){Lose, &tetris_state_lose}
};

tetris_game* tetris_game_create(){
    tetris_game* pGame = calloc(1, sizeof(tetris_game));
    float game_speed = TETRIS_GAME_SPEED;
    
    // Set up the game settings
    pGame->settings = (tetris_game_settings){
        1, // First level
        3, // Last level
    };
    
    // Set up the first level
    pGame->level = (tetris_game_level){
        pGame->settings.level_start, // The initial level
        1, // The initial score multiplier
        1  // The initial speed multiplier
    };
    
    // Set up the speed multiplier
    pGame->settings.speed = game_speed / pGame->level.speed_multiplier;
    
    // Set the initial game state (splash screen)
    pGame->state = game_state[(TETRIS_STATE_TYPE)Splash];
    
    // Set the game canvas
    pGame->canvas = canvas_create(TETRIS_GAME_WIDTH, TETRIS_GAME_HEIGHT);
    
    // Set the base piece (background)
    pGame->base_element = canvas_element_add(pGame->canvas, tetris_glyph_to_element(TETRIS_PIECE_BASE));
    
    // Set the current piece (nothing)
    pGame->current_element = NULL;
    
    // Set the command
    pGame->command = CMD_NONE;
    
    return pGame;
}

void tetris_game_destory(tetris_game* pGame){
    canvas_destroy(pGame->canvas); // This frees all elements on the canvas too
    free(pGame);
}

void tetris_game_run(tetris_game* pGame){
    pGame->command = (TETRIS_COMMAND)tetris_input_read();
    
    switch (pGame->command) {
        case CMD_PAUSE:
            if(pGame->state->type != Pause)
                pGame->command = CMD_NONE;
                pGame->state = game_state[(TETRIS_STATE_TYPE)Pause];
            break;
        default: ;
    }
    
    tetris_game_state* next = pGame->state->next(pGame);
    
    if(next != NULL)
        pGame->state = next;
}


