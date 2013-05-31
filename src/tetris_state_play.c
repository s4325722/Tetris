//
//  tetris_state_play.c
//  Tetris
//
//  Created by Blake on 29/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "timer2.h"
#include "canvas.h"
#include "tetris.h"
#include "tetris_piece.h"
#include "tetris_input.h"
#include "tetris_sound.h"

volatile uint32_t ticks;
volatile uint32_t ticks_last_dropped;

typedef enum movement_direction {
    MOVE_NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN
} movement_direction;

typedef enum movement_result {
    COLLIDED_NONE,
    COLLIDED_EDGE,
    COLLIDED_PIECE,
    COLLIDED_BASE
} movement_result;

typedef struct collision_test_state {
    canvas_point* position;
    canvas_element* search;
} collision_test_state;

movement_result try_move_piece(canvas_element* pPieceElement, movement_direction direction);
movement_result collision_test(canvas_element* pPieceElement, canvas_point* pPosition);
int collision_test_predicate(canvas_element* pElement, void* pState);
void merge_piece(canvas_element* pSrcElement, canvas_element* pDstElement);
movement_result rotate_piece_(canvas_element* pPieceElement);


tetris_game_state* tetris_state_play(tetris_game* pGame){
    ticks = get_clock_ticks();
    char is_canvas_dirty = 0;
    char is_game_over = 0;
    
    if(pGame->command == CMD_PAUSE){
        return game_state[(TETRIS_STATE_TYPE)Pause];
    }
    
    if(pGame->current_element != NULL){
        movement_direction movement = MOVE_NONE;
            
        switch(pGame->command){
            case CMD_ROTATE:
                if(rotate_piece_(pGame->current_element) == COLLIDED_NONE){
                    is_canvas_dirty = 1;
                }else{
                    tetris_sound_play(BEEP);
                }
                break;
            case CMD_MOVE_LEFT:
                movement = MOVE_LEFT;
                break;
            case CMD_MOVE_RIGHT:
                movement = MOVE_RIGHT;
                break;
            case CMD_MOVE_DOWN:
                movement = MOVE_DOWN;
                ticks_last_dropped = get_clock_ticks();
                break;
            case CMD_PLUMMET:
                movement = MOVE_DOWN;
                while(try_move_piece(pGame->current_element, MOVE_DOWN) == COLLIDED_NONE)
                    continue;
                break;
            default:
                // If we're not doing anything else, we should be dropping the piece
                if(ticks >= ticks_last_dropped + pGame->settings.speed){
                    movement = MOVE_DOWN;
                    ticks_last_dropped = get_clock_ticks();
                }
        }

        if(movement != MOVE_NONE){
            switch(try_move_piece(pGame->current_element, movement)){
                case COLLIDED_NONE:
                    is_canvas_dirty = 1;
                    printf("dropped one... [%d,%d]\n", pGame->current_element->position.x, pGame->current_element->position.y);
                    break;
                case COLLIDED_EDGE:
                case COLLIDED_PIECE:
                    //is_canvas_dirty = 1;
                    tetris_sound_play(BEEP);
                    break;
                case COLLIDED_BASE:
                    if(movement != MOVE_DOWN){
                        tetris_sound_play(BEEP);
                    }else{
                        is_canvas_dirty = 1;
                        // Special case, the element is at the top of the board
                        if(pGame->current_element->position.y == 0){
                            is_game_over = 1;
                        }
                        
                        // Copy to base element
                        merge_piece(pGame->current_element, pGame->base_element);
                        // Then clean up
                        canvas_element_remove(pGame->canvas, pGame->current_element);
                        canvas_element_free(pGame->current_element);
                        //free(pGame->current_element);
                        pGame->current_element = NULL;
                        return game_state[(TETRIS_STATE_TYPE)Drop];
                    }
                    break;
            }
        }
    }else{
        int pieces_count = sizeof(TETRIS_PIECES) / sizeof(tetris_piece*);
        int random_piece_index = (int)(((double)rand() / (double)RAND_MAX) * (pieces_count - 1));
        //int random_piece_index = 0;
        canvas_element* pNewEelement = tetris_glyph_to_element(TETRIS_PIECES[random_piece_index]);
        pGame->current_element = canvas_element_add(pGame->canvas, pNewEelement);
        ticks_last_dropped = get_clock_ticks();
        is_canvas_dirty = 1;
        
        if(collision_test(pGame->current_element, &pGame->current_element->position) != COLLIDED_NONE){
            is_game_over = 1;
        }
    }
    
    if(is_canvas_dirty){
        canvas_render(pGame->canvas);
        pGame->updated = 1;
    }
    
    if(is_game_over){
        return game_state[(TETRIS_STATE_TYPE)Lose];
    }
    
    return NULL;
}

movement_result try_move_piece(canvas_element* pPieceElement, movement_direction direction){
    canvas_point position = (canvas_point){pPieceElement->position.x, pPieceElement->position.y};
    
    // Test to make sure it's not on an edge first.
    switch(direction){
        case MOVE_LEFT:
            if(position.x - 1 + tetris_element_edge(pPieceElement, SIDE_LEFT) < 0)
                return COLLIDED_EDGE;
            position.x--;
            break;
        case MOVE_RIGHT:
            if(position.x + 1 + tetris_element_edge(pPieceElement, SIDE_RIGHT) >= pPieceElement->canvas->width)
                return COLLIDED_EDGE;
            position.x++;
            break;
        case MOVE_DOWN:
            if(position.y + tetris_element_edge(pPieceElement, SIDE_BOTTOM) >= pPieceElement->canvas->height - 1)
                return COLLIDED_BASE;
            position.y++;
            break;
        default: ;
    }
    
    // Do collision test
    movement_result result = collision_test(pPieceElement, &position);
    
    if(result == COLLIDED_NONE){
        pPieceElement->position.x = position.x;
        pPieceElement->position.y = position.y;
    }

    return result;
}

movement_result collision_test(canvas_element* pPieceElement, canvas_point* position){
    collision_test_state test_state = (collision_test_state){position, pPieceElement};
    canvas_element_filter filter = (canvas_element_filter){
        &test_state, &collision_test_predicate, NULL
    };
    //canvas_element_filter* pFilter = calloc(1, sizeof(canvas_element_filter));
    //pFilter->next = NULL;
    //pFilter->state = &test_state;
    //pFilter->predicate = &collision_test_predicate;
    
    //canvas_element_filter* filter = &(canvas_element_filter){&collision_test_predicate, (void*)&test_state};
    
    canvas_element_list* pCollisions = canvas_elements_filtered(pPieceElement->canvas, &filter);
    movement_result result = COLLIDED_NONE;
    
    if(canvas_list_count(pCollisions)){
        result = COLLIDED_PIECE;
        
        do{
            if(pCollisions->element->type == PIECE_TYPE_BASE){
                result = COLLIDED_BASE;
                break;
            }
        }while(canvas_list_next(&pCollisions) != NULL);
    }
    
    canvas_list_free(pCollisions);
    
    return result;
}

int collision_test_predicate(canvas_element* pElement, void* pState){
    collision_test_state* pTestState = (collision_test_state*)pState;
    canvas_element* pSearchElement = pTestState->search;
    
    if(pElement == pSearchElement)
        return 0;
    
    char (*pSearchValue)[pSearchElement->width] = (char(*)[pSearchElement->width])pSearchElement->value;
    char (*pElementValue)[pElement->width] = (char(*)[pElement->width])pElement->value;
    
    for(int y = 0; y < pSearchElement->height; y++){
        uint8_t cmpY = pTestState->position->y + y;
        
        if(cmpY < pElement->position.y || cmpY > pElement->position.y + pElement->height)
            continue;
        
        for(int x = 0; x < pSearchElement->width; x++){
            uint8_t cmpX = pTestState->position->x + x;
            
            if(cmpX < pElement->position.x || cmpX > pElement->position.x + pElement->width)
                continue;
            
            if(pSearchValue[y][x] != '\0' && pElementValue[cmpY][cmpX] != '\0')
                return 1;
        }
    }
    
    return 0;
}


void merge_piece(canvas_element* pSrcElement, canvas_element* pDstElement){
    char (*pDstValue)[pDstElement->width] = (char(*)[pDstElement->width])pDstElement->value;
    char (*pSrcValue)[pSrcElement->width] = (char(*)[pSrcElement->width])pSrcElement->value;
    
    for(int i = 0; i < pSrcElement->height; i++){
        uint8_t y = pSrcElement->position.y + i;
        
        if(y >= pDstElement->height)
            y = y % (pDstElement->height);
        
        for(int j = 0; j < pSrcElement->width; j++){
            uint8_t x = pSrcElement->position.x + j;
            
            if(x >= pDstElement->width)
                x = x % (pDstElement->width);
            
            if(!pDstValue[y][x])
                pDstValue[y][x] = pSrcValue[i][j];
        }
    }
}

movement_result rotate_piece_(canvas_element* pPieceElement){
    char (*pValue)[pPieceElement->width] = (char(*)[pPieceElement->width])pPieceElement->value;
    int dimension = pPieceElement->width;
    movement_result result = COLLIDED_NONE;
    
    int layer, first, last, offset, top;
    
    for(int i = 0; i < 4; i++){
        for (layer = 0; layer < dimension / 2 ; layer++) {
            first = layer;
            last = dimension - layer - 1;
            for (int j = first; j < last ; j++) {
                offset = j - first;
                top = pValue[first][j];
                
                pValue[first][j] = pValue[last-offset][first];
                pValue[last-offset][first] = pValue[last][last-offset];
                pValue[last][last-offset] = pValue[j][last];
                pValue[j][last] = top;
            }
        }
        
        int edge_left = tetris_element_edge(pPieceElement, SIDE_LEFT);
        int edge_right = tetris_element_edge(pPieceElement, SIDE_RIGHT);
        int edge_top = tetris_element_edge(pPieceElement, SIDE_TOP);
        int edge_bottom = tetris_element_edge(pPieceElement, SIDE_BOTTOM);
        
        if(i == 0){
            result = collision_test(pPieceElement, &pPieceElement->position);
            
            if(result != COLLIDED_NONE){
                break;
            } else if (
               edge_left + pPieceElement->position.x < 0 ||
               edge_right + pPieceElement->position.x > pPieceElement->canvas->width - 1 ||
               edge_top + pPieceElement->position.y < 0 ||
               edge_bottom + pPieceElement->position.y > pPieceElement->canvas->height - 1
               ){
                result = COLLIDED_EDGE;
            }else{
                break;
            }
        }
    }
    
    return result;
}