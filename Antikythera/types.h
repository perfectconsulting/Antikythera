/*
 *
 *    types.h
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

#ifndef TYPES_H
#define TYPES_H

typedef unsigned char   Types_Byte;
typedef unsigned short  Types_Word;
typedef unsigned long   Types_DWord;
typedef short           Types_Bool;
typedef double          Types_Real;

typedef union _Types_Poly {
    Types_Bool          Bool;
    Types_Byte          Byte;
    Types_Word          Word;
    Types_DWord         DWord;
    Types_Real          Real;
    
} Types_Poly;

#endif /* TYPES_H */

