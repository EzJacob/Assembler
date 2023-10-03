#ifndef PROJ1_COMMAND_ERRORS_H
#define PROJ1_COMMAND_ERRORS_H

/* checking if a string represents an integer, if so returns 1. Otherwise, returns 0. */
int isNum(char* str);

/* checking if a word is a symbol, if so returns 1. Otherwise, returns 0 and prints the error. */
int isSymbol(char* word, int lineCount);

int isOpNum(char* word, int lineCount);

/* The function checks if the operands are legitimate for "mov", "add" and "sub" commands and returns 0 if so. Otherwise, returns 1. */
int checkMovAddSubCommandErrors(char* op1, char* op2, int lineCount);

/* The function checks if the operands are legitimate for the "cmp" command and returns 0 if so. Otherwise, returns 1. */
int checkCmpCommandErrors(char* op1, char* op2, int lineCount);

/* The function checks if the operands are legitimate for the "lea" command and returns 0 if so. Otherwise, returns 1. */
int checkLeaCommandErrors(char* op1, char* op2, int lineCount);

/* The function checks if the operand is a legitimate for a command that takes only one operand with no parameters and returns 0 if so. Otherwise, returns 1. */
int check1OpCommandErrors(char* op1, int lineCount);

/* The function checks if the operand is a legitimate for the "prn" command and returns 0 if so. Otherwise, returns 1. */
int checkPrnCommandErrors(char* op1, int lineCount);

/* The function checks if the operand is a legitimate for the "jmp", "bne" and "jsr" commands and returns 0 if so. Otherwise, returns 1. */
int checkJmpBneJsrCommandErrors(char* op1, int lineCount);

/* The function checks if "rts" and "stop" commands don't any arguments and returns 0 if so. Otherwise, returns 1. */
int CheckRtsStopCommandErrors(char* op1, int lineCount);


#endif
