//
//  tetris_sound.h
//  Tetris
//
//  Created by Blake on 30/05/13.
//
//

#ifndef Tetris_tetris_sound_h
#define Tetris_tetris_sound_h

typedef enum tetris_sound {
    TICK,
    BEEP,
} tetris_sound;

void tetris_sound_play(tetris_sound sound);

#endif
