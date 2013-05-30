//
//  tetris_sound.c
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#include <stdio.h>
#include "tetris_sound.h"


#ifndef AVR

void tetris_sound_play(tetris_sound sound){
    switch(sound){
        default:
            printf("\a");
    }
}

#endif

#ifdef AVR

void tetris_sound_play(tetris_sound sound){
    switch(sound){
        default:
            printf("\a");
    }
}

#endif