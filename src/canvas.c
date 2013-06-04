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
    canvas_element_list* pCurrentItem = canvas_list_first(pCanvas->items);
    
    if(pCurrentItem == NULL)
        return;
    
    do{
        pCanvas->render(pCurrentItem->current);
    }while(canvas_list_next(&pCurrentItem) != NULL);
}

void canvas_render_default(canvas_element* pElement){
    canvas* pCanvas = pElement->canvas;
    char (*pCanvasValue)[pCanvas->width] = (char(*)[pCanvas->width])pCanvas->value;
    char (*pElementValue)[pElement->width] = (char(*)[pElement->width])pElement->value;
    
    if(!pElement->visible)
        return;
    
    for(int i = 0; i < pElement->height; i++){
        int16_t y = pElement->position.y + i;
        
        if(y >= pCanvas->height && pElement->wrap_x)
            y = abs(y % (pCanvas->height));
        
        if(y < 0 || y > pCanvas->height - 1)
            continue;
            
        
        for(int j = 0; j < pElement->width; j++){
            int16_t x = pElement->position.x + j;
            
            if(x >= pCanvas->width && pElement->wrap_x)
                x = abs(x % (pCanvas->width));
            
            if(x < 0 || x > pCanvas->width - 1)
                continue;
            
            if(pCanvasValue[y][x] == '\0')
                pCanvasValue[y][x] = pElementValue[i][j];
        }
    }
    
    return;
}

canvas_element* canvas_element_create(uint8_t width, uint8_t height, char* pValue){
    canvas_element* pElement = (canvas_element*)calloc(1, sizeof(canvas_element));
    pElement->width = width;
    pElement->height = height;
    pElement->type = 10;
    pElement->position.x = 0;
    pElement->position.y = 0;
    pElement->visible = 1;
    pElement->value = pValue;
    
    return pElement;
}

canvas_element* canvas_element_copy(canvas_element* pCanvasElement){
    int size = pCanvasElement->width * pCanvasElement->height * sizeof(char);
    canvas_element* pElement = (canvas_element*)calloc(1, sizeof(canvas_element));
    char* pValue = NULL;
    
    if(pCanvasElement->value != NULL){
        pValue = malloc(size);
        memcpy(pValue, pCanvasElement->value, size);
    }

    pElement->width = pCanvasElement->width;
    pElement->height = pCanvasElement->height;
    pElement->type = pCanvasElement->type;
    pElement->position.x = 0;
    pElement->position.y = 0;
    pElement->visible = 1;
    pElement->value = pValue;
    
    return pElement;
}

void canvas_element_free(canvas_element* pElement){
    if(pElement == NULL)
        return;
    
    if(pElement->name != NULL)
        free(pElement->name);

    if(pElement->value != NULL)
        free(pElement->value);
    
    free(pElement);
}

void canvas_list_elements_free(canvas_element_list* pCanvasItems){
    canvas_element_list* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return;
    
    pCurrentItem = canvas_list_first(pCurrentItem);
    
    while(pCurrentItem != NULL){
        canvas_element_free(pCurrentItem->current);
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

canvas_element_list* canvas_list_append(canvas_element_list** ppElementList, canvas_element* pElement){
    if(ppElementList == NULL)
        return NULL;
    
    if(pElement == NULL)
        return *ppElementList;
    
    if((*ppElementList)->current == NULL){
        (*ppElementList)->current = pElement;
        return *ppElementList;
    }
    
    canvas_element_list* pLastElement = canvas_list_last(*ppElementList);
    canvas_element_list* pNewElement = (canvas_element_list*)calloc(1, sizeof(canvas_element_list));
    pNewElement->current = pElement;
    pNewElement->previous = pLastElement;
    pLastElement->next = pNewElement;
    *ppElementList = pNewElement;

    return pNewElement;
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
    pElmentList->current = pElement;
    
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
    
    canvas_element_list* pElementList = canvas_list_first(pCanvas->items);
    
    do{
        if(pElementList->current == pElement){
            if(pElementList->previous != NULL)
                pElementList->previous->next = pElementList->next;
            
            if(pElementList->next != NULL)
                pElementList->next->previous = pElementList->previous;
            
            if(pElementList->previous != NULL){
                pCanvas->items = pElementList->previous;
            }else if(pElementList->next != NULL){
                pCanvas->items = pElementList->next;
            }else{
                pCanvas->items = NULL;
            }
            
            free(pElementList);
            
            return 1;
        }
    }while(canvas_list_next(&pElementList) != NULL);
    
    return -1;
}


canvas_element_list* canvas_elements_by_type(canvas* pCanvas, uint8_t type){
    if(pCanvas == NULL)
        return NULL;
    
    int count = 0;
    canvas_element* pElements[canvas_items_count(pCanvas)];
    
    canvas_element_list* pCurrentItem = pCanvas->items;
    
    while(pCurrentItem != NULL){
        if(pCurrentItem->current->type == type)
            pElements[count++] = pCurrentItem->current;
    }
    
    canvas_element_list* pItems = calloc(count, sizeof(canvas_element_list));
    
    for(int i = 0; i < count; i++){
        pItems[i].current = pElements[i];

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
    canvas_element_list* pCurrentItem = canvas_list_first(pCanvas->items);
    canvas_element* pResults[canvas_items_count(pCanvas)];
    int count = 0;
    
    while(pCurrentItem != NULL){
        pCurrentPredicate = pPredicate;
        char match = 0;
        
        while(pCurrentPredicate != NULL){
            if(pCurrentPredicate->predicate(pCurrentItem->current, pPredicate->state) == 1)
                match = 1;
                break;
            
            pCurrentPredicate = pCurrentPredicate->next;
        }
        
        if(match)
            pResults[count++] = pCurrentItem->current;
        
        pCurrentItem = pCurrentItem->next;
    }
    
    canvas_element_list* pItems = NULL;
    
    if(count > 0){
        pItems = calloc(count, sizeof(canvas_element_list));
    
        for(int i = 0; i < count; i++){
            pItems[i].current = pResults[i];
            
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

 
