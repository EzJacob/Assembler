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
| 2 | addressing jump with parameters | This method of addressing is relevant and given For use only in jump commands (jmp, bne, jsr). The method is designed for convenient transfer of two parameters for using the destination jump. The first parameter will be passed in r6, and the second in register r7, (The previous contents of these registers run over). In this addressing method there are at most 3 additional information words in the instruction code. The first additional word is the label address to jump to. The second additional word is the encoding of the first parameter. The third extra word is the second parameter encoding. It is possible that both parameters will be coded to a common word, then there will only be 2 additional information words. A pair of bits from the field A, R, E are added to each of the additional words of the addressing method. | The operand consists of the name of a label you want to jump to, followed by the two parameters in parentheses, separated from each other by a comma. There are no spaces between the elements of the operand. You cannot specify more or less than two parameters. Each parameter can be an immediate number (similar to addressing method 0), or a label (similar to addressing method 1), or a register - (similar to addressing method 3 see definition later in the table). The bits of each parameter in the first code word will be determined according to the type of the parameter. The additional word will be coded similarly to the corresponding addressing method. If the first parameter is a register, it will be coded with an additional word, like a source register in addressing method 3. If the second parameter is a register, it will be coded like a destination register. If both parameters are registers, they will share a common word. | jmp L1(#5,N) In this example, the constant 5 is encoded in the third word of the instruction, and the address represented by the label N is encoded in the fourth word. The instruction jumps to label L1 and two parameters are passed: the constant 5 (in register r6), and the word that in address of N (in register r7). Another Example: jsr L1(r3,r5) , In this example, both parameters are registers, and they share the third word of the instruction code. |
| 3 | Direct register addressing | The operand is a register. If the register is used as a destination operand, another word of the command will contain the number of the register in the six bits 2-7. If the register is used as a source operand, it will be coded with an additional word that will contain the number of the register in the six bits 8-13. If the command has two operands, and both are registers, they will share one additional word in common, with bits 2-7 for the destination register, and bits 8-13 for the source register. To this representation, a pair of bits is added from the field A, R, E . Unused bits will contain 0. | The operand is the name of a register. | Example: mov r1,r2 In this example, the instruction copies the contents of register r1 into register r2. In this example the two operands are registers, which will be encoded into a common extra word. |

__________________________________________________________________________

**Assembly**

To convert an assembly file you need to enter the command: "./assembler" followed by the name of the files you want to assemble with space between them.<br>
For Example: ./assembler ex1 ex2 ex3 ex4<br>
The input will be a file with a ".as" extension. For example the file: ex1.as<br>
The output will a file with a ".am" extension and a file with ".ob" extension. For example: ex1.am ex1.ob<br>
The file with ".am" extension is a file simillar to the original input but if there were macros in the input file they will be parsed in the ".am" file.<br>
The file with ".ob" extension contains the binary code of the assembly.<br>
If in the input file there are entry or extern directives then there could be 2 more files as outputs with the extension of ".ent" and ".ext". For example: ex1.ent ex1.ext

__________________________________________________________________________

