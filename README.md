# PL0Compiler
A compiler that reads a program written in PL/0 and generates code for the Virtual Machine.

Lezette Leuterio and Hannah Main


This program attempts to create a Recursive Descent Parser and an Intermediate Code Generator for tiny PL/0. As well as a compiler driver to combine all of the compiler parts into one single program.


Example commands:

To compile and print only the lexeme table and the list of the lexemes:
        
        -”gcc driver.c lex.c parser.c codegen.c vm.c”
        -”./a.out input.txt -l”


To compile and print only the assembly code:
        
        -”gcc driver.c lex.c parser.c codegen.c vm.c”
        -”./a.out input.txt -a”


To compile and print only the VM execution trace:
        
        -”gcc driver.c lex.c parser.c codegen.c vm.c”
        -”./a.out input.txt -v”


To compile and print all types of output:

        -”gcc driver.c lex.c parser.c codegen.c vm.c”
        -”./a.out input.txt -l -a -v”


To compile and only print the source code:

        -”gcc driver.c lex.c parser.c codegen.c vm.c”
        -”./a.out input.txt”
