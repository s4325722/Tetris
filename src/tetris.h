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
#define TETRIS_GAME_SPEED 1000;

struct tetris_t;
typedef struct tetris_game tetris_game;
typedef struct tetris_game_state tetris_game_state;
struct tetris_piece;

typedef struct tetris_game_settings tetris_game_settings;
typedef struct tetris_game_level tetris_game_level;

tetris_game* tetris_game_create();
void tetris_game_run(tetris_game* pGame);
void tetris_game_display(tetris_game* pGame);
void tetris_game_destroy(tetris_game* pGame);

typedef enum TETRIS_COMMAND {
    CMD_NONE = 0,
    CMD_ROTATE = 'A',
    CMD_MOVE_LEFT = '0',
    CMD_MOVE_RIGHT = 'D',
    CMD_MOVE_DOWN = '1',
    CMD_PAUSE = 'a'
} TETRIS_COMMAND;

//typedef struct tetris_t {
//    struct canvas* canvas;
//} tetris_t;

struct tetris_game_level {
    uint8_t number;
    // The speed at which pieces are dropped (in milliseconds)
    float score_multiplier;
    float speed_multiplier;
};

struct tetris_game_settings {
    uint8_t level_start;
    uint8_t level_end;
    uint16_t speed;
    uint32_t seed;
};

typedef enum TETRIS_STATE_TYPE {
    Splash = 0,
    Play = 1,
    Pause = 2,
    Lose = 3,
} TETRIS_STATE_TYPE;

struct tetris_game_state {
    TETRIS_STATE_TYPE type;
    tetris_game_state* (*next) (tetris_game* pGame);
};

struct tetris_game {
    tetris_game_settings settings;
    tetris_game_level level;
    tetris_game_state* state;
    canvas* canvas;
    canvas_element* base_element;
    canvas_element* current_element;
    TETRIS_COMMAND command;
    char updated;
};

typedef enum tetris_element_type {
    Background,
    Piece
} tetris_element_type;

tetris_game_state* tetris_state_splash(tetris_game* pGame);
tetris_game_state* tetris_state_play(tetris_game* pGame);
tetris_game_state* tetris_state_level(tetris_game* pGame);
tetris_game_state* tetris_state_lose(tetris_game* pGame);
tetris_game_state* tetris_state_pause(tetris_game* pGame);

tetris_game_state* game_state[4];

#endif