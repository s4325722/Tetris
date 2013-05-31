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

typedef enum canvas_scroll_direction {
    SCROLL_LEFT,
    SCROLL_RIGHT,
    SCROLL_UP,
    SCROLL_DOWN
} canvas_scroll_direction;

canvas_element* canvas_create_char(char character);
canvas_element_list* canvas_create_string(char* pString);
int canvas_text_scroll(canvas_element_list* pCharacterList, canvas_scroll_direction direction);



#endif
