//
//  canvas.c
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "canvas.h"

void canvas_render_default(canvas_element* pElement);

canvas* canvas_create(uint8_t width, uint8_t height){
    canvas* pCanvas = (canvas*)calloc(1, sizeof(canvas));
    pCanvas->width = width;
    pCanvas->height = height;
    pCanvas->items = NULL;
    pCanvas->value = (char*)calloc(width * height, sizeof(char*));
    pCanvas->render = &canvas_render_default;
    
    return pCanvas;
}

void canvas_destroy(canvas* pCanvas){
    if(pCanvas == NULL)
        return;
    
    canvas_list_elements_free(pCanvas->items);
    canvas_list_free(pCanvas->items);
    free(pCanvas->value);
    free(pCanvas);
}

void canvas_render(canvas* pCanvas){
    if(pCanvas == NULL)
        return;
    
    memset(pCanvas->value, '\0', sizeof(char*) * pCanvas->width * pCanvas->height);
    //free(pCanvas->value);
    //pCanvas->value = malloc(sizeof(char*) * pCanvas->width * pCanvas->height);
    canvas_element_list* pCurrentItem = pCanvas->items;
    
    while(pCurrentItem != NULL){
        pCanvas->render(pCurrentItem->element);
        pCurrentItem = pCurrentItem->next;
    }
}

void canvas_render_default(canvas_element* pElement){
    canvas* pCanvas = pElement->canvas;
    char (*pCanvasValue)[pCanvas->width] = (char(*)[pCanvas->width])pCanvas->value;
    char (*pElementValue)[pElement->width] = (char(*)[pElement->width])pElement->value;
    
    if(!pElement->visible)
        return;
    
    for(int i = 0; i < pElement->height; i++){
        uint8_t y = pElement->position.y + i;
        
        if(y >= pCanvas->height)
            y = y % (pCanvas->height);
        
        for(int j = 0; j < pElement->width; j++){
            uint8_t x = pElement->position.x + j;
            
            if(x >= pCanvas->width)
                x = x % (pCanvas->width);
            
            pCanvasValue[y][x] |= pElementValue[i][j];
        }
    }
    
    return;
}

void canvas_element_free(canvas_element* pElement){
    if(pElement == NULL)
        return;
    
    free(pElement->value);
    free(pElement);
}

void canvas_list_elements_free(canvas_element_list* pCanvasItems){
    canvas_element_list* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return;
    
    pCurrentItem = canvas_list_first(pCurrentItem);
    
    while(pCurrentItem != NULL){
        canvas_element_free(pCurrentItem->element);
        //free(pCurrentItem->element);
        pCurrentItem = pCurrentItem->next;
    };
}

void canvas_list_free(canvas_element_list* pCanvasItems){
    canvas_element_list* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return;
    
    pCurrentItem = canvas_list_first(pCurrentItem);
    
    while(pCurrentItem != NULL){
        canvas_element_list* pTmp = pCurrentItem;
        pCurrentItem = pCurrentItem->next;
        free(pTmp);
    }
}

canvas_element_list* canvas_list_first(canvas_element_list* pCanvasItems){
    canvas_element_list* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return NULL;
    
    while(pCurrentItem->previous != NULL)
        pCurrentItem = pCurrentItem->previous;
    
    return pCurrentItem;
}

canvas_element_list* canvas_list_last(canvas_element_list* pCanvasItems){
    canvas_element_list* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return NULL;
    
    while(pCurrentItem->next != NULL)
        pCurrentItem = pCurrentItem->next;
    
    return pCurrentItem;
}

canvas_element_list* canvas_list_next(canvas_element_list** ppCanvasItems){
    if(ppCanvasItems == NULL)
        return NULL;

    *ppCanvasItems = (*ppCanvasItems)->next;
    
    return *ppCanvasItems;
}

int canvas_list_count(canvas_element_list* pElementList){
    if(pElementList == NULL)
        return 0;
    
    int count = 0;
    canvas_element_list* pCurrentItem = pElementList;
    
    while(pCurrentItem != NULL){
        count++;
        pCurrentItem = pCurrentItem->next;
    }
    
    return count;
}

int canvas_items_count(canvas* pCanvas){
    return canvas_list_count(pCanvas->items);
}

canvas_element* canvas_element_add(canvas* pCanvas, canvas_element* pElement){
    if(pElement == NULL || pCanvas == NULL)
        return NULL;
    
    canvas** ppCanvas = &pElement->canvas;
    *ppCanvas = pCanvas;
    canvas_element_list* pElmentList = (canvas_element_list*)calloc(1, sizeof(canvas_element_list));
    pElmentList->element = pElement;
    
    canvas_element_list* pCurrentItem = pCanvas->items;

    if(pCurrentItem == NULL){
        pCanvas->items = pElmentList;
    }else{
        while(pCurrentItem->next != NULL)
            pCurrentItem = pCurrentItem->next;
        
        pCurrentItem->next = pElmentList;
        pElmentList->previous = pCurrentItem;
    }
    
    return pElement;
}

int canvas_element_remove(canvas* pCanvas, canvas_element* pElement){
    if(pElement == NULL || pCanvas == NULL)
        return -1;
    
    canvas_element_list* pCurrentItem = pCanvas->items;
    
    do{
        if(pCurrentItem->element == pElement){
            if(pCurrentItem->previous != NULL)
                pCurrentItem->previous->next = pCurrentItem->next;
            
            if(pCurrentItem->next != NULL)
                pCurrentItem->next->previous = pCurrentItem->previous;
            
            if(pCurrentItem == pCanvas->items)
                pCanvas->items = pCurrentItem->next;
            
            free(pCurrentItem);
            // Have to free the element itself separately
            
            return 1;
        }
        
        pCurrentItem = pCurrentItem->next;
    }while(pCurrentItem != NULL);
    
    return -1;
}


canvas_element_list* canvas_elements_by_type(canvas* pCanvas, uint8_t type){
    if(pCanvas == NULL)
        return NULL;
    
    int count = 0;
    canvas_element* pElements[canvas_items_count(pCanvas)];
    
    canvas_element_list* pCurrentItem = pCanvas->items;
    
    while(pCurrentItem != NULL){
        if(pCurrentItem->element->type == type)
            pElements[count++] = pCurrentItem->element;
    }
    
    canvas_element_list* pItems = calloc(count, sizeof(canvas_element_list));
    
    for(int i = 0; i < count; i++){
        pItems[i].element = pElements[i];

        if(i > 0){
            pItems[i].previous = &pItems[i - 1];
            pItems[i-1].next = &pItems[i];
        }
    }
    
    return pItems;
}

canvas_element_list* canvas_elements_at_point(canvas* pCanvas, canvas_point* pPoint){
    if(pCanvas == NULL || pPoint == NULL)
        return NULL;
    
    if(pPoint->x > pCanvas->width - 1 || pPoint->y > pCanvas->height - 1)
        return NULL;
    
    canvas_element_filter filter;
    filter.state = pPoint;
    filter.predicate = &canvas_element_filter_point;
    
    return canvas_elements_filtered(pCanvas, &filter);
}

canvas_element_list* canvas_elements_value_at_point(canvas* pCanvas, canvas_point_value* pPointValue){
    if(pCanvas == NULL || pPointValue == NULL)
        return NULL;
    
    if(pPointValue->point.x > pCanvas->width - 1 || pPointValue->point.y > pCanvas->height - 1)
        return NULL;
    
    canvas_element_filter filter;
    filter.state = pPointValue;
    filter.predicate = &canvas_element_filter_value;
    
    return canvas_elements_filtered(pCanvas, &filter);
}


char canvas_element_value(canvas_element* pElement, canvas_point* pPoint){
    if(pElement == NULL || pPoint == NULL)
        return '\0';
    
    canvas_point* pElementPoint = &pElement->position;
    char (*pElementValue)[pElement->width] = (char(*)[pElement->width])pElement->value;
    
    if(
       pPoint->x >= pElementPoint->x && pPoint->x <= pElement->width - 1 &&
       pPoint->y >= pElementPoint->y && pPoint->y <= pElement->height - 1
    ){
        uint8_t relX = pPoint->x - pElementPoint->x;
        uint8_t relY = pPoint->y - pElementPoint->y;
        
        return pElementValue[relY][relX];
    }
    
    return '\0';
}

canvas_element_list* canvas_elements_filtered(canvas* pCanvas, canvas_element_filter* pPredicate){
    if(pCanvas == NULL || pPredicate == NULL)
        return NULL;

    canvas_element_filter* pCurrentPredicate;
    canvas_element_list* pCurrentItem = pCanvas->items;
    canvas_element* pResults[canvas_items_count(pCanvas)];
    int count = 0;
    
    while(pCurrentItem != NULL){
        pCurrentPredicate = pPredicate;
        char match = 0;
        
        while(pCurrentPredicate != NULL){
            if(pCurrentPredicate->predicate(pCurrentItem->element, pPredicate->state) == 1)
                match = 1;
                break;
            
            pCurrentPredicate = pCurrentPredicate->next;
        }
        
        if(match)
            pResults[count++] = pCurrentItem->element;
        
        pCurrentItem = pCurrentItem->next;
    }
    
    canvas_element_list* pItems = NULL;
    
    if(count > 0){
        pItems = calloc(count, sizeof(canvas_element_list));
    
        for(int i = 0; i < count; i++){
            pItems[i].element = pResults[i];
            
            if(i > 0){
                pItems[i].previous = &pItems[i - 1];
                pItems[i-1].next = &pItems[i];
            }
        }
    }
    
    return pItems;
}

/* Element filter predicates */
 
int canvas_element_filter_point(canvas_element* pElement, void* pState){
    canvas_point* pPoint = (canvas_point*)pState;
 
    uint8_t x = pElement->position.x;
    uint8_t y = pElement->position.y;
    
    if(
       pPoint->x >= x && pPoint->x <= pElement->width - 1 &&
       pPoint->y >= y && pPoint->y <= pElement->height - 1
    ){
        return 1;
    }
    
    return 0;
}

int canvas_element_filterype(canvas_element* pElement, void* pState){
    uint8_t type = *(uint8_t*)pState;
    
    if(pElement->type == type)
        return 1;
    
    return 0;
}

int canvas_element_filter_value(canvas_element* pElement, void* pState){
    canvas_point_value* pPointValue = (canvas_point_value*)pState;
    
    if(canvas_element_value(pElement, &pPointValue->point) == pPointValue->value){
        return 1;
    }
    
    return 0;
}

 
