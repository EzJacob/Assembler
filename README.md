# Assembler
Assembler machine that converts assembly code to binary base code, written in C language.

**Explanation**

The CPU has 8 general registers with the names r0, r1, r2, r3, r4, r5, r6, r7.
The size of each general register is 14 bits.
There is only support for positive and negative integers.
Machine instruction structure:
13  12      11  10      9  8  7  6      5  4      3  2      1 0
parameter1  parameter2  opcode          operand   operand   E,R,A
                                        address   address 
                                        source    target



