// Lezette Leuterio and Hannah Main
// COP 3402, Fall 2020
// HW#3: Parser- Code Generator
// h file for parser

#ifndef __PARSER_H
#define __PARSER_H

#include "lex.h"

typedef struct symbol{
    int kind;           // constants = 1, variables = 2, procedures = 3
    char name[12];      // name of the variable or reserved word
    int val;            // number value (ASCII value)
    int level;          // L, lex level (always assume 0 for HW3)
    int addr;           // M, source address
    int mark;           // remove or delete a name or variable, not needed anymore, 0: in use; 1: unavailable
} symbol;

symbol* parse(tokenStruct *list);

#endif
