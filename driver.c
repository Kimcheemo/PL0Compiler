// Lezette Leuterio and Hannah Main
// COP 3402, Fall 2020
// HW#3: Parser- Code Generator
// Where main is, to call all the different .c files

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lex.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"


// Takes in an input file and reads char by char creating a string of the whole file.
// It then prints the source code and passes that string to the lex analyzer, parser, and code generator
int main(int argc, char **argv){


    if (argc < 2){
        printf("ERROR: please include the file name");
        exit(0);
    }

    int k;
    int flagL = 0;
    int flagA = 0;
    int flagV = 0;
    for(k = 2; k < argc; k++) {
        if(strcmp(argv[k], "-l") == 0) 
            flagL = 1;
        if(strcmp(argv[k], "-a") == 0) 
            flagA = 1;
        if(strcmp(argv[k], "-v") == 0) 
            flagV = 1;
    }

    FILE *ifp = fopen(argv[1], "r");

    char *inputfile = malloc(500 * sizeof(char));   // an array that hold the characters of the input file
    char c = fgetc(ifp);
    int i = 0;

    while (1){
        inputfile[i++] = c;
        c = fgetc(ifp);
        if (c == EOF)
            break;
    }
    inputfile[i] = '\0';

    printf("\nSource Program: \n");
    printf("%s\n", inputfile);

    tokenStruct* list = lex_analyze(inputfile, flagL);
    if(list[0].tokenID != -1){

        symbol* table = parse(list);

        if(table[0].kind != -1)
        {
            instruction *code = generate_code(table, list);
            virtual_machine(code, flagA, flagV);
        }
    }

    return 0;
}