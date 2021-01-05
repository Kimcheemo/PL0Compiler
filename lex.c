// Lezette Leuterio and Hannah Main
// COP 3402, Fall 2020
// HW#2: Lexical Analyzer

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "lex.h"

tokenStruct* lex_analyze(char* inputfile, int flag){

    char* reservedWords[] = {"const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd"};
    char* specialSymbols[] = {"+", "-", "*", "/", "(", ")", "=", ",", ".", "<", ">", ";", ":"}; 

    char currentString[100];    // the token we are analyzing
    memset(currentString, 0, sizeof currentString);     // initializes all to zero

    // the tokens in our lexeme list
    tokenStruct* lexemeList = malloc(500 * sizeof(tokenStruct));

    int lookAhead = 0;  // if we need to look into the next character or not
    int i = 0;  // index for currentString
    int j = 0;  // index for reservedWords
    int listIndex = 0;  // index for the lexeme list
    int errorFlag = 0;  // if 1, there is an error; 0, there is no error
    int commentFlag = 0;    // if it is a comment
    int indexy;

    int endingIndex = strlen(inputfile);
    int z = 0;  // current index in inputfile string
    int x = inputfile[z++];  // holds our current char in the input file

    // evaluates char by char
    while(z <= endingIndex){
   
        // if it is a white space, tab, or newline ignore it
        if(x == ' '||x == '\t'||x == '\n'){
            x = inputfile[z++];
            lookAhead = 0;
            continue;
        }
        
        // if it is a letter, it can be an identifier or reserved word
        if(isalpha(x)){
      
            // resets characters in currentString
            memset(currentString, 0, sizeof currentString); 

            // add first character into the currentString
            i=0;
            currentString[i] = x;
            i++;
            lookAhead = 1;

            // keep scanning to get full token           
            while((isalpha(x = inputfile[z++]) || isdigit(x)) && (x != ' '||x != '\t'||x != '\n')){
               
                // check if possible identifier is too long: when length is 12
                if(i>10){
                    printf("\nError: identifier too long. Max of 11 characters.\n\n");
                    exit(0);        
                }
                else{
                    errorFlag = 0;
                    currentString[i] = x;
                }
                i++;
            }

            // compare the current token with the reservedWords array
            for(j=0; j<14; j++){

                if(strcmp(currentString, reservedWords[j]) == 0){
                    indexy = j; 
                    break;
                }
                else    // if it is not a reserved word
                    indexy = -1;
            }

            // assigns numerical value to current token
            switch(indexy){

                case 0: 
                        lexemeList[listIndex].tokenID = constsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;        
                case 1:
                        lexemeList[listIndex].tokenID = varsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 2:
                        lexemeList[listIndex].tokenID = procsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 3:
                        lexemeList[listIndex].tokenID = callsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 4:
                        lexemeList[listIndex].tokenID = beginsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 5:
                        lexemeList[listIndex].tokenID = endsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 6:
                        lexemeList[listIndex].tokenID = ifsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 7:
                        lexemeList[listIndex].tokenID = thensym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 8:
                        lexemeList[listIndex].tokenID = elsesym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 9:
                        lexemeList[listIndex].tokenID = whilesym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 10:
                        lexemeList[listIndex].tokenID = dosym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 11:
                        lexemeList[listIndex].tokenID = readsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 12:
                        lexemeList[listIndex].tokenID = writesym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                case 13:
                        lexemeList[listIndex].tokenID = oddsym;
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
                default:    // it is an identifier
                        if(errorFlag == 1){
                            printf("\nError: invalid identifier.\n\n");     // invalid identifier
                            exit(0);
                        }
                        else 
                            lexemeList[listIndex].tokenID = identsym;   // valid identifier
                        
                        strcpy(lexemeList[listIndex].name, currentString);
                        break;
            }
            listIndex++;
            errorFlag = 0;
        } 

        // if it is a digit
        else if(isdigit(x)){
     
            int num = x -'0';
            int y;
            int count = 1;
            lookAhead = 1;

            // check if digit is too long (can only be 5 digits long)   
            while(isdigit(x = inputfile[z++])){

                if(count>4){                                       
                    printf("\nError: Number is too large. Max of 5 digits.\n\n");
                    exit(0);
                }
                else{

                    errorFlag = 0;
                    y = x - '0';
                    num = 10 * num + y;
                    count++;
                   
                }   
            }
            // add our full digit into the lexeme list
            lexemeList[listIndex].numberValue = num;
           
            // if the next char is a letter, it is an invalid identifier
            i = 0;
            char invalidString[100];
            memset(invalidString, 0, sizeof invalidString);

            if(isalpha(x)){
                
                while(isalpha(x)){
                    printf("\nError: variable can not start with a number.\n\n");
                    exit(0);
                }
            }

            // if no error add digit into the lexemeList
            if(errorFlag != 1){
                lexemeList[listIndex].tokenID = numbersym;
                lexemeList[listIndex].numberValue = num;
            }
            listIndex++;
        }
        
        // it is a special character
        else{    

            lookAhead = 0;

            char str[2];
            memset(str, 0, sizeof str);

            char comment[5000];
            memset(comment, 0, sizeof comment);

            if(x != '/')
                str[0] = x;
            else if (x == '/')
                comment[0] = x;

            // compare scanned symbol with specialSymbol array to see which it is
            for(j=0; j<13; j++){
           
                if(x == '/'){   // for comments
                    if(strcmp(comment, specialSymbols[j]) == 0){
                        indexy = j; 
                        break;
                    }
                    else
                        indexy = -1; 
                }
                else{   // for everything else
                    if(strcmp(str, specialSymbols[j]) == 0){
                        indexy = j; 
                        break;
                    }
                    else
                        indexy = -1;  
                }   
            }

            // assigns numerical value to each token
            switch(indexy){

                case 0: 
                        lexemeList[listIndex].tokenID = plussym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;        
                case 1:
                        lexemeList[listIndex].tokenID = minussym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;
                case 2:
                        lexemeList[listIndex].tokenID = multsym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;

                // for "/" we need to consider it possibly being a comment
                case 3:
                        x = inputfile[z++];
                        comment[1] = x;
                        lookAhead = 1;
                    
                        // it is a comment, scan whole comment but do not inclue in lexeme list
                        if(comment[1] == '*'){
                          
                            commentFlag = 1; 
                            x = inputfile[z++];

                            while(commentFlag == 1){

                                if(x == '*'){   // possible end of comment
                                    x = inputfile[z++];

                                    if(x == '/'){   // end of comment
                                        commentFlag = 0;
                                        lookAhead = 0;
                                    }
                                    else{
                                        commentFlag = 1;
                                    }
                                }
                                else{   // still a comment
                                    x = inputfile[z++];  
                                }                                                            
                            }
                        }
                        else{   // it is a division sign so we add it in our list
                            comment[1] = '\0';
                            lexemeList[listIndex].tokenID = slashsym;
                            strcpy(lexemeList[listIndex].name, comment);
                            listIndex++;
                        }
                        break;
                case 4:
                        lexemeList[listIndex].tokenID = lparentsym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;
                case 5:
                        lexemeList[listIndex].tokenID = rparentsym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;
                case 6:
                        lexemeList[listIndex].tokenID = eqlsym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;
                case 7:
                        lexemeList[listIndex].tokenID = commasym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;
                case 8:
                        lexemeList[listIndex].tokenID = periodsym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;

                // for "<" we need to consider it being <= or <> (neqsym)
                case 9:
                        x = inputfile[z++];

                        if(x != ' ' && x != '\t' && x != '\n'){
       
                            str[1] = x;
                            lookAhead = 1;

                            // it is a <>
                            if (x == '>'){
                                lookAhead = 0;
                                lexemeList[listIndex].tokenID = neqsym;
                                str[2] = '\0';
                                strcpy(lexemeList[listIndex].name, str);
                                listIndex++;
                            }
                            // it is a <=
                            else if(x == '='){
                                lookAhead = 0;
                                str[2] = '\0';
                                lexemeList[listIndex].tokenID = leqsym;
                                strcpy(lexemeList[listIndex].name, str);
                                listIndex++;
                            }
                        }

                        // if the next character is a space, then it is a <
                        if(x == ' '||x == '\t'||x == '\n'){
                            lookAhead = 0;
                            lexemeList[listIndex].tokenID = lessym;
                            str[1] = '\0';
                            strcpy(lexemeList[listIndex].name, str);
                            listIndex++;
                        }
                        break;

                // for ">" we need to consider >=
                case 10:
                        x = inputfile[z++];
        
                        if(x != ' ' && x != '\t' && x != '\n'){

                            str[1] = x;
                            lookAhead = 1;

                            // it is >=
                            if(x == '='){
                                lookAhead = 0;
                                str[2] = '\0';
                                lexemeList[listIndex].tokenID = geqsym;
                                strcpy(lexemeList[listIndex].name, str);
                                listIndex++;
                            }
                        }
                        // if the next character is a space, it is a >
                        if(x == ' '||x == '\t'||x == '\n'){
                            lexemeList[listIndex].tokenID = gtrsym;
                            strcpy(lexemeList[listIndex].name, str);
                            listIndex++;
                            lookAhead = 0;
                        }
                        break;

                case 11:
                        lexemeList[listIndex].tokenID = semicolonsym;
                        strcpy(lexemeList[listIndex].name, str);
                        listIndex++;
                        break;

                // for ":=" but it is an invalid symbol if it does not have "=" after it
                case 12: 
                        x = inputfile[z++];
                        str[1] = x;
                        str[2] = '\0';
                 
                        // it is :=
                        if(str[1] == '='){
                            lexemeList[listIndex].tokenID = becomessym;
                            strcpy(lexemeList[listIndex].name, str);
                            listIndex++;
                        }
                        else{   // invalid symbol
                            printf("\nError: invalid symbol.\n\n");
                            exit(0);
                        }
                        break;

                // it is an invalid symbol
                default:    
                        if(x != '\r'){  
                          printf("\nError: invalid symbol.\n\n");
                          exit(0);
                        }
                        break;
            } 
        }
    
        // when lookAhead is 0, we want to read in a new currentString
        if(lookAhead == 0){
            x = inputfile[z++];
        }
    }

    if(flag == 1){

        // print Lexeme Table
        printf("\nLexeme Table:\n");
        printf("Lexeme\t\t\ttoken type\n");
        for(j=0; j<listIndex; j++){

            if(lexemeList[j].tokenID == 3) // numbers
                printf("%d\t\t\t%d\n", lexemeList[j].numberValue, lexemeList[j].tokenID);
            else 
                printf("%s\t\t\t%d\n", lexemeList[j].name, lexemeList[j].tokenID);

        }

        // print Lexeme List
        printf("\nLexeme List:\n");
        for(j=0; j<listIndex; j++){

            // if there is an error
            if(lexemeList[j].tokenID == -1 || lexemeList[j].tokenID == -2 || lexemeList[j].tokenID == -3 || lexemeList[j].tokenID == -4)
                errorFlag = 1;
            
            // no errors
            if(lexemeList[j].tokenID == 2)
                printf("%d %s ", lexemeList[j].tokenID, lexemeList[j].name);
            else if(lexemeList[j].tokenID == 3)
                printf("%d %d ", lexemeList[j].tokenID, lexemeList[j].numberValue);
            else if(lexemeList[j].tokenID != -1 && lexemeList[j].tokenID != -2 && lexemeList[j].tokenID != -3 && lexemeList[j].tokenID != -4)
                printf("%d ", lexemeList[j].tokenID);

        }
    }

    printf("\n\n");

    return lexemeList;
    
}
