// Lezette Leuterio and Hannah Main
// COP 3402, Fall 2020
// HW#3: Parser- Code Generator
// h file for code gernerator

#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "lex.h"
#include "parser.h"

typedef struct instruction{
    int opcode;
    char *op;
    int r;
    int l;
    int m;
} instruction;

instruction* generate_code(symbol *table, tokenStruct *list);

#endif