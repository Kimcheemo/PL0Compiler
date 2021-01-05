// Lezette Leuterio and Hannah Main
// COP 3402, Fall 2020
// HW#3: Parser- Code Generator
// h file for lexeme

#ifndef __LEX_H
#define __LEX_H

typedef enum{
    nulsym = 1, identsym, numbersym, plussym, minussym,
    multsym, slashsym, oddsym, eqlsym, neqsym, lessym, leqsym,
    gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
    periodsym, becomessym, beginsym, endsym, ifsym, thensym,
    whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
    readsym , elsesym
} token_type;

typedef struct tokenStruct{
    token_type tokenID;     // for reserved words and special symbols
    int numberValue;        // for digits
    char name[12];          // for identifier's name (max 11 cahracters)
} tokenStruct;

// declared function to call in lex.c
tokenStruct* lex_analyze(char* inputfile, int flag);

#endif
