//
//  tetris.h
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdint.h>
#include "canvas.h"

#ifndef Tetris_tetris_h
#define Tetris_tetris_h

#define TETRIS_GAME_WIDTH 7
#define TETRIS_GAME_HEIGHT 15

struct tetris_t;
typedef struct tetris_game tetris_game;
struct tetris_piece;
typedef struct tetris_game_state tetris_game_state;
typedef struct tetris_game_settings tetris_game_settings;
typedef struct tetris_game_level tetris_game_level;

void tetris_init();
void tetris_run();
void tetris_display();

typedef enum TETRIS_STATE_TYPE {
    Splash,
    Play,
    Pause
} TETRIS_STATE_TYPE;

typedef enum TETRIS_MOVE_DIRECTION {
    LEFT,
    RIGHT,
    DOWN
} TETRIS_MOVE_DIRECTION;

typedef struct tetris_t {
    struct canvas* canvas;
} tetris_t;

struct tetris_game_level {
    uint8_t number;
    // The speed at which pieces are dropped (in milliseconds)
    uint8_t speed_multiplier;
    uint8_t score_multiplier;
};

struct tetris_game_settings {
    uint8_t level_start;
    uint8_t level_end;
    uint8_t level_score_multiplier;
    uint8_t level_speed_multiplier;
};

struct tetris_game_state {
    TETRIS_STATE_TYPE type;
    tetris_game_state* (*next) (tetris_game* pGame);
};

struct tetris_game {
    tetris_game_settings settings;
    tetris_game_level level;
    tetris_game_state state;
    canvas* canvas;
    canvas_element* base_element;
    canvas_element* current_element;
};

typedef enum tetris_element_type {
    Background,
    Piece
} tetris_element_type;




#endif
