//
//  tetris_state_drop.c
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetris.h"
#include "tetris_piece.h"
#include "timer2.h"
#include "tetris_sound.h"

static uint32_t last_beat;
static uint8_t last_pulse;
static canvas_element_list* pCompletedRows = NULL;

tetris_game_state* tetris_state_drop(tetris_game* pGame){
    canvas_element* pBaseElement = pGame->base_element;
    char (*pBaseValue)[pBaseElement->width] = (char(*)[pBaseElement->width])pBaseElement->value;
    uint8_t row_size = pBaseElement->width * sizeof(char);
    
    if(pGame->command == CMD_PAUSE)
        return game_state[(TETRIS_STATE_TYPE)Pause];
    
    if(pCompletedRows == NULL){
        //printf("Clearing rows...\n");
        pCompletedRows = (canvas_element_list*)calloc(1, sizeof(canvas_element_list));
        
        for(int y = 0; y < pBaseElement->height; y++){
            for(int x = 0; x < pBaseElement->width; x++)
                if(pBaseValue[y][x] == '\0')
                    goto next_row;
            
            char* pRowCopy = malloc(row_size);
            memcpy(pRowCopy, pBaseValue[y], row_size);
            memset(pBaseValue[y], '\0', row_size);
            
            canvas_element* pRowElement = (canvas_element*)calloc(1, sizeof(canvas_element));
            pRowElement->canvas = pGame->canvas;
            pRowElement->height = 1;
            pRowElement->width = pBaseElement->width;
            pRowElement->position.x = 0;
            pRowElement->position.y = y;
            pRowElement->type = PIECE_TYPE_DEFAULT;
            pRowElement->visible = 1;
            pRowElement->value = pRowCopy;
            
            if(pCompletedRows->current == NULL){
                pCompletedRows->current = pRowElement;
            }else{
                canvas_list_append(&pCompletedRows, pRowElement);
            }
            
            canvas_element_add(pGame->canvas, pRowElement);
            
        next_row:
            continue;
        }
        
        if(pCompletedRows->current != NULL){
            canvas_render(pGame->canvas);
            pGame->updated = 1;
            last_beat = get_clock_ticks();
            last_pulse = 0;
        }else{
            canvas_list_elements_free(pCompletedRows);
            canvas_list_free(pCompletedRows);
            pCompletedRows = NULL;
            
            return game_state[(TETRIS_STATE_TYPE)Play];
        }
    }else{
        if(get_clock_ticks() - last_beat < 150)
            return NULL;
        
        last_beat = get_clock_ticks();
        canvas_element_list* pCompletedRow = canvas_list_first(pCompletedRows);
        
        if(last_pulse < 5){
            if(pCompletedRow != NULL){
                do{
                    pCompletedRow->current->visible = pCompletedRow->current->visible ? 0 : 1;
                }while(canvas_list_next(&pCompletedRow));
            }
            canvas_render(pGame->canvas);
            pGame->updated = 1;
            last_pulse++;            
        }else if(pCompletedRow != NULL){
            do{
                for(int y = pCompletedRow->current->position.y; y > 0; y--){
                    memcpy(pBaseValue[y], pBaseValue[y-1], row_size);
                }
                
                memset(pBaseValue[0], '\0', row_size);
                
                canvas_element_remove(pGame->canvas, pCompletedRow->current);
            }while(canvas_list_next(&pCompletedRow));
            
            uint8_t rows_cleared_count = canvas_list_count(pCompletedRows);
            canvas_list_elements_free(pCompletedRows);
            canvas_list_free(pCompletedRows);
            pCompletedRows = NULL;
            canvas_render(pGame->canvas);
            pGame->updated = 1;
            
            
            pGame->level.score += pGame->level.score_multiplier * rows_cleared_count;
            pGame->level.cleared += rows_cleared_count;
            
            return game_state[(TETRIS_STATE_TYPE)Level];
        }
    }
    
    return NULL;
}
