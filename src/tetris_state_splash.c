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
#include "canvas.h"
#include "canvas_text.h"

static canvas_element_list* pTickerElements = NULL;
static uint32_t last_beat;

tetris_game_state* tetris_state_splash(tetris_game* pGame){
    if(pTickerElements == NULL){
        pTickerElements = canvas_text_create(pGame->settings.ticker);
        canvas_element_list* pTickerElementsTmp = pTickerElements;
        
        do{
            canvas_element_add(pGame->canvas, pTickerElementsTmp->current);
        }while(canvas_list_next(&pTickerElementsTmp) != NULL);

        canvas_text_scroll(pTickerElements, SCROLL_DOWN);
        canvas_render(pGame->canvas);
        pGame->updated = 1;
        
    }else if(get_clock_ticks() - last_beat > 500){
        last_beat = get_clock_ticks();
        canvas_text_scroll(pTickerElements, SCROLL_DOWN);
        canvas_render(pGame->canvas);
        pGame->updated = 1;
    }else if(pGame->command != CMD_NONE){
        canvas_element_list* pTickerElementsTmp = canvas_list_first(pTickerElements);
        
        do{
            canvas_element_remove(pGame->canvas, pTickerElementsTmp->current);
        }while(canvas_list_next(&pTickerElementsTmp) != NULL);
        canvas_text_destroy(pTickerElements);
        canvas_render(pGame->canvas);
        pGame->updated = 1;
        pTickerElements = NULL;
        
        srand(get_clock_ticks());
        return game_state[(TETRIS_STATE_TYPE)Play];
    }
    
    return game_state[(TETRIS_STATE_TYPE)Splash];
}