//
//  canvas_text.c
//  Tetris
//
//  Created by Blake on 29/05/13.
//
//

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "canvas.h"
#include "canvas_text.h"
#include "font_7x4.h"

#ifdef AVR
#include <avr/pgmspace.h>
#else
/* Non AVR - can't put constants in program memory */
#define PROGMEM
#define pgm_read_byte(addr) (*(addr))
#define pgm_read_word(addr) (*(addr))
#endif

const int CANVAS_FONT_WIDTH = 7;
const int CANVAS_FONT_HEIGHT = 4;


canvas_element* canvas_text_char_create(char character){
    character = toupper(character);
    uint8_t offset = character;
    
    if(character == ' '){
        offset = 0;
    }else if(isdigit(character)){
        offset -= 47;
    }else if(isalpha(character)){
        offset -= 54;
    }else{
        return NULL;
    }

    canvas_element* pElement = calloc(1, sizeof(canvas_element));
    char (*pElementValue)[CANVAS_FONT_WIDTH] = (char(*)[CANVAS_FONT_WIDTH])calloc(CANVAS_FONT_HEIGHT * CANVAS_FONT_WIDTH, sizeof(char));
    
    for(int x = 0; x < CANVAS_FONT_HEIGHT; x++){
        //uint8_t font_char = font_5x7[offset][CANVAS_FONT_HEIGHT - 1 - x];
        uint8_t font_char = pgm_read_byte(&font_7x4[offset * CANVAS_FONT_HEIGHT + x]);
        
        for(int y = 0; y < CANVAS_FONT_WIDTH; y++){
            pElementValue[CANVAS_FONT_HEIGHT - 1 - x][y] = (font_char & (1 << y)) ? ' ' : '\0';
        }
    }
    
    pElement->height = CANVAS_FONT_HEIGHT;
    pElement->width = CANVAS_FONT_WIDTH;
    pElement->position.x = 0;
    pElement->position.y = 0;
    pElement->type = 10;
    pElement->visible = 1;
    pElement->wrap_x = 0;
    pElement->wrap_y = 0;
    pElement->value = (char*)pElementValue;

    return pElement;
}

canvas_element_list* canvas_text_create(char* pString){
    canvas_element_list* pElementList = calloc(1, sizeof(canvas_element_list));
    
    for(int i = 0; i < strlen(pString); i++){
        canvas_element* pCharElement = canvas_text_char_create(pString[i]);
        
        if(pCharElement != NULL){
            pCharElement->position.x = 0;
            pCharElement->position.y = -1 + (i * (CANVAS_FONT_HEIGHT * -1));
            canvas_list_append(&pElementList, pCharElement);
        }
    }
    
    return canvas_list_first(pElementList);
}

void canvas_text_destroy(canvas_element_list* pStringElements){
    if(pStringElements == NULL)
        return;
    
    canvas_list_elements_free(pStringElements);
    canvas_list_free(pStringElements);
}


int canvas_text_scroll(canvas_element_list* pCharacterList, canvas_scroll_direction direction){
    if(pCharacterList == NULL)
        return -1;
    
    canvas_element_list* pTmpList = canvas_list_first(pCharacterList);
    uint8_t max_y_offset = (canvas_list_count(pCharacterList) * CANVAS_FONT_HEIGHT) - 1;
    uint8_t result = (pCharacterList->current->position.y + 1) / CANVAS_FONT_HEIGHT;
    
    do{
        if(pTmpList->current->position.y == max_y_offset){
            pTmpList->current->position.y = -1;
        }else{
            pTmpList->current->position.y += CANVAS_FONT_HEIGHT;
        }
    }while(canvas_list_next(&pTmpList) != NULL);

    return result;
}