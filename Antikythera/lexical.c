/*
 *
 *    lexical.c
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
#include <ctype.h>
#include <string.h>
#include "constants.h"

typedef enum tokentype { E_TOKEN=0, IDENTIFER_TOKEN, LABEL_TOKEN, INTEGER_TOKEN, REAL_TOKEN, STRING_TOKEN } TokenType;

typedef struct lexical {
    short       line;
    char        symbol;
    FILE        *fp;    
    
    TokenType   tokentype;
    char        *token;
} Lexical;

Lexical *Lexical_Create(short maximumtokensize)
{
    Lexical *lex = malloc(sizeof(Lexical));
    
    lex->symbol = ' ';
    lex->fp = NULL;
    
    lex->tokentype = E_TOKEN;
    lex->token = malloc(maximumtokensize);
    
    return(lex);
}

short Lexical_Destroy(Lexical *lex)
{
    if(lex == NULL)
        return(FALSE);
    
    if(lex->fp != NULL)
        fclose(lex->fp);

    free(lex->token);
    
    free(lex);
    
    return(TRUE);
}

static char Next(Lexical *lex)
{
    if(lex == NULL)
        return 0;
    
    if(lex->fp == NULL)
        return 0;
    
    if(lex->symbol == EOF)
        return lex->symbol;
        
    lex->symbol = fgetc(lex->fp);
    
    if(lex->symbol == '\n')
        lex->line++;
    
    return(lex->symbol);
}

static short isWhiteSpace(char c)
{
    return(c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static void SkipWhiteSpace(Lexical *lex)
{
    if(lex == NULL)
        return;
    
    while(isWhiteSpace(lex->symbol))
        Next(lex);
}

short Lexical_Prime(Lexical *lex, char *file)
{
    if(lex == NULL)
        return(FALSE);

    lex->fp = fopen(file,"r");
    if(lex->fp == NULL)
        return(TRUE);
    
    lex->line = 1;
    Next(lex);            
    
    return(TRUE);
 }

short Lexical_HaveSymbol(Lexical *lex, char ch)
{
    if(lex == NULL)
        return(FALSE);
    
    if(lex->symbol != ch)
        return(FALSE);
    
    Next(lex);
    
    return(TRUE);
}

short Lexical_NextToken(Lexical *lex)
{
    if(lex == NULL)
        return FALSE;
    
    if(lex->symbol == EOF)
        return(FALSE);
    
    SkipWhiteSpace(lex);
    
    if(lex->symbol == EOF)
        return(FALSE);
    
    char *p = lex->token;
    
    if(lex->symbol == '"'){
        do {
             *p++ = lex->symbol;
             Next(lex);
        } while(lex->symbol != '"');
        
        Next(lex);
        
        *p = 0;
        lex->tokentype = STRING_TOKEN;
        
        SkipWhiteSpace(lex);
        return(TRUE);
    }
    
    if(isdigit(lex->symbol)) {
        lex->tokentype = INTEGER_TOKEN;
        do {
            if(lex->symbol == '.')
                lex->tokentype = REAL_TOKEN;
            
            *p++ = lex->symbol;
            Next(lex);
        } while(isdigit(lex->symbol));
        
        *p = 0;
 
        SkipWhiteSpace(lex);
        return(TRUE);
    }
    
    lex->tokentype = IDENTIFER_TOKEN;

    do {
        *p++ = lex->symbol;
        Next(lex);
    } while(isalnum(lex->symbol));
    
    if(lex->symbol == ':'){
        *p++ = Next(lex);
        lex->tokentype = LABEL_TOKEN;
    }
    
    *p = 0;
  
    SkipWhiteSpace(lex);
    return(TRUE);
}




