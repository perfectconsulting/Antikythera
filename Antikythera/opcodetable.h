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
#ifndef OPCODETABLE_H
#define OPCODETABLE_H

#include "types.h"
#include "ops.h"

typedef enum operand { NO_OPERAND=0, BYTE_OPERAND, WORD_OPERAND, DWORD_OPERAND, REAL_OPERAND, STRING_OPERAND, OFFSET_OPERAND } Operand;

typedef struct opcode {
    char                *mnemonic;
    Types_Byte          opcode;
    Operand             operand1;
    Operand             operand2;
} OpCode;
  
static OpCode OpCodeTable[] =   
{  
    { "noop", NO_OP, NO_OPERAND, NO_OPERAND},    
    { "llitb", LL_INT_1, BYTE_OPERAND, NO_OPERAND},
    { "lliti", LL_INT_4, DWORD_OPERAND, NO_OPERAND},
    { "llitr", LL_REAL_8, REAL_OPERAND, NO_OPERAND},    
    { "llits", LL_STR_1, STRING_OPERAND, NO_OPERAND}, 
    { "finish", FINISH_OP, NO_OPERAND, NO_OPERAND},
    { "\0", 0, 0, 0}     
}; 

#endif /* OPCODETABLE_H */

