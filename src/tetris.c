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

const uint8_t TETRIS_ELEMENT_BACKGROUND = 0;
const uint8_t TETRIS_ELEMENT_PIECE = 1;

//tetris_t game = {NULL};
//tetris_t* pGame = &(tetris_t){NULL};
static tetris_t* pGame = NULL;
static canvas_element_list* pTestElement = NULL;

int tetris_collision_test(canvas_element* pPieceElement, canvas_point* pPosition);
void tetris_display_terminal();
void tetris_display_led_matrix();

void tetris_init(){
    pGame = malloc(sizeof(tetris_t));
    pGame->canvas = canvas_create(7, 15);
    //*pGame = *(tetris_t*)malloc(sizeof(tetris_t));
    //pGame->canvas = canvas_create(7, 15);
    //canvas_t canvas = *canvas_create(7, 15);
    //canvas_t canvas = *pCanvas;
    //pGame->canvas = pCanvas;
    canvas_element* element = tetris_piece_to_canvas_element(TETRIS_PIECE_ANY);
    //pTestElement = canvas_element_add(pGame->canvas, element);
    pTestElement = canvas_element_add(pGame->canvas, element);
    pTestElement->element->canvas = pGame->canvas;
    
    canvas_render(pGame->canvas);
}

void tetris_run(){
    canvas canvas = *pGame->canvas;
    uint8_t x = pTestElement->element->position.x;
    uint8_t y = pTestElement->element->position.y;
    x = x >= pTestElement->element->canvas->width - 1 ? 0 : x + 1;
    y = y >= pTestElement->element->canvas->height - 1 ? 0 : y + 1;
    pTestElement->element->position.x = x;
    pTestElement->element->position.y = y;
    
    
    canvas_render(&canvas);
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

void tetris_display(){
#ifndef AVR
    tetris_display_terminal();
#endif
#ifdef AVR
    tetris_display_led_matrix();
#endif
}

void tetris_display_terminal(){
    canvas* pCanvas = pGame->canvas;
    char (*pCanvasValue)[pCanvas->width] = (char(*)[pCanvas->width])pCanvas->value;
    uint8_t width = pCanvas->width;
    uint8_t height = pCanvas->height;
    uint8_t display_width = width * 2;
    uint8_t display_height = height;
    uint8_t padding = 0;
    //char (*pCanvasValue)[canvas.height][canvas.width] = &canvas.value;
    //char (*pCanvasValue)[canvas.width] = canvas.value;
    
	int startX = 25;
	int startY = 4;
    hide_cursor();
    clear_terminal();
	draw_horizontal_line(startY - 1 - padding, startX - 2 - padding, startX + display_width + 1 + padding);
	draw_horizontal_line(startY + display_height + padding, startX - 2 - padding, startX + display_width + 1 + padding);
	draw_vertical_line(startX - 1 - padding, startY - padding, startY + display_height + padding);
    draw_vertical_line(startX - 2 - padding, startY - padding, startY + display_height + padding); // Double up on these lines for narrow chars
	draw_vertical_line(startX + display_width + padding, startY - padding, startY + display_height + padding);
    draw_vertical_line(startX + display_width + 1 + padding, startY - padding, startY + display_height + padding); // Double up on these lines for narrow chars
    for(int i = 0; i < height; i++){
        move_cursor(startX, startY + i);
        
        for(int j = 0; j < width; j++){
            if(pCanvasValue[i][j] == ' ')
                set_display_attribute(46);

            //if(pCanvas->value[(i * pCanvas->width) + j] == ' ')
           //     set_display_attribute(46);
            
            printf(" ");
            printf(" ");
            set_display_attribute(49);
        }
    }
}
