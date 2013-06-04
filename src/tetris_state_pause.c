//
//  tetris_state_pause.c
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "timer2.h"
#include "tetris.h"
#include "tetris_piece.h"
#include "tetris_input.h"

static canvas_element* pPausedElement;
static uint32_t last_beat;

tetris_game_state* tetris_state_pause(tetris_game* pGame){
    canvas_element_list* pElementList = canvas_list_first(pGame->canvas->items);
    
    if(pPausedElement == NULL){
        pPausedElement = tetris_glyph_to_element(TETRIS_GLYPH_PAUSE);
        pPausedElement->position.x = 1;
        pPausedElement->position.y = 4;

        if(pElementList->current != NULL){
            do{
                pElementList->current->visible = 0;
            }while(canvas_list_next(&pElementList) != NULL);
        }
        
        canvas_element_add(pGame->canvas, pPausedElement);
        
    }else if(pGame->command == 'a'){
        if(pElementList->current != NULL){
            do{
                pElementList->current->visible = 1;
            }while(canvas_list_next(&pElementList) != NULL);
        }
        
        canvas_element_remove(pGame->canvas, pPausedElement);
        canvas_element_free(pPausedElement);
        pPausedElement = NULL;
        
        return game_state[(TETRIS_STATE_TYPE)Play];

    }else if(get_clock_ticks() - last_beat > 500){
        last_beat = get_clock_ticks();
        pPausedElement->visible = pPausedElement->visible ? 0 : 1;
        canvas_render(pGame->canvas);
        pGame->updated = 1;
    }

    return game_state[(TETRIS_STATE_TYPE)Pause];
}