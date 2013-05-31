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
#include "font.h"

#ifdef AVR
#include <avr/pgmspace.h>
#else
/* Non AVR - can't put constants in program memory */
#define PROGMEM
#define pgm_read_byte(addr) (*(addr))
#define pgm_read_word(addr) (*(addr))
#endif


canvas_element* canvas_create_char(char character){
    character = toupper(character);
    int offset = character;
    
    if(isdigit(character)){
        offset -= 60;
    }else if(isalpha(character)){
        offset -= 41;
    }else{
        return NULL;
    }

    canvas_element* pElement = calloc(1, sizeof(canvas_element));
    char (*pElementValue)[pElement->width] = (char(*)[pElement->width])calloc(5*7, sizeof(char));
    int k = 0;
    
    for(int i = 5; i >= 0; i--){
        uint8_t binChar = pgm_read_byte(&font_5x7[offset][i]);
        
        for(int j = 7; i >= 0; j--){
            uint8_t bit = (binChar) & (1 << j);
         
            pElementValue[k++][j] = bit ? ' ' : '\0';
        }
    }
    
    pElement->height = 7;
    pElement->width = 5;
    pElement->type = 10;
    pElement->visible = 1;
    pElement->value = (char*)pElementValue;
    pElement->position.x = 0;
    pElement->position.y = 0;
    
    return pElement;
}

canvas_element_list* canvas_create_string(char* pString){
    canvas_element_list* pElementList = calloc(1, sizeof(canvas_element_list));
    
    for(int i = 0; i < strlen(pString); i++){
        canvas_element* pCharElement = canvas_create_char(pString[i]);
        
        if(pCharElement != NULL){
            pCharElement->position.x = i * 5;
            pCharElement->position.y = 0;
            canvas_list_append(&pElementList, pCharElement);
        }
    }
    
    return pElementList;
}



int canvas_text_scroll(canvas_element_list* pCharacterList, canvas_scroll_direction direction){
    if(pCharacterList == NULL)
        return -1;
    
    canvas_element_list* pTmpList = pCharacterList;
    
    do{
        pTmpList->element->position.x += 5;
    }while(canvas_list_next(&pTmpList) != NULL);
    
    return 0;
}