/*
 * This file searches for errors for a specific command
 */
#include "general_functions.h"
#include "command_errors.h"
#include "syntax_errors.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_NUM (8191)
#define MIN_NUM (-8192)



/* This function checks if a string is a number.
 *
 * This function works by checking each character if it is a legitimate character that can make an integer.
 * The integer can contain '+' or '-' signs at the start.
 *
 * @Parameters
 *     str - a string
 *
 * @Return
 *     if the string represents a number the function returns 1. Otherwise, the function returns 0.
 *
 */
int isNum(char* str)
{
    int i;
    int len = strlen(str);

    if(*str != '+' && *str != '-' && !isdigit(*str))
        return 0;
    if((*str == '+' || *str == '-') && len == 1)
        return 0;
    if(*str == '0' && len > 1)
        return 0;
    if((*str == '+' || *str == '-') && *(str + 1) == '0' && len > 2)
        return 0;
    for(i = 1; i < len; i++)
    {
        if(!isdigit(str[i]))
            return 0;
    }

    return 1;
}

/* This function checks if a word is a symbol.
 *
 * This function works by checking if the first character is an alpha-bet and the rest of the characters are alpha-bet
 * or digits and if the symbol is limited to 30 characters.
 *
 * @Parameters
 *     word - a word in a line of code
 *     lineCount - the number of the line
 *
 * @Return
 *     If the word is a symbol the function returns 1. Otherwise, returns 0 and prints the error.
 */
int isSymbol(char* word, int lineCount)
{
    int i;

    if(!isalpha(word[0]))
    {
        fprintf(stdout, "Error in line %i: symbol needs to be start with an alpha-bet characters\n", lineCount);
        return 0;
    }
    if(strlen(word) >= MAX_WORD_LEN )
    {
        fprintf(stdout, "Error in line %i: symbol is limited to 30 characters\n", lineCount);
        return 0;
    }
    for(i = 1; i < strlen(word); i++)
    {
        if(!isdigit(word[i]) && !isalpha(word[i]))
        {
            fprintf(stdout, "Error in line %i: symbol can contain alpha-bet characters or numbers only\n", lineCount);
            return 0;
        }
    }

    return 1;
}

/* This function checks if an operand is a legitimate number.
 *
 * This function works by checking if the first character of the word is '#', if the rest of the word
 * is an integer and if the number is not beyond it's limit value.
 *
 * @Parameters
 *     word - a word in the line of code
 *     lineCount = the line number
 *
 * @return
 *   if the first character is not '#' then the function returns 0 - it means the function might be a legitimate symbol.
 *   If the function found error the function will print it and return -1.
 *   If the operand is a legitimate number the function will return 1.
 *
 */
int isOpNum(char* word, int lineCount)
{
    if(word[0] == '#')
    {
        if(!isNum(word + 1))
        {
            fprintf(stdout, "Error in line %i: after \'#\' there needs to be an integer only with an optional + or - sign.", lineCount);
            return -1;
        }
        else if(atoi(word + 1) > MAX_NUM || atoi(word + 1) < MIN_NUM)
        {
            fprintf(stdout, "Error in line %i: a number is beyond its value limit.\n", lineCount);
            return -1;
        }
        else
            return 1;
    }
    return 0;
}


/* This function checks for errors in a "mov", "add" and "sub" commands.
 *
 * This function works by checking if the source operand is a number or a symbol and if the target operand is a
 * symbol and not a number.
 *
 * @Parameters
 *     op1 - the source operand
 *     op2 - the target operand
 *     lineCount - the number of a line of code
 *
 * @Return
 *     The function returns 0 if the operands are legitimate for those commands. Otherwise, returns 1.
 */
int checkMovAddSubCommandErrors(char* op1, char* op2, int lineCount)
{
    if(isOpNum(op1, lineCount) == -1)
        return 1;
    if(!isOpNum(op1, lineCount))
    {
        if(isNum(op1))
        {
            fprintf(stdout, "Error in line %i: You need to put '#' before a number.\n", lineCount);
            return 1;
        }
        if (!isSymbol(op1, lineCount))
            return 1;
    }

    if(isOpNum(op2, lineCount) == 1)
    {
        fprintf(stdout, "Error in line %i: the second operand cannot be a number.\n", lineCount);
        return 1;
    }
    if(isOpNum(op2, lineCount) == -1)
        return 1;
    if(!isSymbol(op2, lineCount))
        return 1;

    return 0;
}
/* This function checks for errors in a "cmp" command.
 *
 * This function works by checking if the source operand and the target operand are a legitimate numbers of symbols.
 *
 * @Parameters
 *     op1 - the source operand
 *     op2 - the target operand
 *     lineCount - the number of a line of code
 *
 * @Return
 *     The function returns 0 if the operands are legitimate for the "cmp" command. Otherwise, returns 1.
 */
int checkCmpCommandErrors(char* op1, char* op2, int lineCount)
{
    if(isOpNum(op1, lineCount) == -1)
        return 1;
    if(isOpNum(op1, lineCount) == 0)
    {
        if(isNum(op1))
        {
            fprintf(stdout, "Error in line %i: You need to put '#' before a number.\n", lineCount);
            return 1;
        }
        if (!isSymbol(op1, lineCount))
            return 1;
    }

    if(isOpNum(op2, lineCount) == -1)
        return 1;
    if(isOpNum(op2, lineCount) == 0)
    {
        if(isNum(op2))
        {
            fprintf(stdout, "Error in line %i: You need to put '#' before a number.\n", lineCount);
            return 1;
        }
        if (!isSymbol(op2, lineCount))
            return 1;
    }
    return 0;
}
/* This function checks for errors in a "lea" command.
 *
 * This function works by checking if the source operand is a symbol and not a number or a register. Also, the
 * function checks if the target operand is a symbol but not a number.
 *
 * @Parameters
 *     op1 - the source operand
 *     op2 - the target operand
 *     lineCount - the number of a line of code
 *
 * @Return
 *     The function returns 0 if the operands are legitimate for the "lea" command. Otherwise, returns 1.
 */
int checkLeaCommandErrors(char* op1, char* op2, int lineCount)
{
    if(isOpNum(op1, lineCount) == -1)
        return 1;
    if(isOpNum(op1, lineCount) == 1)
    {
        fprintf(stdout, "Error in line %i: the first operand cannot be a number.\n", lineCount);
        return 1;
    }
    if(isRegister(op1))
    {
        fprintf(stdout, "Error in line %i: the first operand cannot be register.\n", lineCount);
        return 1;
    }
    if(!isSymbol(op1,lineCount))
        return 1;

    if(isOpNum(op2, lineCount) == -1)
        return 1;
    if(isOpNum(op2, lineCount) == 1)
    {
        fprintf(stdout, "Error in line %i: the second operand cannot be a number.\n", lineCount);
        return 1;
    }
    if(!isSymbol(op2,lineCount))
        return 1;

    return 0;
}
/* This function checks for errors in a command that takes one operand with no parameters.
 *
 * This function works by checking if the operand is a symbol but not a number.
 *
 * @Parameters
 *     op1 - the target operand
 *     lineCount - the number of a line of code
 *
 * @Return
 *     The function returns 0 if the operand is a legitimate for the commands that take one operand with no parameters. Otherwise, returns 1.
 */
int check1OpCommandErrors(char* op1, int lineCount)
{
    if(isOpNum(op1, lineCount) == -1)
        return 1;
    if(isOpNum(op1, lineCount) == 1)
    {
        fprintf(stdout, "Error in line %i: the first operand cannot be a number.\n", lineCount);
        return 1;
    }
    if(!isSymbol(op1,lineCount))
        return 1;

    return 0;
}
/* This function checks for errors in a "prn" command.
 *
 * This function works by checking if the operand is a number or a symbol.
 *
 * @Parameters
 *     op1 - the target operand
 *     lineCount - the number of a line of code
 *
 * @Return
 *     The function returns 0 if the operands are legitimate for the "prn" command. Otherwise, returns 1.
 */
int checkPrnCommandErrors(char* op1, int lineCount)
{
    if(isOpNum(op1, lineCount) == -1)
        return 1;
    if(isOpNum(op1, lineCount) == 0)
    {
        if(isNum(op1))
        {
            fprintf(stdout, "Error in line %i: You need to put '#' before a number.\n", lineCount);
            return 1;
        }
        if (!isSymbol(op1, lineCount))
            return 1;
    }
    return 0;
}
/* This function checks for errors in a "jmp", "bne" and "jsr" commands.
 *
 * This function works by checking if the operand has a legitimate symbol and with or without parenthesis that have
 * parameters in it.
 *
 * @Parameters
 *     op1 - the target operand
 *     lineCount - the number of a line of code
 *
 * @Return
 *     The function returns 0 if the operand is legitimate for those commands. Otherwise, returns 1.
 */
int checkJmpBneJsrCommandErrors(char* op1, int lineCount)
{
    int i = 0, j = 0, len = strlen(op1);
    char parametersCopy[MAX_LINE_LEN];

    if(isOpNum(op1, lineCount) == -1)
        return 1;
    if(isOpNum(op1, lineCount) == 1)
    {
        fprintf(stdout, "Error in line %i: the first operand cannot be a number.\n", lineCount);
        return 1;
    }

    /* checking there are not spaces or tabs in the operand */
    for(i = 0; i < len; i++)
    {
        if(op1[i] == ' ' || op1[i] == '\t')
        {
            fprintf(stdout, "Error in line %i: the operand cannot contain within it spaces or tabs.\n", lineCount);
            return 1;
        }
    }
    i = 0;

    /* getting the indexes of the parenthesis */
    while(op1[i] != '(' && op1[i] != '\0')
        i++;
    while(op1[j] != ')' && op1[j] != '\0')
        j++;

    /* the case that the operand gets only a symbol */
    if(i == len && j == len)
    {
        /* checking if there is a comma in the middle of the operand. (the general errors function should detect at the start and at the end) */
        for(i = 0; i < strlen(op1); i++)
        {
            if (op1[i] == ',')
            {
                fprintf(stdout, "Error in line %i: too many arguments.\n", lineCount);
                return 1;
            }
        }
        /* checking if the operand is not a symbol */
        if(!isSymbol(op1,lineCount))
            return 1;
        /* checking if the operand is a register */
        if(isRegister(op1))
        {
            fprintf(stdout, "Error in line %i: the symbol after the command can't be register.\n", lineCount);
            return 1;
        }
    }
    /* the case the operand gets parenthesis, but they are not properly used */
    else if(i == len || j == len || i > j)
    {
        fprintf(stdout, "Error in line %i: the use of parenthesis were not properly used.\n", lineCount);
        return 1;
    }
    /* the case there is text after the parenthesis */
    else if(op1[j + 1] != '\0')
    {
        fprintf(stdout, "Error in line %i: extra text after parenthesis.\n", lineCount);
        return 1;
    }
    /* the case the operand gets a symbol and parenthesis with parameters within it */
    else
    {
        /* copying to parameterCopy only the symbol without the other parameters */
        strcpy(parametersCopy, op1);
        parametersCopy[i] = '\0';

        /* checking if the first symbol is not a register */
        if(isRegister(parametersCopy))
        {
            fprintf(stdout, "Error in line %i: the symbol after the command can't be register.\n", lineCount);
            return 1;
        }

        /* removing the parenthesis in the operand */
        op1[i] = ' ';
        op1[j] = '\0';

        /* copying to the parameterCopy the parameters of the operand */
        strcpy(parametersCopy, op1 + i + 1);

        /* for example from op1 = "L1(r3,r4) the result will be parametersCopy = "r3,r4" */
        /* go to check "cmp" function because it has the same structure with the parameters */
        return (checkSyntax("cmp", parametersCopy , lineCount));
    }

    return 0;
}
/* This function checks for errors in a "rts" and "stop" commands.
 *
 * This function works by checking if after the command there is extra text.
 *
 * @Parameters
 *     op1 - text after the command
 *     lineCount - the number of a line of code
 *
 * @Return
 *     The function returns 0 if there is no extra test after the command "rts" or "stop". Otherwise, returns 1.
 */
int CheckRtsStopCommandErrors(char* op1, int lineCount)
{
    if(op1[0] != '\0')
    {
        trim(op1);
        fprintf(stdout, "Error in line %i: extra text after a command with no operands.\n", lineCount);
        return 1;
    }
    return 0;
}

