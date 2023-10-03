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

__________________________________________________________________________

Below is a description of the addressing methods in this machine:

| number | method address | The content of the additional words | The way of writing | Example |
| -------- | -------- | -------- | -------- | -------- |
| 0 | Immediate addressing | The additional word of the instruction contains the operand itself, which is a number represented in 12 bits, to them A pair of bits is added from the field A, R, E | The operand starts with # and after it and next to it A whole number appears in the base decimal | mov #-1,r2 In this example the operand The first of the command Given by address method immediate The teaching is writing the value -1 into r2 register |
| 1 | Direct addressing | The additional word of the instruction contains address of a word in memory. this word In memory is the operand. The addressee is represented in 12 bits to which a pair of bits is added from the field A, R, E | The operand is a label already stated or which will be stated later in the file. The statement is made by writing a label in the start of the instruction '.data' or '.string' or or at the beginning instruction of the program or using an operand of '.extern' directive | For example, the definition is given: x: .data 23 You can write an instruction: Dec. x In this example, the instruction Decreases your content by 1 the word in the address x in memory (the "variable" x) |
| 2 | Row 3, Col 2 | Row 3, Col 3 | aa | aaa |
| 3 | Row 3, Col 2 | Row 3, Col 3 | aa | aaa |

__________________________________________________________________________



