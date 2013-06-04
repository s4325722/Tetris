//
//  tetris_hud.c
//  Tetris
//
//  Created by Blake on 31/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include "tetris_hud.h"
#include "serialio.h"
#include "tetris_piece.h"

#ifdef AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#else
/* For non AVR build, we remove our interrupt enable */
#define cli(arg)
#define sei(arg)
#endif

void hud_initialize(tetris_hud* pHud);


tetris_hud* tetris_hud_create(tetris_game* pGame){
    tetris_hud* pHud = calloc(1, sizeof(tetris_hud));
    pHud->game = pGame;
    pHud->canvas = canvas_create(8, 8);
    
    char* pBackground = calloc(8*8, sizeof(char));
    //char* pLevelText = "01";
    //canvas_element* pLevelElement = canvas_element_create(2, 1, pLevelText);
    //canvas_element_add(pHud->canvas, pLevelElement);
    pHud->background = canvas_element_create(8, 8, pBackground);
    
    canvas_element_add(pHud->canvas, pHud->background);
    
    pHud->updated = 1;
    return pHud;
}

void tetris_hud_display(tetris_hud* pHud){
    static uint8_t hud_initialized = 0;
    
    if(!hud_initialized){
        clear_terminal();
        hud_initialize(pHud);
        hud_initialized = 1;
    }

    //cli();
    if(pHud->game->command != CMD_NONE){
        hud_initialize(pHud);
        //move_cursor(0, 14);
        clear_to_end_of_line();
        printf(" ");
    }
    
    
    if(pHud->game->hud_updated){
        pHud->game->hud_updated = 0;
        
        canvas* pCanvas = pHud->canvas;
        char (*pCanvasValue)[pCanvas->width] = (char(*)[pCanvas->width])pCanvas->value;
        uint8_t width = pCanvas->width;
        uint8_t height = pCanvas->height;
        int startX = 4;
        int startY = 4;
        
        if(pHud->game->next_element != NULL){
            canvas_element* pNextPieceElement = canvas_element_copy(pHud->game->next_element);
            canvas_element_add(pHud->canvas, pNextPieceElement);
            
            pNextPieceElement->position.x -= tetris_element_edge(pNextPieceElement, SIDE_LEFT);
            pNextPieceElement->position.y -=  tetris_element_edge(pNextPieceElement, SIDE_TOP);
            
            canvas_render(pHud->canvas);
            canvas_element_remove(pHud->canvas, pNextPieceElement);
            canvas_element_free(pNextPieceElement);
        }
        
        uint8_t video_reversed = 0;
        
        for(int i = 0; i < height; i++){
            move_cursor(startX, startY + i);
            
            for(int j = 0; j < width; j++){
                if(pCanvasValue[i][j] != '\0'){
                    if(!video_reversed){
                        set_display_attribute(46);
                        video_reversed = 1;
                    }
                    
                    printf("  ");
                }else{
                    if(video_reversed){
                        set_display_attribute(49);
                        video_reversed = 0;
                    }
                    
                    printf("  ");
                }
            }
        }
        
        move_cursor(0, 14);
    }
    //sei();
}

void hud_initialize(tetris_hud* pHud){
    canvas* pCanvas = pHud->canvas;
    uint8_t width = pCanvas->width;
    uint8_t height = pCanvas->height;
    int startX = 4;
    int startY = 4;
    uint8_t display_width = width * 2;
    uint8_t display_height = height;
    uint8_t padding = 0;
    
    draw_horizontal_line(startY - 1 - padding, startX - 2 - padding, startX + display_width + 1 + padding);
    draw_horizontal_line(startY + display_height + padding, startX - 2 - padding, startX + display_width + 1 + padding);
    draw_vertical_line(startX - 1 - padding, startY - padding, startY + display_height + padding);
    draw_vertical_line(startX - 2 - padding, startY - padding, startY + display_height + padding); // Double up on these lines for narrow chars
    draw_vertical_line(startX + display_width + padding, startY - padding, startY + display_height + padding);
    draw_vertical_line(startX + display_width + 1 + padding, startY - padding, startY + display_height + padding); // Double up on these lines for narrow chars
    
    move_cursor(0, 14);
    printf(" ");
}

void tetris_hud_display_(tetris_hud* pHud){
    
    clear_terminal();
    
    canvas* pCanvas = pHud->canvas;
    char (*pCanvasValue)[pCanvas->width] = (char(*)[pCanvas->width])pCanvas->value;
    uint8_t width = pCanvas->width;
    uint8_t height = pCanvas->height;
    int startX = 4;
    int startY = 4;
    uint8_t display_width = width * 2;
    uint8_t display_height = height;
    uint8_t padding = 0;
    
    draw_horizontal_line(startY - 1 - padding, startX - 2 - padding, startX + display_width + 1 + padding);
    draw_horizontal_line(startY + display_height + padding, startX - 2 - padding, startX + display_width + 1 + padding);
    draw_vertical_line(startX - 1 - padding, startY - padding, startY + display_height + padding);
    draw_vertical_line(startX - 2 - padding, startY - padding, startY + display_height + padding); // Double up on these lines for narrow chars
    draw_vertical_line(startX + display_width + padding, startY - padding, startY + display_height + padding);
    draw_vertical_line(startX + display_width + 1 + padding, startY - padding, startY + display_height + padding); // Double up on these lines for narrow chars
    
    if(pHud->game->hud_updated){
        pHud->game->hud_updated = 0;
        
        if(pHud->game->next_element != NULL){
            canvas_element* pNextPieceElement = canvas_element_copy(pHud->game->next_element);
            canvas_element_add(pHud->canvas, pNextPieceElement);
            
            pNextPieceElement->position.x -= tetris_element_edge(pNextPieceElement, SIDE_LEFT);
            pNextPieceElement->position.y -=  tetris_element_edge(pNextPieceElement, SIDE_TOP);
            
            canvas_render(pHud->canvas);
            canvas_element_remove(pHud->canvas, pNextPieceElement);
            canvas_element_free(pNextPieceElement);
        }
    }
    
    uint8_t video_reversed = 0;
    
    for(int i = 0; i < height; i++){
        move_cursor(startX, startY + i);
        
        for(int j = 0; j < width; j++){
            if(pCanvasValue[i][j] != '\0'){
                if(!video_reversed){
                    set_display_attribute(46);
                    video_reversed = 1;
                }
                
                printf("  ");
            }else{
                if(video_reversed){
                    set_display_attribute(49);
                    video_reversed = 0;
                }
                
                printf("  ");
            }
        }
    }
}