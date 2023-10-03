#ifndef PROJ1_HELP_SECOND_PASS_H
#define PROJ1_HELP_SECOND_PASS_H

#include <stdio.h>
#include "linked_list.h"




/* coding to binary code the num according to NumOfBits to the object file */
void num2code(FILE* object, unsigned int num, int NumOfBits);

/* coding to binary code the addressing mode of the operand to the object file */
void operandType2code(FILE* object, char* operand);

/* coding the operands in a line of code to decimal address and to the binary code. Returns 0 if no errors were found. Otherwise, returns 1. */
int operand2code(FILE* object, Node* labels, int IC, char* op1, char* op2, int lineCount);


#endif
