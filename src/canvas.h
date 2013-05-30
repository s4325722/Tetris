//
//  canvas.h
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdint.h>

#ifndef Tetris_canvas_h
#define Tetris_canvas_h

typedef struct canvas canvas;
typedef struct canvas_point canvas_point;
typedef struct canvas_element canvas_element;
typedef struct canvas_element_list canvas_element_list;
typedef struct canvas_element_filter canvas_element_filter;
typedef struct canvas_point_value canvas_point_value;

canvas* canvas_create(uint8_t width, uint8_t height);
void canvas_destroy(canvas* pCanvas);
void canvas_render(canvas* pCanvas);
void canvas_render_list(canvas_element_list* pCanvasItem);
canvas_element_list* canvas_list_first(canvas_element_list* pCanvasItems);
canvas_element_list* canvas_list_last(canvas_element_list* pCanvasItems);
canvas_element_list* canvas_list_next(canvas_element_list** pCanvasItems);
int canvas_list_count(canvas_element_list* pElementList);
void canvas_list_free(canvas_element_list* pCanvasItems);
void canvas_list_elements_free(canvas_element_list* pCanvasItems);

canvas_element* canvas_element_create(canvas* pCanvas, uint8_t width, uint8_t height, char value);
void canvas_element_free(canvas_element* pElement);
canvas_element* canvas_element_add(canvas* pCanvas, canvas_element* pElement);
int canvas_element_remove(canvas* pCanvas, canvas_element* pElement);
canvas_element_list* canvas_elements_filtered(canvas* pCanvas, canvas_element_filter* pPredicate);
int canvas_element_filter_point(canvas_element* pElement, void* pState);
int canvas_element_filter_type(canvas_element* pElement, void* pState);
int canvas_element_filter_value(canvas_element* element, void* state);

canvas_element_list* canvas_elements_value_at_point(canvas* pCanvas, canvas_point_value* pPointValue);

struct canvas {
    uint8_t width;
    uint8_t height;
    canvas_element_list* items;
    void (*render) (canvas_element* pElement);
    char* value;
};

struct canvas_point {
    uint8_t x;
    uint8_t y;
};

struct canvas_point_value {
    canvas_point point;
    char value;
};

struct canvas_element {
    canvas* canvas;
    canvas_point position;
    uint8_t width;
    uint8_t height;
    uint8_t type;
    char visible;
    char* value;
};

struct canvas_element_filter {
    void* state;
    int (*predicate) (canvas_element* pElement, void* state);
    canvas_element_filter* next;
};

struct canvas_element_list {
    canvas_element_list* previous;
    canvas_element_list* next;
    canvas_element* element;
};

#endif
