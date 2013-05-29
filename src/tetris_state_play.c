//
//  tetris_state_play.c
//  Tetris
//
//  Created by Blake on 29/05/13.
//
//

#include <stdio.h>
#include "timer2.h"
#include "tetris_input.h"
#include "tetris_state_play.h"

static uint32_t ticks;
static uint32_t ticks_last_dropped;

tetris_game_state* tetris_state_play(tetris_game* pGame){
    ticks = get_clock_ticks();
    // set up speed here;
    uint16_t speed = 1000;
    
    if(ticks < ticks_last_dropped + speed)
        return NULL;
        
    if(pGame->current_element != NULL){
        
    }else{
        
    }
    
    return NULL;
}

int tetris_move_piece(canvas_element* pPieceElement, TETRIS_MOVE_DIRECTION direction){
    canvas_point position = (canvas_point){pPieceElement->position.x, pPieceElement->position.y};
    
    switch(direction){
        case LEFT:
            position.x--;
            break;
        case RIGHT:
            position.x++;
            pPieceElement->position.x++;
            break;
        case DOWN:
            position.y++;
            pPieceElement->position.y++;
            break;
    }
    
    if(tetris_collision_test(pPieceElement, &position) == 0){
        pPieceElement->position.x = position.x;
        pPieceElement->position.y = position.y;
    }
    
    return -1;
}

int tetris_collision_test(canvas_element* pPieceElement, canvas_point* pPosition){
    canvas* pCanvas = pPieceElement->canvas;
    canvas_point_value point_value = (canvas_point_value){*pPosition, ' '};
    int collisions = 0;
    
    for(int y = pPosition->y; y < pPieceElement->height ;y++){
        for(int x = pPosition->x; x < pPieceElement->width; x++){
            canvas_element_list* pElements = canvas_elements_value_at_point(pCanvas, &point_value);
            collisions += canvas_list_count(pElements);
            canvas_list_free(pElements);
        }
    }
    
    return collisions;
}