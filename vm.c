// Lezette Leuterio and Hannah Main
// HW1: P-Machine

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "codegen.h"
#include "vm.h"

#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500


int sp = MAX_STACK_HEIGHT;
int bp;
int pc = 0;
int* rf;
int* stack;
struct instruction ir;
int oldPC = 0;

void fetch(instruction *code);
void switchMachine(instruction ir);
void printList(instruction* assem, int index);

// helps find a different AR some L levels up
int base(int l, int base, int* stack){

    int b1; //find base L levels up
    b1 = base;
    while (l>0){
        b1 = stack[b1];
        l--;
    }

    return b1;
}

void virtual_machine(instruction *code, int flagA, int flagV){

    int i;
    bp = sp - 1;
    rf = (int*) calloc(8, sizeof(int));  
    stack = (int*) calloc(MAX_STACK_HEIGHT, sizeof(int)); 

    if(flagA == 1){
        printf("Line\tOP   R   L   M\n");
        for(i=0; i<100; i++){
        printList(code, i);
        }
    }

    if(flagV == 1){
        // print initial values
        printf("\t\t\t\t\t pc     bp     sp\n");
        printf("Initial values\t\t\t\t 0     999     1000\n");
        printf("Registers: 0 0 0 0 0 0 0 0\n");
        printf("Stack: 0 0 0 0 0 0 0 0 0 0\n\n");

        for(i=0; i<500; i++){
            fetch(code);
            switchMachine(ir); //execution
           
        }
    }
}

void printList(instruction* assem, int index){
 
    int op = assem[index].opcode;

    switch(op){
        case 1:
            printf("%d\tlit  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 2:
            printf("%d\trtn  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 3:
            printf("%d\tlod  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 4:
            printf("%d\tsto  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 5:
            printf("%d\tcal  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 6:
            printf("%d\tinc  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 7:
            printf("%d\tjmp  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 8:
            printf("%d\tjpc  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 9:
            printf("%d\tsys  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 10:
            printf("%d\tneg  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 11:
            printf("%d\tadd  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 12:
            printf("%d\tsub  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 13:
            printf("%d\tmul  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 14:
            printf("%d\tdiv  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 15:
            printf("%d\todd  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 16:
            printf("%d\tmod  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 17:
            printf("%d\teql  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 19:
            printf("%d\tlss  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 20:
            printf("%d\tleq  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 21:
            printf("%d\tgtr  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        case 22:
            printf("%d\tgeq  %d   %d   %d\n", index, assem[index].r, assem[index].l, assem[index].m);
            break;
        default:
            break;
    }
}

// retreives the instruction to be executed
void fetch(instruction *code){

        //Fetch current instruction
        ir.opcode = code[pc].opcode;
        ir.r = code[pc].r;
        ir.l = code[pc].l;
        ir.m = code[pc].m;

        //Increment the Program Counter before we start execution.
        //Program Counter points to the NEXT instruction.
        pc++;

        //Return to main function.
        return;
}

void switchMachine(instruction ir){

    int op = ir.opcode;
    int r = ir.r;
    int l = ir.l;
    int m = ir.m;
    int i;
    int oldbp;
    int savedpc;

 
    switch(op){

        // LIT R, 0, M: Loads a constant value M into register R
        case 1:
            rf[r] = m;

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d lit %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        // RTN 0, 0, 0: Returns from a subroutine and restore the caller environment.
        case 2:
            sp = bp + 1;
            bp = stack[sp-2];
            pc = stack[sp-3];

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d rtn %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        // LOD R, L, M: Load value into selected register from the stack location at offset M from L levels up
        case 3:
            rf[r] = stack[base(l, bp, stack) - m];

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d lod %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;
        
        // STO R, L, M: Store value from a selected register from the stack location at offset M from L levels up
        case 4:
            stack[base(l, bp, stack) - m] = rf[r];

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d sto %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        // CAL 0, L, M: Call procedure at index M (generates new AR and PC <- M)    
        case 5:      
            oldbp = sp - 1;
            savedpc = m;
            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d cal %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, savedpc, oldbp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");

            stack[sp-1] = base(l, bp, stack);  //static link
            stack[sp-2] = bp;                  //dynamic link
            stack[sp-3] = pc;              //return address
            bp = sp - 1;
            pc = m;

            break;
        
        // INC 0, 0, M: Allocate M memory words (increment sp by m).
        case 6:     
            sp = sp - m;

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d inc %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){
                printf("%d ", stack[i]);


                // if(i == bp+1)
                // {
                //     printf("| ");
                // }

            }
            printf("\n\n");
            break;
        
        // JMP 0, 0, M: Jump to instruction M (PC <- M)
        case 7:
            pc = m;
            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d jmp %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;
        
        // JPC R, 0, M: jumps to M if R1 = 0.
        case 8:
            if(rf[r] == 0)
                pc = m; 

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d jpc %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;
        
        // SYS (3 types)
        case 9:
            // writes a register to the screen
            if(m == 1){
                printf("\nR[%d] = %d\n", r, rf[r]);
            }
            //read in value from screen to register
            if(m == 2){
                printf("\nEnter a number to store in the register.\n");
                scanf("%d", &rf[r]);
            }
            //ends the program
            if(m == 3){

                printf("\t\t\t\t\t pc     bp     sp\n");
                printf("%d sys %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
                printf("Registers: ");
                for(i=0; i<8; i++)
                    printf("%d ", rf[i]);
                printf("\n");
                printf("Stack: ");
                for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }

                return;    
            }

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d sys %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //NEG
        case 10:
            rf[r] = 0 - rf[r];

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d neg %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //ADD
        case 11:
            rf[r] = rf[l] + rf[m];

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d add %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //SUB
        case 12:
            rf[r] = rf[l] - rf[m];

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d sub %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //MUL
        case 13:
            rf[r] = rf[l] * rf[m];

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d mul %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //DIV
        case 14:
            rf[r] = ((rf[l])/(rf[m]));

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d div %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //ODD
        case 15:
            rf[r] = rf[r] % 2;

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d odd %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //MOD
        case 16:
            rf[r] = (rf[l])%(rf[m]);
          
            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d mod %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //EQL
        case 17:
            if(rf[l] == rf[m])
                rf[r] = 1;
            else
                rf[r] = 0;

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d eql %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //NEQ
        case 18:
            if(rf[l] != rf[m])
                rf[r] = 1;
            else
                rf[r] = 0;
            
            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d neq %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //LSS
        case 19:
            if(rf[l] < rf[m])
                rf[r] = 1;
            else
                rf[r] = 0;
            
            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d lss %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //LEQ
        case 20:
            if(rf[l] <= rf[m])
                rf[r] = 1;
            else
                rf[r] = 0;

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d leq %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //GTR
        case 21:
            if(rf[l] > rf[m])
                rf[r] = 1;
            else
                rf[r] = 0;

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d gtr %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;

        //GEQ
        case 22:
            if(rf[l] >= rf[m])
                rf[r] = 1;
            else
                rf[r] = 0;

            printf("\t\t\t\t\t pc     bp     sp\n");
            printf("%d geq %d %d %d\t\t\t\t %d     %d     %d\n", oldPC, r, l, m, pc, bp, sp);
            printf("Registers: ");
            for(i=0; i<8; i++)
                printf("%d ", rf[i]);
            printf("\n");
            printf("Stack: ");
            for(i=999; i>=sp; i--){    
                printf("%d ", stack[i]);
                if(i == bp+1)
                {
                    printf("| ");
                }
            }
            printf("\n\n");
            break;
    }
    oldPC = pc;
}


