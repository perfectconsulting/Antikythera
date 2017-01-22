/*
 * 
 *    lexicam.h
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
#ifndef LEXICAL_H
#define LEXICAL_H

typedef enum tokentype { E_TOKEN=0, IDENTIFER_TOKEN, LABEL_TOKEN, INTEGER_TOKEN, REAL_TOKEN, STRING_TOKEN } TokenType;

typedef struct lexical {
    short       line;
    char        symbol;
    FILE        *fp;    
    
    TokenType   tokentype;
    char        *token;
} Lexical;

Lexical *Lexical_Create(short maximumtokensize);
short Lexical_Destroy(Lexical *lex);
short Lexical_Prime(Lexical *lex, char *file);
short Lexical_HaveSymbol(Lexical *lex, char ch);
short Lexical_NextToken(Lexical *lex);

#endif /* LEXICAL_H */

