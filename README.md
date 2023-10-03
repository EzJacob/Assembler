# Assembler
Assembler machine that converts assembly code to binary base code, written in C language.

<br><br>

**Explanation**
_______________________________________________________________________


The CPU has 8 general registers with the names r0, r1, r2, r3, r4, r5, r6, r7.

The size of each general register is 14 bits.

There is only support for positive and negative integers.
__________________________________________________________________________
Machine instruction structure:

13--12________11--10________9--8--7--6________5--4________3--2________1--0

param1_______param2_______opcode_________op-source___op-dest___E,R,A
__________________________________________________________________________

Command names: mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop

The commands code in decimal respectively: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15

__________________________________________________________________________

*bits 0-1 A,R,E:* These bits indicate the type of encoding, whether it is 'Absolute', 'External' or 'Relocatable'. 

A value of 00 means that the encoding is absolute. 

A value of 01 means that the encoding is external. 

A value of 10 means that the encoding requires repositioning. 

These bits are added only to encodings of instructions (not of data), and they are also added to all
The additional words that these codes have. 

__________________________________________________________________________

*Bits 2-3:* encode the address method number of the destination operand (destination operand).

__________________________________________________________________________

*Bits 4-5:* encode the number of the addressing method of the source operand (source operand).

__________________________________________________________________________

In this language there are four methods of addressing, whose number is 0, 1, 2 or 3.

Bits 10-13 are relevant only if it is addressing method number 2 (see explanation below), and otherwise is 0.
The bits characterize two parameters. Bits 10-11 are responsible for parameter 2, while bits 12-13 are responsible for parameter 1.
A detailed explanation appears later, in the discussion on methods of addressing.

If the parameter is an immediate number, the bits will get 00.

If the parameter is a register, the bits will get 11.

If the parameter is a label name, the bits will be set to 0

