//
//  canvas_text.h
//  Tetris
//
//  Created by Blake on 29/05/13.
//
//

#ifndef Tetris_canvas_text_h
#define Tetris_canvas_text_h

#include "canvas.h"

typedef struct canvas_text canvas_text;

struct canvas_text {
    char* value;
};

canvas_element* canvas_create_char(char character);
canvas_element_list* canvas_create_string(char* pString);

int canvas_text_scroll(canvas_element_list* pCharacterList);



#endif
