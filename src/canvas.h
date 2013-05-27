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

struct canvas_t;
struct canvas_point_t;
struct canvas_element_t;
struct canvas_item_t;
struct canvas_element_filter_t;
struct canvas_point_value_t;

struct canvas_item_t* canvas_items_first(struct canvas_item_t* pCanvasItems);
struct canvas_item_t* canvas_items_last(struct canvas_item_t* pCanvasItems);
struct canvas_item_t* canvas_items_next(struct canvas_item_t* pCanvasItems);
void canvas_items_free(struct canvas_item_t* pCanvasItems);

struct canvas_item_t* canvas_elements_filtered(struct canvas_t* pCanvas, struct canvas_element_filter_t* pPredicate);
int canvas_element_filter_point(struct canvas_element_t* pElement, void* pState);
int canvas_element_filter_type(struct canvas_element_t* pElement, void* pState);
int canvas_element_filter_value(struct canvas_element_t* element, void* state);

typedef struct canvas_t {
    uint8_t width;
    uint8_t height;
    struct canvas_item_t* items;
} canvas_t;

typedef struct canvas_point_t {
    uint8_t x;
    uint8_t y;
} canvas_point_t;

typedef struct canvas_point_value_t {
    canvas_point_t point;
    char value;
} canvas_point_value_t;

typedef struct canvas_element_t {
    canvas_t canvas;
    canvas_point_t position;
    uint8_t width;
    uint8_t height;
    uint8_t type;
    char** element;
} canvas_element_t;

typedef struct canvas_element_filter_t {
    void* state;
    int (*predicate) (canvas_element_t* pElement, void* state);
    struct canvas_element_filter_t* next;
} canvas_element_filter_t;

typedef struct canvas_item_t {
    struct canvas_item_t* previous;
    struct canvas_item_t* next;
    canvas_element_t element;
} canvas_item_t;

#endif
