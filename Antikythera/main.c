/*
 *
 *    main.c
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
#include "keystore.h"
#include "bytecodeassembler.h"
#include "types.h"

int main(int argc, char** argv) {
    Configuration* GlobalConfig = malloc(sizeof(Configuration));
    
    GlobalConfig->debugmode = TRUE;
    GlobalConfig->silentmode = FALSE;
    GlobalConfig->codespacesize = 1024 * 64;
    GlobalConfig->file = "test.ant";

    if(GlobalConfig->silentmode)
    {
        printf("Antikythera Abstract Machine\n");
        printf("Vesrion %d.%d\n", CONFIGUTATION_MAJORVERSION, CONFIGUTATION_MINORVERSION);
        printf("Copyright 2017 Steven Janes (www.perfectconsulting.co.uk)\n");
    }

    Types_Byte *codespace = malloc(GlobalConfig->codespacesize);

    
    ByteCodeAssembler *bca = ByteCodeAssembler_Create(GlobalConfig);
    bca->codespace = codespace; 
    
    if(!ByteCodeAssembler_Assemble(bca, "test.ant")) {
        free(codespace);
        printf("unable to assmeble file '%s'\n", GlobalConfig->file);
        
        return(EXIT_FAILURE);
    }
    
    ByteCodeAssembler_CodeMemoryDump(bca);
    
    free(GlobalConfig);
    return (EXIT_SUCCESS);
}

