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

#ifndef HEAPMANAGER_H
#define HEAPMANAGER_H

typedef struct heapmanager {
    unsigned char       *heapbase;
    unsigned char       *free;
    long                size;
    unsigned char       *firstnode;
    unsigned char       *lastnode;
    long                nodes;
} HeapManager;

HeapManager *HeapManager_Create(Types_Byte *heapbase, long size);
short HeapManager_Destroy(HeapManager *heap);
long HeapManager_FreeSpace(HeapManager *heap);
unsigned char *HeapManager_Allocate(HeapManager *heap, long size);
short HeapManager_DeAllocate(HeapManager *heap, unsigned char* mem);

#endif /* HEAPMANAGER_H */

