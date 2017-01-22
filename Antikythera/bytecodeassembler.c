/*
 *
 *    assembler.c
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
#include <string.h>
#include "constants.h"
#include "types.h"
#include "configuration.h"
#include "keystore.h"
#include "stringpool.h"
#include "keystore.h"
#include "lexical.h"
#include "ops.h"
#include "opcodetable.h"
#include "types.h"

typedef struct bytecodeassembler {
    Types_Byte          *codespace;
    Types_Byte          *cp;
    unsigned long       size;
} ByteCodeAssembler;

ByteCodeAssembler  *ByteCodeAssembler_Create(void){
    ByteCodeAssembler *bca = malloc(sizeof(ByteCodeAssembler));
    
    return(bca);
}

short ByteCodeAssembler_Destroy(ByteCodeAssembler *bca)
{
    if(bca == NULL)
        return(FALSE);
    
    free(bca);
}

void ByteCodeAssembler_CodeMemoryDump(ByteCodeAssembler *bca)
{
    if(bca == NULL)
        return;
    
    Types_Byte *dp = bca->codespace;
    long size = bca->size;
    
    printf("----------------------------------------------------------------------------------------------------------\n");
    
    do {
        printf("%pl ",dp );
    
        for(int i=0; i<32; i++)
            printf("%02x ", *dp++);
        
        size -= 32;
        printf("\n");
    } while(size > 0);
    
    printf("----------------------------------------------------------------------------------------------------------\n");
}

static OpCode *FindOpCode(char* mnemonic)
{
    for(OpCode *op = OpCodeTable; *(op->mnemonic); op++)
        if(strcmp(op->mnemonic, mnemonic) == 0)
            return(op);
     
    return(NULL);
}

static void AssembleByte(ByteCodeAssembler *bca, Types_Byte value)
{
    short size = sizeof(Types_Byte);
    *(bca->cp) = value;
    bca->cp+=size;
    bca->size+=size;
}

static void AssembleWord(ByteCodeAssembler *bca, Types_Word value)
{
    short size = sizeof(Types_Word);
    *((Types_Word*)bca->cp) = value;
    bca->cp+=size;
    bca->size+=size;
}

static void AssembleDWord(ByteCodeAssembler *bca, Types_DWord value)
{
    short size = sizeof(Types_DWord);
    *((Types_DWord*)bca->cp) = value;
    bca->cp+=size;
    bca->size+=size;
}

static void AssembleReal(ByteCodeAssembler *bca, Types_Real value)
{
    short size = sizeof(Types_Real);
    *((Types_Real*)bca->cp) = value;
    bca->cp+=size;
    bca->size+=size;
}

static void AssembleString(ByteCodeAssembler *bca, char* value)
{
    AssembleWord(bca, strlen(value));
    char *p = value;
    while(*p)
        AssembleByte(bca, *p++);
}

static short AssemblePoly(ByteCodeAssembler *bca, Operand op, char *value)
{
    Types_Poly poly;
    short result = TRUE;
    
    switch(op){
        case NO_OPERAND:
            break;
            
        case BYTE_OPERAND:
            poly.Byte = atoi(value);
            AssembleByte(bca, poly.Byte);
            break;

        case WORD_OPERAND:
            poly.Word = atoi(value);
            AssembleWord(bca, poly.Word);
            break;
            
        case DWORD_OPERAND:
            poly.DWord = atol(value);
            AssembleDWord(bca, poly.DWord);
            break;
            
        case REAL_OPERAND:
            poly.Real =  atof(value);
            AssembleReal(bca, poly.Real);
            break;
            
        case STRING_OPERAND:
            AssembleString(bca, value);
            break;
    }
}

static void Error(char *error)
{
    printf("%s\n", error);
}

short ByteCodeAssembler_Assemble(ByteCodeAssembler *bca, char *file)
{
    char pad[128];
    
    if(bca == NULL)
        return(FALSE);

    bca->cp = bca->codespace;
        
    Lexical *lex = Lexical_Create(128);   
 
    short error = FALSE;
    
    if(!Lexical_Prime(lex, file)){
        Lexical_Destroy(lex);
        return(FALSE);
    }

    while(lex->symbol != EOF)
    {
        if(Lexical_NextToken(lex)) {
            if(lex->tokentype != IDENTIFER_TOKEN){
                printf(pad,"error on line %d, found '%s' expeceted identifier", lex->line, lex->token);
                Error(pad);
                error = TRUE;
                break;
            }
            
            OpCode *op = FindOpCode(lex->token);
            if(op == NULL){
                sprintf(pad,"error on line %d, unknown mnemonic '%s'.\n", lex->line, lex->token);
                Error(pad);
                error = TRUE;
                break;                
            }
            
            AssembleByte(bca, op->opcode);
     
            
            if(op->operand1 != NO_OPERAND) {
                if(!Lexical_HaveSymbol(lex, '(')){
                    sprintf(pad, "error on line %d, expected '(' found '%c'", lex->line, lex->symbol);
                    Error(pad);
                    error = TRUE;
                    break;                                    
                }
                    
                if(!Lexical_NextToken(lex)){
                    sprintf(pad,"error on line %d, literal value expected", lex->line);
                    Error(pad);
                    error = TRUE;
                    break;                                                        
                }
                
                AssemblePoly(bca, op->operand1, lex->token);
                
                if(Lexical_HaveSymbol(lex, ',')) {
                    if(!Lexical_NextToken(lex)){
                        sprintf(pad, "error on line %d, literal value expected", lex->line);
                        Error(pad);
                        error = TRUE;
                        break;                                                        
                    }
                    
                    AssemblePoly(bca, op->operand2, lex->token);
                }

                if(!Lexical_HaveSymbol(lex, ')')){
                    printf("error on line %d, expected '()' found '%c'", lex->line, lex->symbol);
                    error = TRUE;
                    break;                                    
                }                
            }
        }
    }
    
    return(!error);
}


 