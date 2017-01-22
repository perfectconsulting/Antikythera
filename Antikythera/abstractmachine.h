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


#ifndef ABSTRACTMACHINE_H
#define ABSTRACTMACHINE_H

#include "types.h"


typedef struct abstractmachine {
    Configuration       *config;
    
    Types_Byte          *codespace;
    Types_Byte          *dataspace;
    
} AbstractMachine;

AbstractMachine *AbstractMachine_Create(Configuration *config);
short AbstractMachine_Destrory(AbstractMachine *mac);
short AbstractMachine_Execute(AbstractMachine *mac);
#endif /* ABSTRACTMACHINE_H */

