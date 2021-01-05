// Lezette Leuterio and Hannah Main
// COP 3402, Fall 2020
// HW#3: Parser- Code Generator

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"
#include "lex.h"

int errorFlag = 0;
int nameFound = 0;
int i = 0;  // index for going through list
int j = 0;  // index for going through the table
symbol *table;
char identName[12];
int k;
int numSymbols = 1; // how to know how many variables to mark

void programFoo(tokenStruct *list);
void blockFoo(tokenStruct *list, int lexlevel);
int constFoo(tokenStruct *list, int lexlevel);
int varFoo(tokenStruct *list, int lexlevel);
int procedureFoo(tokenStruct *list, int lexlevel);
void statementFoo(tokenStruct *list, int lexlevel);
void conditionFoo(tokenStruct *list, int lexlevel);
void expressionFoo(tokenStruct *list, int lexlevel);
void termFoo(tokenStruct *list, int lexlevel);
void factorFoo(tokenStruct *list, int lexlevel);

symbol* parse(tokenStruct *list){
    
    table = malloc(500 * sizeof(symbol));

    programFoo(list);
    printf("No errors, program is syntactically correct.\n\n");
    return table;
    
}

void programFoo(tokenStruct *list){

    table[j].kind = 3;
    strcpy(table[j].name, "main"); 
    table[j].val = 0;
    table[j].level = 0;
    table[j].addr = 0;
    table[j].mark = 0;
	j++;
    blockFoo(list, 0);
    if(list[i].tokenID != periodsym){
        printf("Error: Period expected.");
        exit(0);
    }

}

void blockFoo(tokenStruct *list, int lexlevel){

    numSymbols += constFoo(list, lexlevel);
    numSymbols += varFoo(list, lexlevel);
    numSymbols += procedureFoo(list, lexlevel);

    // once there are no more identifiers to declare, we then move on to doing its statements
    statementFoo(list, lexlevel);
    
    // after finishing a procedure, we should mark its local variables to 1 so that it can no longer be accessed
    // mark the last numSymbols number of unmarked symbols ONLY for local variables (same lex level)
    for(k=numSymbols; k>=0; k--){
        if(table[k].level == lexlevel)
            table[k].mark = 1;
    }
}

int constFoo(tokenStruct *list, int lexlevel){

    int numConsts = 0;

    if(list[i].tokenID == constsym){
	
        do{
            i++;
            if(list[i].tokenID != identsym){
                printf("Error: const must be followed by an identifier.");
                exit(0);
            }
            strcpy(identName, list[i].name);       
            for(k=0; k<numSymbols; k++){  
                if(strcmp(table[k].name, identName) == 0 && table[k].mark == 0 && table[k].level == lexlevel){
                    printf("Error: identifier is already in the symbol table.");
                    exit(0);
                }
            }
            i++;
            if(list[i].tokenID != eqlsym){
                printf("Error: identifier must be followed by =.");
                exit(0);
            }
            i++;
            if(list[i].tokenID != numbersym){
                printf("Error: = must be followed by a number.");
                exit(0);
            }

            // if no errors add to the symbol table
            table[j].kind = 1;
            strcpy(table[j].name, identName);
            table[j].val = list[i].numberValue;
            table[j].level = lexlevel;
            table[j].addr = 0;
            table[j].mark = 0;
            numConsts++;
            i++;
			j++;
        }
        while(list[i].tokenID == commasym);

        if(list[i].tokenID != semicolonsym){
        
            printf("Error: semicolon is missing.");
            exit(0);
        }
        i++;
    }

    return numConsts;
}

int varFoo(tokenStruct *list, int lexlevel){
    
    int numVars = 0;

    if(list[i].tokenID == varsym){
      
        do{
            numVars++;
            i++;
            if(list[i].tokenID != identsym){
                printf("Error: var must be followed by an identifier.");
                exit(0);
            }
            for(k=0; k<numSymbols; k++){
                if(strcmp(table[k].name, list[i].name) == 0 && table[k].mark == 0 && table[k].level == lexlevel){
                    printf("Error: the identifier is already in the symbol table.");
                    exit(0);
                }
            }

            // if no errors add to the symbol table
            table[j].kind = 2;
            strcpy(table[j].name, list[i].name);
            table[j].val = 0;
            table[j].level = lexlevel;
            table[j].addr = numVars + 2;
            table[j].mark = 0;
            i++;
            j++;

        }
        while(list[i].tokenID == commasym);

        if(list[i].tokenID != semicolonsym){
            printf("Error: semicolon or comma is missing.");
            exit(0);
        }
        i++;
    }

    return numVars;
}

int procedureFoo(tokenStruct *list, int lexlevel){

    int numProcedures = 0;

    if(list[i].tokenID == procsym){

        do{
            i++;
            if(list[i].tokenID != identsym){
                printf("Error: procedure must be followed by an identifier.");
                exit(0);
            }

            // check to see if ident is already in the symbol table as unmarked and at the same level
            for(k=0; k<numSymbols; k++){
                if(strcmp(table[k].name, list[i].name) == 0 && table[k].mark == 0 && table[k].level == lexlevel){
                    printf("Error: the identifier is already in the symbol table.");
                    exit(0);
                }
            }
            
            // if no errors add to the symbol table
            table[j].kind = 3;
            strcpy(table[j].name, list[i].name);
            table[j].val = 0;
            table[j].level = lexlevel;
            table[j].addr = 0;
            table[j].mark = 0;
            numProcedures++;
            i++;
            j++;

            // every time a new procedure is called we should go up a level
            lexlevel++;
            

            if(list[i].tokenID != semicolonsym){
                printf("Error: incorrect symbol after procedure declaration.");
                exit(0);
            }
            
            i++;
            blockFoo(list, lexlevel);

            if(list[i].tokenID != semicolonsym){
                printf("Error: semicolon or comma is missing.");
                exit(0);
            }
            i++;
        }
        while(list[i].tokenID == procsym);
    }

    return numProcedures;
}

void statementFoo(tokenStruct *list, int lexlevel){

    // for when we do variable operations like x + y
    if(list[i].tokenID == identsym){
	
        // search the symbol table working backwards and find the latest unmarked VAR with the desired name
        for(k=numSymbols; k>=0; k--){ 
            if(strcmp(table[k].name, list[i].name) == 0 && table[k].mark == 0 && table[k].kind == 2)
                nameFound = 1;
        }
        if(nameFound != 1){   // if not in symbol table at all
            printf("Error: undeclared identifier.");
            exit(0);
        }
        nameFound = 0;  // reset to use again later
        i++;
      
        if(list[i].tokenID != becomessym){
            printf("Error: used = instead of :=");
            exit(0);
        }
        i++;
        expressionFoo(list, lexlevel);

        // Checks for semicolon after each statement line
        if(list[i].tokenID != semicolonsym){
            printf("Error: semicolon between statements missing.");
            exit(0);
        }
        return;
    }

    // add call
    if(list[i].tokenID == callsym){
     
        i++;
        // search the symbol table working backwards and find the latest unmarked PROCEDURE with the desired name
        for(k=numSymbols; k>=0; k--){ 
            if(strcmp(table[k].name, list[i].name) == 0 && table[k].mark == 0 && table[k].kind == 3)
                nameFound = 1;
        }
        if(nameFound != 1){   // if not in symbol table at all
            printf("Error: call must be followed by a defined identifier.");
            exit(0);
        }
        nameFound = 0;  // reset to use again later
        i++;
        return;
    }

    if(list[i].tokenID == beginsym){
     
        i++;
        statementFoo(list, lexlevel);
        while(list[i].tokenID == semicolonsym){
            i++;
            statementFoo(list, lexlevel);
        }
        if(list[i].tokenID != endsym){
            printf("Error: missing 'end' symbol.");
            exit(0);
        }
        i++;
        return;
    }

    if(list[i].tokenID == ifsym){
        i++;
        conditionFoo(list, lexlevel);
        if(list[i].tokenID != thensym){
            printf("Error: 'then' expected.");
            exit(0);
        }
        i++;
        statementFoo(list, lexlevel);
        // add if-else
        if(list[i].tokenID == elsesym){
            i++;
            statementFoo(list, lexlevel);
        }
        return;
    }

    if(list[i].tokenID == whilesym){
        i++;
        conditionFoo(list, lexlevel);
        if(list[i].tokenID != dosym){
            printf("Error: 'do' expected.");
            exit(0);
        }
        i++;
        statementFoo(list, lexlevel);
        return;
    }

    if(list[i].tokenID == readsym){
        i++;
        // search the symbol table working backwards and find the latest unmarked VAR with the desired name
        for(k=numSymbols; k>=0; k--){ 
            if(strcmp(table[k].name, list[i].name) == 0 && table[k].mark == 0 && table[k].kind == 2)
                nameFound = 1;
        }
        if(nameFound != 1){   // if not in symbol table at all
            printf("Error3: undeclared identifier.");
            exit(0);
        }
        nameFound = 0;  // reset to use again later
        i++;
        return;
    }

    if(list[i].tokenID == writesym){
        i++;
        expressionFoo(list, lexlevel);
        return;
    }
    return;
}

void conditionFoo(tokenStruct *list, int lexlevel){
  
    if(list[i].tokenID == oddsym){
        i++;
        expressionFoo(list, lexlevel);
    }
    else{
        expressionFoo(list, lexlevel);
        if(list[i].tokenID != eqlsym && list[i].tokenID != neqsym && list[i].tokenID != lessym && list[i].tokenID != leqsym && list[i].tokenID != gtrsym && list[i].tokenID != geqsym){
            printf("Error: relational operator expected.");
            exit(0);
        }
		i++;
    	expressionFoo(list, lexlevel);
    }    
}

void expressionFoo(tokenStruct *list, int lexlevel){
    
    if(list[i].tokenID == plussym || list[i].tokenID == minussym)
        i++;
    termFoo(list, lexlevel);
    while(list[i].tokenID == plussym || list[i].tokenID == minussym){
        i++;
        termFoo(list, lexlevel);
    }
}

void termFoo(tokenStruct *list, int lexlevel){

    factorFoo(list, lexlevel);
    while(list[i].tokenID == multsym || list[i].tokenID == slashsym){
        i++;
        termFoo(list, lexlevel);
    }
}

void factorFoo(tokenStruct *list, int lexlevel){

    if(list[i].tokenID == identsym){
        // search the symbol table working backwards and find the latest unmarked VAR OR CONST with the desired name
    
		for(k=numSymbols; k>=0; k--){ 
            if((strcmp(table[k].name, list[i].name) == 0 && table[k].mark == 0) && (table[k].kind == 1 || table[k].kind == 2))
                nameFound = 1;
        }
        if(nameFound != 1){   // if not in symbol table at all
            printf("Error: undeclared identifier.");
            exit(0);
        }
        nameFound = 0;  // reset to use again later
        i++;
    }
    else if(list[i].tokenID == numbersym)
        i++;
    else if(list[i].tokenID == lparentsym){
        i++;
        expressionFoo(list, lexlevel);
        if(list[i].tokenID != rparentsym){
            printf("Error: missing right parenthesis.");
            exit(0);
        }
        i++;
    }
    else{
        printf("Error: incorrect symbol.");
        exit(0);
    }
}