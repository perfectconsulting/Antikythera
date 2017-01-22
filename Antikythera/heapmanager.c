/*
 *
 *    heapmanager.g
 *    Version 1.00 (C99)  
 * 
 *    Copyright 2017 Steven Janes (www.perfectconsulting.co.uk)
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "types.h"

typedef struct heapmanager {
    unsigned char       *heapbase;
    unsigned char       *free;
    long                size;
    unsigned char       *firstnode;
    unsigned char       *lastnode;
    long                nodes;
} HeapManager;

typedef struct heapnode 
{
    unsigned char       signature;
    unsigned char       *ref;
    long                count;
    short               size;

    void                *next;
} HeapNode;

HeapManager *HeapManager_Create(Types_Byte *heapbase, long size)
{
    HeapManager *heap = malloc(sizeof(HeapManager));
    
    heap->heapbase = heapbase;
    heap->free = heapbase;
    heap->size = size;
    heap->firstnode = NULL;
    heap->lastnode = NULL;
    heap->nodes = 0;
    
    return(heap);
}

short HeapManager_Destroy(HeapManager *heap)
{
    if(heap == NULL)
        return(FALSE);
    
    free(heap);
}

long HeapManager_FreeSpace(HeapManager *heap)
{
    if(heap == NULL)
        return(0);
    
    unsigned char *highwater = heap->heapbase + heap->size;
    
    return(highwater - heap->free);
}

unsigned char *HeapManager_CoreAllocate(HeapManager *heap, short size)
{
    if(heap == NULL)
        return(NULL);
    
    if(HeapManager_FreeSpace(heap) < size)
        return(NULL);
    
    unsigned char *new = heap->free;
    heap->free += size;
    
    return(new);
}

HeapNode *HeapManager_FirstFit(HeapManager *heap, short size)
{
    for(HeapNode *node = (HeapNode*)heap->firstnode; node; node = node->next)
        if(node->count == 0 && node->size >= size)
            return(node);
        
    return(NULL);
}

unsigned char *HeapManager_Allocate(HeapManager *heap, long size)
{
    if(heap == NULL)
        return(NULL);
   
    HeapNode *node = HeapManager_FirstFit(heap, size);
    if(node != NULL) {
        node->count = 1;
        return(node->ref);
    }
        
    if(HeapManager_FreeSpace(heap) < sizeof(HeapNode) + size)
        return(NULL);
        
    node = (HeapNode*)HeapManager_CoreAllocate(heap, sizeof(HeapNode));
    node->signature = 0x99;
    node->ref = HeapManager_CoreAllocate(heap, size);
    node->count = 1;
    
    heap->nodes++; 
    
    if(heap->size == 0){
        heap->firstnode = (unsigned char*)node;
        heap->lastnode = (unsigned char*)node;
        
        return(node->ref);
    }
   
    return(node->ref);
}

short HeapManager_DeAllocate(HeapManager *heap, unsigned char* mem)
{
    HeapNode *node = (HeapNode*)mem - sizeof(HeapNode);
    if(node->signature != 0x99)
        return(FALSE);
    
    if(node->count > 0)
        node->count--;
    
    return(TRUE);
}
