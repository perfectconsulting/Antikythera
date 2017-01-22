/*
 *
 *    stringpool.c
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

typedef struct stringpoo_node {
    char*                       pad;
    short                       maxlength;
    void*                       next;
} StringPool_Node;

typedef struct _StringPool {
    long                        size;
    long                        maxsize;
    void*                       node;
} StringPool;

StringPool* StringPool_Create(long maxsize) {
    if(maxsize == 0)
        return NULL;
    
    StringPool* sp = malloc(sizeof(StringPool));
    
    sp->maxsize = maxsize;
    sp->size = 0;
    sp->node = NULL;
    
    return(sp);
}

short StringPool_Destroy(StringPool *sp) {
    if(sp == NULL)
        return(FALSE);
    
    StringPool_Node *nd;
    nd = sp->node;
    
    StringPool_Node *p;
    
    while(nd != NULL) {
        p = nd->next;
        free(nd->pad);
        free(nd);
        nd = p;
    }
    
    free(sp);
    
    return(TRUE);
}

char* StringPool_String(StringPool *sp, short maxlength) {
    if(sp == NULL || maxlength == 0)
        return(NULL);
    
    long size = sp->size + maxlength;
    if(size > sp->maxsize)
        return(NULL);
    
    char *pad = malloc(maxlength + 1);
    StringPool_Node* nd = malloc(sizeof(StringPool_Node));
    
    sp->size = size;
    
    nd->pad = pad;
    nd->maxlength = maxlength;
    nd->next = sp->node;
    
    sp->node = nd;
    
    return(pad);
}
void StringPool_Dump(StringPool *sp) {
    if (sp == NULL)
        return;

    printf("-----------------------------------------------------------------------------------\n");
    printf("(@%p) size=%ld maxsize=%ld\n", sp, sp->size, sp->maxsize);

    for (StringPool_Node *np = sp->node; np->next != NULL; np = np->next) {
        printf("(@%p) [%d]\"%s\"\n", np, np->maxlength , np->pad);
    }
    printf("-----------------------------------------------------------------------------------\n");
}