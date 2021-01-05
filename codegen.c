// Lezette Leuterio and Hannah Main
// COP 3402, Fall 2020
// HW#3: Parser-Code Generator

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lex.h"
#include "parser.h"
#include "codegen.h"

#define CODE_SIZE 500


int listIndex = 0;  // index for list
int tableIndex = 1;  // index for table
int codeIndex = 0;  // index for code
instruction *code;
int savedCondition;
int savedJump;
int lexlevel = 0;
int numSymbols1 = 0;
int j;
int updateIndex;   // allows us to continue where we left off when assigning m for the current prodecure
int factorIndex;
int markIndex;  // allows us to continue where we left off when marking local variables to 1
int count = 0;
int count2 = 0;

void programPhoo(symbol *table, tokenStruct *list);
void blockPhoo(symbol *table, tokenStruct *list, int lexlevel);
void statementPhoo(symbol *table, tokenStruct *list, int lexlevel);
void conditionPhoo(symbol *table, tokenStruct *list, int lexlevel);
void expressionPhoo(symbol *table, tokenStruct *list, int regtoendupin, int lexlevel);
void termPhoo(symbol *table, tokenStruct *list, int regtoendupin, int lexlevel);
void factorPhoo(symbol *table, tokenStruct *list, int regtoendupin, int lexlevel);
void emit(int op, int r, int l, int m);

instruction* generate_code(symbol *table, tokenStruct *list){
    
    code = malloc(500 * sizeof(instruction));
    programPhoo(table, list);

    return code;
}

void programPhoo(symbol *table, tokenStruct *list){
    
    // count up the number of procedures in the symbol table (this includes main)
    // emit a JMP for every procedure
    // main's ID is 0
    int idx = 0;  // i is for assigning an "ID" to each procedure
    int i, j;
    for(j=0; j<500; j++){
        if(table[j].kind == 3){
            table[j].val = idx;
            idx++;
            emit(7, 0, 0, 0);
        }
    }
    blockPhoo(table, list, lexlevel);
    
    // we should now have the m values for the procedures and calls
    // fix all of our JMPs using the M value procedure in the symbol table
    
    // set up main's address first
    code[0].m = table[0].addr;

    for(i=1; i<idx; i++){   //start at line 1 in assembly code
        for(j=1; j<numSymbols1; j++){    //go through table to find corresponding i value
            if(table[j].kind == 3 && table[j].val == i){
                code[i].m = table[j].addr;
                
            }
        }
    }

    // fixing calls
    for(i=0; i<codeIndex; i++){
        if(code[i].opcode == 5){
            for(j=0; j<numSymbols1; j++){
                if(table[j].kind == 3 && (table[j].val == code[i].m))
                    code[i].m = table[j].addr; 
            }
        }
    }
	emit(9, 0, 0, 3);   //HALT

}

void blockPhoo(symbol *table, tokenStruct *list, int lexlevel){
    
    int numVars = 0;
    if(list[listIndex].tokenID == constsym){
  
        do{
            listIndex++; 
            numSymbols1++;
            table[tableIndex].mark = 0; //unmark the const in the symbol table
            tableIndex++;
            listIndex += 3;
        }
        while(list[listIndex].tokenID == commasym);
        listIndex++;
    }

    if(list[listIndex].tokenID == varsym){
        do{
            listIndex++;
            numVars++;
            numSymbols1++;
            table[tableIndex].mark = 0; //unmark the var in the symbol table
            tableIndex++;
            listIndex++; 
        }
        while(list[listIndex].tokenID == commasym);
        listIndex++;
    }


    // include procedure
    if(list[listIndex].tokenID == procsym){
        do{
            listIndex++;
            numSymbols1++;
            table[tableIndex].mark = 0; //unmark the procedure in the symbol table
            tableIndex++;
            listIndex += 2; 
            blockPhoo(table, list, lexlevel+1);
            emit(2, 0, 0, 0); //RTN
            listIndex++;
        }
        while(list[listIndex].tokenID == procsym);
    }

    updateIndex = numSymbols1 - count2;

    // update the symbol table entry for this procedure (current procedure) M = current code index
    // the current procedure is the inner most one
    for(j=updateIndex; j>=0; j--){
  
        if(table[j].kind == 3){
            table[j].addr = codeIndex;
            count2++;
            break;
        }
        count2++;
    }

    j--;
    updateIndex = j; //start back where we left off
    emit(6, 0, 0, 3 + numVars); //INC
    statementPhoo(table, list, lexlevel);

    // after finishing a procedure, we have to mark each of its local variables to be 1
    markIndex = numSymbols1 - count;

    if(table[markIndex].level != 0){
        do{ 
            table[markIndex].mark = 1;
           
            markIndex--;
            count++;
        }
        while(table[markIndex].kind != 3);
        
        count++;
        markIndex--;
    }
}

void statementPhoo(symbol *table, tokenStruct *list, int lexlevel){
 
    int savedIndex;
  
    if(list[listIndex].tokenID == identsym){
       
        // save the symbol table index of the entry unmarked and closest in lex level
        for(j=numSymbols1; j>0; j--){
            if(strcmp(table[j].name, list[listIndex].name) == 0 && table[j].mark != 1)
                break;
        }
        savedIndex = j;    // saving index
        listIndex += 2;
        expressionPhoo(table, list, 0, lexlevel);
        emit(4, 0, lexlevel - table[savedIndex].level, table[savedIndex].addr);  //STO    
    }

    //add call
    if(list[listIndex].tokenID == callsym){

        listIndex++;
        // save the symbol talbe index of the entry unmarked and closest in lext level
        for(j=numSymbols1; j>0; j--){
            if(strcmp(table[j].name, list[listIndex].name) == 0 && table[j].mark != 1)
                break;
        }
        savedIndex = j;    // saving index
        emit(5, 0, lexlevel - table[savedIndex].level, table[savedIndex].addr);  //CAL    
        listIndex++;
    }
    
    if(list[listIndex].tokenID == beginsym){
    
        listIndex++;
        statementPhoo(table, list, lexlevel);
        while(list[listIndex].tokenID == semicolonsym){
            listIndex++;
            statementPhoo(table, list, lexlevel);
        }
        listIndex++;
    }

    if(list[listIndex].tokenID == ifsym){
        listIndex++;
        conditionPhoo(table, list, lexlevel);
        savedIndex = codeIndex;
        emit(8, 0, 0, 0);   //JPC
        listIndex++;
        statementPhoo(table, list, lexlevel);

        // add if else
        if(list[listIndex].tokenID == elsesym){
            listIndex++;
            savedIndex = codeIndex;
            emit(7, 0, 0, 0);   //JMP emit the true case to skip over what we are doing
            code[savedIndex].m = codeIndex;   //fix the jpc from earlier
            statementPhoo(table, list, lexlevel);
            code[savedIndex].m = codeIndex;   //fix the JMP from earlier
        }
        else{
            // if there was no "else" keyword
            code[savedIndex].m = codeIndex;   //fix the jpc from earlier
        }
    }

    if(list[listIndex].tokenID == whilesym){
        listIndex++;
        savedCondition = codeIndex;
        conditionPhoo(table, list, lexlevel);
        listIndex++;
        savedJump = codeIndex;
        emit(8, 0, 0, 0);   //JPC
        statementPhoo(table, list, lexlevel);
        emit(7, 0, 0, savedCondition);  //JMP
        code[savedJump].m = codeIndex;   //fix the jpc from earlier
    }

    if(list[listIndex].tokenID == readsym){
        listIndex++;
        // save the symbol table index of the var entry unmarked and closest to current lexlevel
        for(j=numSymbols1; j>0; j--){
            if(table[j].kind == 2 && (strcmp(table[j].name, list[listIndex].name) == 0) && table[j].mark != 1)
                break;
        }
        savedIndex = j;    // saving index
        listIndex++;
        emit(9, 0, 0, 2);   //READ 
        emit(4, 0, lexlevel - table[tableIndex].level, table[savedIndex].addr);  //STO
    }

    if(list[listIndex].tokenID == writesym){
        
        listIndex++;
        expressionPhoo(table, list, 0, lexlevel);
        emit(9, 0, 0, 1);   //WRITE
        listIndex++;
        statementPhoo(table, list, lexlevel);
    }
    
}

void conditionPhoo(symbol *table, tokenStruct *list, int lexlevel){

    if(list[listIndex].tokenID == oddsym){
        listIndex++;
        expressionPhoo(table, list, 0, lexlevel);  // 0 means first expression in register 1
        emit(15, 0, 0, 0);  //ODD
    }
    else{
        expressionPhoo(table, list, 0, lexlevel);
        if(list[listIndex].tokenID == eqlsym){
            listIndex++;
            expressionPhoo(table, list, 1, lexlevel);
            emit(17, 0, 0, 1);  //EQL
        }
        if(list[listIndex].tokenID == neqsym){
            listIndex++;
            expressionPhoo(table, list, 1, lexlevel);
            emit(18, 0, 0, 1);  //NEQ
        }
        if(list[listIndex].tokenID == lessym){
            listIndex++;
            expressionPhoo(table, list, 1, lexlevel);
            emit(19, 0, 0, 1);  //LSS
        }
        if(list[listIndex].tokenID == leqsym){
            listIndex++;
            expressionPhoo(table, list, 1, lexlevel);
            emit(20, 0, 0, 1);  //LEQ
        }
        if(list[listIndex].tokenID == gtrsym){
            listIndex++;
            expressionPhoo(table, list, 1, lexlevel);
            emit(21, 0, 0, 1);  //GTR
        }
        if(list[listIndex].tokenID == geqsym){
            listIndex++;
            expressionPhoo(table, list, 1, lexlevel);
            emit(22, 0, 0, 1);  //GEQ
        }
    }
}

void expressionPhoo(symbol *table, tokenStruct *list, int regtoendupin, int lexlevel){

    if(list[listIndex].tokenID == plussym){
        listIndex++;
    }

    if(list[listIndex].tokenID == minussym){
     
        listIndex++;
        
        termPhoo(table, list, regtoendupin, lexlevel);   // the first term to addition and subtraction
     
        emit(10, regtoendupin, 0, 0);   //NEG
        
        while(list[listIndex].tokenID == plussym || list[listIndex].tokenID == minussym){
            if(list[listIndex].tokenID == plussym){
                listIndex++;
                termPhoo(table, list, regtoendupin + 1, lexlevel);
                emit(11, regtoendupin, regtoendupin, regtoendupin + 1); //ADD
            }
            if(list[listIndex].tokenID == minussym){
                listIndex++;
                termPhoo(table, list, regtoendupin + 1, lexlevel);
                emit(12, regtoendupin, regtoendupin, regtoendupin + 1); //SUB
            }
        }
        return;
    }

    termPhoo(table, list, regtoendupin, lexlevel);
  

    while(list[listIndex].tokenID == plussym || list[listIndex].tokenID == minussym){
        if(list[listIndex].tokenID == plussym){
            listIndex++;
            termPhoo(table, list, regtoendupin + 1, lexlevel);
            emit(11, regtoendupin, regtoendupin, regtoendupin + 1); //ADD
        }
        if(list[listIndex].tokenID == minussym){
            listIndex++;
            termPhoo(table, list, regtoendupin + 1, lexlevel);
            emit(12, regtoendupin, regtoendupin, regtoendupin + 1); //SUB
        }
    }
}

void termPhoo(symbol *table, tokenStruct *list, int regtoendupin, int lexlevel){
 
    factorPhoo(table, list, regtoendupin, lexlevel);

    while(list[listIndex].tokenID == multsym || list[listIndex].tokenID == slashsym){
        if(list[listIndex].tokenID == multsym){
            listIndex++;
            factorPhoo(table, list, regtoendupin + 1, lexlevel);
            emit(13, regtoendupin, regtoendupin, regtoendupin + 1); //MUL
        }
        if(list[listIndex].tokenID == slashsym){
            listIndex++;
            factorPhoo(table, list, regtoendupin + 1, lexlevel);
            emit(14, regtoendupin, regtoendupin, regtoendupin + 1); //DIV
        }
    }
}

void factorPhoo(symbol *table, tokenStruct *list, int regtoendupin, int lexlevel){

    if(list[listIndex].tokenID == identsym){

        // save the symbol table index of the var or const entry unmarked and closest in lexlevel
        for(j=numSymbols1; j>=0; j--){
            if(strcmp(table[j].name, list[listIndex].name) == 0 && table[j].mark != 1)
                factorIndex = j;
        } 

        if(table[factorIndex].kind == 1){
            emit(1, regtoendupin, 0, table[factorIndex].val);    //LIT
        }

        if(table[factorIndex].kind == 2){
            for(j=numSymbols1; j>=0; j--){
                if(strcmp(table[j].name, list[listIndex].name) == 0 && table[j].mark != 1)
                    break;
            }
            emit(3, regtoendupin, lexlevel - table[j].level, table[j].addr);       //LOD
        }
        
        listIndex++;
    }
    else if(list[listIndex].tokenID == numbersym){
        emit(1, regtoendupin, 0, list[listIndex].numberValue);  //LIT
        listIndex++;
    }
    else{   // other symbols
      
        listIndex++;
        expressionPhoo(table, list, regtoendupin, lexlevel);
        listIndex++;
    }
}

void emit(int op, int r, int l, int m){

    if(codeIndex > CODE_SIZE)
        printf("Error: too much code.");
    else
    {
        code[codeIndex].opcode = op;
		code[codeIndex].r = r;
		code[codeIndex].l = l;
		code[codeIndex].m = m;
		codeIndex++;
    }
}