InstructionSetSimulator
=======================

A simple instruction set simulator developed for the Systems Programming Lab course at IIT Patna.


NOTES:
The 'factorial.asm' file has been defined for the factorial of the number 4. It can be changed
by changing the number '4' in the line "cmp 4,R1" in the file.

The output factorial is situated in the register file, whose contents are printed on the screen
at each stage of execution. After the final stage of execution, the register file's first element
contains the factorial of 4, i.e., 24. Note that you can also see the contents of the other registers,
some of which were used to compute the factorial.

Commands To Run The Program:
1) make
2) ./out factorial.asm

Information About The Architecture Used:
Basically the architecture being simulated is similar to x86 - the instructions implemented are only those
that will be required for the factorial computation, but the program can be very easily extended to include any other
instruction as well. Opcodes used are mentioned in the decode() function.
