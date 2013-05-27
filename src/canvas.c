//
//  canvas.c
//  Tetris
//
//  Created by Blake on 27/05/13.
//
//

#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"

canvas_t* canvas_create(uint8_t width, uint8_t height){
    canvas_t* pCanvas = malloc(sizeof(canvas_t));
    pCanvas->width = width;
    pCanvas->height = height;
    pCanvas->items = NULL;
    
    return pCanvas;
}

void canvas_destroy(canvas_t* pCanvas){
    free(pCanvas);
}

void canvas_items_free(canvas_item_t* pCanvasItems){
    canvas_item_t* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return;
    
    pCurrentItem = canvas_items_first(pCurrentItem);
    
    do{
        pCurrentItem = pCurrentItem->next;
        free(pCurrentItem);
    }while(pCurrentItem != NULL);
}

canvas_item_t* canvas_items_first(canvas_item_t* pCanvasItems){
    canvas_item_t* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return NULL;
    
    while(pCurrentItem->previous != NULL)
        pCurrentItem = pCurrentItem->previous;
    
    return pCurrentItem;
}

canvas_item_t* canvas_items_last(canvas_item_t* pCanvasItems){
    canvas_item_t* pCurrentItem = pCanvasItems;
    
    if(pCurrentItem == NULL)
        return NULL;
    
    while(pCurrentItem->next != NULL)
        pCurrentItem = pCurrentItem->next;
    
    return pCurrentItem;
}

canvas_item_t* canvas_items_next(canvas_item_t* pCanvasItems){
    if(pCanvasItems == NULL)
        return NULL;
    
    return pCanvasItems->next;
}

int canvas_items_count(canvas_t* pCanvas){
    int count = 0;
    canvas_item_t* pCurrentItem = pCanvas->items;
    
    if(pCurrentItem == NULL)
        return 0;
    
    while(pCurrentItem != NULL){
        count++;
        pCurrentItem = pCurrentItem->next;
    }
    
    return count;
}

canvas_item_t* canvas_element_add(canvas_t* pCanvas, canvas_element_t* pElement){
    if(pElement == NULL || pCanvas == NULL)
        return NULL;

    canvas_item_t* pNewItem = malloc(sizeof(canvas_item_t));
    pNewItem->element = *pElement;
    
    canvas_item_t* pCurrentItem = pCanvas->items;

    if(pCurrentItem == NULL){
        pCanvas->items = pNewItem;
    }else{
        while(pCurrentItem->next != NULL)
            pCurrentItem = pCurrentItem->next;
        
        pCurrentItem->next = pNewItem;
        pNewItem->previous = pCurrentItem;
    }
    
    return pNewItem;
}

int canvas_element_remove(canvas_t* pCanvas, canvas_element_t* pElement){
    if(pElement == NULL || pCanvas == NULL)
        return -1;
    
    canvas_item_t* pCurrentItem = pCanvas->items;
    
    do{
        if(&pCurrentItem->element == pElement){
            if(pCurrentItem->previous != NULL)
                pCurrentItem->previous->next = pCurrentItem->next;
            
            if(pCurrentItem->next != NULL)
                pCurrentItem->next->previous = pCurrentItem->previous;
            
            if(pCurrentItem == pCanvas->items)
                pCanvas->items = pCurrentItem->next;
            
            free(pCurrentItem);
            
            return 1;
        }
        
        pCurrentItem = pCurrentItem->next;
    }while(pCurrentItem != NULL);
    
    return -1;
}


canvas_item_t* canvas_elements_by_type(canvas_t* pCanvas, uint8_t type){
    if(pCanvas == NULL)
        return NULL;
    
    int count = 0;
    canvas_element_t* pElements[canvas_items_count(pCanvas)];
    
    canvas_item_t* pCurrentItem = pCanvas->items;
    
    while(pCurrentItem != NULL){
        if(pCurrentItem->element.type == type)
            pElements[count++] = &pCurrentItem->element;
    }
    
    canvas_item_t* pItems = malloc(sizeof(canvas_item_t) * count);
    
    for(int i = 0; i < count; i++){
        pItems[i].element = *pElements[i];

        if(i > 0){
            pItems[i].previous = &pItems[i - 1];
            pItems[i-1].next = &pItems[i];
        }
    }
    
    return pItems;
}

canvas_item_t* canvas_elements_at_point(canvas_t* pCanvas, canvas_point_t* pPoint){
    if(pCanvas == NULL || pPoint == NULL)
        return NULL;
    
    if(pPoint->x > pCanvas->width - 1 || pPoint->y > pCanvas->height - 1)
        return NULL;
    
    canvas_element_filter_t filter;
    filter.state = pPoint;
    filter.predicate = &canvas_element_filter_point;
    
    return canvas_elements_filtered(pCanvas, &filter);
}

canvas_item_t* canvas_elements_value_at_point(canvas_t* pCanvas, canvas_point_value_t* pPointValue){
    if(pCanvas == NULL || pPointValue == NULL)
        return NULL;
    
    if(pPointValue->point.x > pCanvas->width - 1 || pPointValue->point.y > pCanvas->height - 1)
        return NULL;
    
    canvas_element_filter_t filter;
    filter.state = pPointValue;
    filter.predicate = &canvas_element_filter_value;
    
    return canvas_elements_filtered(pCanvas, &filter);
}

char canvas_element_value(canvas_element_t* pElement, canvas_point_t* pPoint){
    if(pElement == NULL || pPoint == NULL)
        return '\0';
    
    canvas_point_t* pElementPoint = &pElement->position;
    
    if(
       pPoint->x >= pElementPoint->x && pPoint->x <= pElement->width - 1 &&
       pPoint->y >= pElementPoint->y && pPoint->y <= pElement->height - 1
    ){
        uint8_t relX = pPoint->x - pElementPoint->x;
        uint8_t relY = pPoint->y - pElementPoint->y;
        
        return pElement->element[relY][relX];
    }
    
    return '\0';
}

canvas_item_t* canvas_elements_filtered(canvas_t* pCanvas, canvas_element_filter_t* pPredicate){
    if(pCanvas == NULL || pPredicate == NULL)
        return NULL;

    canvas_element_filter_t* pCurrentPredicate;
    canvas_item_t* pCurrentItem = pCanvas->items;
    canvas_element_t* pResults[canvas_items_count(pCanvas)];
    int count = 0;
    
    while(pCurrentItem != NULL){
        pCurrentPredicate = pPredicate;
        
        while(pCurrentPredicate != NULL){
            if(pPredicate->predicate(&pCurrentItem->element, pPredicate->state) == 0)
                goto next_item;
            
            pCurrentPredicate = pCurrentPredicate->next;
        }
        
        pResults[count++] = &pCurrentItem->element;
        
    next_item:
        continue;
    }
    
    canvas_item_t* pItems = malloc(sizeof(canvas_item_t) * count);
    
    for(int i = 0; i < count; i++){
        pItems[i].element = *pResults[i];
        
        if(i > 0){
            pItems[i].previous = &pItems[i - 1];
            pItems[i-1].next = &pItems[i];
        }
    }
    
    return pItems;
}

/* Element filter predicates */
 
int canvas_element_filter_point(canvas_element_t* pElement, void* pState){
    canvas_point_t* pPoint = (canvas_point_t*)pState;
 
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

int canvas_element_filter_type(canvas_element_t* pElement, void* pState){
    uint8_t type = *(uint8_t*)pState;
    
    if(pElement->type == type)
        return 1;
    
    return 0;
}

int canvas_element_filter_value(canvas_element_t* pElement, void* pState){
    canvas_point_value_t* pPointValue = (canvas_point_value_t*)pState;
    
    if(canvas_element_value(pElement, &pPointValue->point) == pPointValue->value){
        return 1;
    }
    
    return 0;
}
 
