//
//  seven_seg_display.c
//  Tetris
//
//  Created by Blake on 22/05/13.
//
//

#include <stdio.h>
#include "seven_seg_display.h" 
#ifdef AVR
#include <avr/io.h>
#endif

static uint8_t seven_seg_data[10] = {63,6,91,79,102,109,125,7,127,111};
volatile int8_t seven_seg_value = -1;

void seven_seg_set(uint8_t value){
    if(value >= 0 && value <= 99){
        seven_seg_value = value;
    }else{
        seven_seg_value = -1;
    }
}

void seven_seg_display(){
#ifdef AVR
    if(seven_seg_value != -1){
        uint8_t lower = seven_seg_value / 10;
        uint8_t upper = seven_seg_value - lower * 10;
        PORTC = seven_seg_data[lower];
        PORTC = seven_seg_data[upper];
    }else{
        PORTC = 0;
    }
#endif
}