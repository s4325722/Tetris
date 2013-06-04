//
//  tetris_display_terminal.c
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#include <stdio.h>

#ifdef AVR
#include <avr/io.h>
#include <avr/cpufunc.h>
#else
#define _NOP()
#endif

#include "terminalio.h"
#include "canvas.h"
#include "tetris.h"


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

#ifdef AVR

void avr_display_initialize(void);

void tetris_game_display(tetris_game* pGame){
    static uint16_t avr_display_buffer[TETRIS_GAME_WIDTH];
    static uint8_t avr_display_initialized = 0;
    
    if(!avr_display_initialized){
        avr_display_initialize();
        avr_display_initialized = 1;
    }
    
    uint8_t width = pGame->canvas->width;
    uint8_t height = pGame->canvas->height;

    if(pGame->updated){
        char (*pCanvasValue)[width] = (char(*)[width])pGame->canvas->value;
        
        for(int x = 0; x < width; x++){
            uint16_t row_data = 0;
            
            for(int y = 0; y < height; y++)
                row_data |= (pCanvasValue[y][x] != '\0') << (height - 1 - y);
            
            avr_display_buffer[x] = row_data;
        }
        
        pGame->updated = 0;
    }
    
    for(int i = 0; i < width; i++){;
        PORTG = i;
        PORTA = ~(uint8_t)(avr_display_buffer[i] & 0xFF);
        PORTC = ~(uint8_t)((avr_display_buffer[i] >> 8)& 0x7F);
    }
}

void avr_display_initialize(void){
    /* Set ports A and C to be outputs (except most significant
	 * bit of port C)
	 */
	DDRA = 0xFF;
	DDRC = 0x7F;
    
	/* Set 3 least significant bits of G to be outputs */
	DDRG = 0x07;
}

#endif