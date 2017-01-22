/*
 *
 *    abstractmachine.c
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
#include "configuration.h"
#include "types.h"
#include "ops.h"
#include "heapmanager.h"

typedef struct abstractmachine {
    Configuration       *config;
    
    Types_Byte          *codespace;
    Types_Byte          *dataspace;
    
} AbstractMachine;

Types_Word              Reg_W;
Types_Poly              Reg_D1,Reg_D2, Reg_D3, Reg_D4;
Types_Byte              *Reg_PC;
Types_Poly              *Reg_SP;    
Types_Byte              Reg_S;

HeapManager             *Heap;

typedef void(*OpCode)(void);

AbstractMachine *AbstractMachine_Create(Configuration *config)
{
    AbstractMachine *mac = malloc(sizeof(AbstractMachine));
    
    mac->config = config;
    
    return(mac);
}

short AbstractMachine_Destrory(AbstractMachine *mac)
{
    if(mac == NULL)
        return(FALSE);
    
    free(mac);
}

static void OpCode_Invalid(void)
{
    
}

static void OpCode_LLIT1(void)
{
    Reg_D1.Byte = *((Types_Byte*)Reg_PC);
    *Reg_SP-- = Reg_D1;
    Reg_SP--;
    Reg_PC+=sizeof(Types_Byte);
}
    
static void OpCode_LLIT4(void)
{
    Reg_D1.DWord = *((Types_DWord*)Reg_PC);
    *Reg_SP-- = Reg_D1;
    Reg_SP--;
    Reg_PC+=sizeof(Types_DWord);
}
    
static void OpCode_LLITR(void)
{
}

static void OpCode_LLITS(void)
{
    
}

static void OpCode_FINISH(void)
{
    Reg_S = 0;
}

short AbstractMachine_Execute(AbstractMachine *mac)
{    
    if(mac == NULL)
        return(FALSE);
        
    OpCode OpCodes[256];

    for(int i = 0; i < 256; i++)
        OpCodes[i] = OpCode_Invalid;

    OpCodes[LL_INT_1]   = OpCode_LLIT1;
    OpCodes[LL_INT_4]   = OpCode_LLIT4;
    OpCodes[LL_REAL_8]  = OpCode_LLITR;
    OpCodes[LL_STR_1]   = OpCode_LLITS;
    OpCodes[FINISH_OP]  = OpCode_FINISH;
   
    Reg_PC = mac->codespace;
    Reg_SP = (Types_Poly*)mac->codespace;
    Reg_SP += (mac->config->codespacesize - sizeof(Types_Poly));
    Reg_S = 1;
    
    Heap = HeapManager_Create(mac->dataspace, mac->config->dataspacesize);
    
    while(Reg_S )
    {
        OpCodes[*Reg_PC++]();
    }
}

static void Invalid(void)
{
    
}