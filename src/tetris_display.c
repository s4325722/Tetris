//
//  tetris_display_terminal.c
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#include <stdio.h>
#include "tetris.h"
#include "canvas.h"
#include "terminalio.h"

#ifndef AVR

uint8_t colour_for_vt100(char value){
    //    41	Red
    //    42	Green
    //    43	Yellow
    //    44	Blue
    //    45	Magenta
    //    46	Cyan
    
    return value / 21 + 41;
}

void tetris_game_display(tetris_game* pGame){
    if(!pGame->updated){
        return;
    }else{
        pGame->updated = 0;
    }
        
    
    canvas* pCanvas = pGame->canvas;
    char (*pCanvasValue)[pCanvas->width] = (char(*)[pCanvas->width])pCanvas->value;
    uint8_t width = pCanvas->width;
    uint8_t height = pCanvas->height;
    uint8_t display_width = width * 2;
    uint8_t display_height = height;
    uint8_t padding = 0;
    
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
            if(pCanvasValue[i][j] != '\0')
                set_display_attribute(colour_for_vt100(pCanvasValue[i][j]));
            
            printf(" ");
            printf(" ");
            set_display_attribute(49);
        }
    }
}

#endif