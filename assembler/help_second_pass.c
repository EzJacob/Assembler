/*
 * This file stores all the helping function for the second pass.
 */
#include "general_functions.h"
#include "help_second_pass.h"
#include <stdlib.h>


/* This function takes an integer and coding to binary machine code to the ".ob" file according
 * to given number of bits the user wants to code.
 *
 * This function works by initializing an unsigned integer tmp and setting its MSB according to the given NumOfBits.
 * Afterwards, the function checks if there's a bit that exist in num and in tmp and then coding to binary machine code
 * according to the result. Then the function moves the bits of tmp to the right and loops according to the
 * given NumOfBits.
 *
 * @Parameters
 *     object -".ob" file
 *     num - a value that is represented as an integer to code to the ".ob" file
 *     NumOfBits - number of bits to code
 */
void num2code(FILE* object, unsigned int num, int NumOfBits)
{
    int i;
    unsigned int tmp = 1;


    tmp <<= NumOfBits - 1;
    for(i = 0; i < NumOfBits; i++)
    {
        if(num & tmp)
            fputc('/', object);
        else
            fputc('.', object);
        tmp >>= 1;
    }
}
/* This function takes an operand and coding it to binary machine code according to the addressing mode.
 *
 * This function works by checking if the operand is a number or register or a symbol.
 * According to each case the function writes in the ".ob" file the code of addressing mode of the operand.
 *
 * @Parameters
 *     object - ".ob" file
 *     operand - operand in a line
 */
void operandType2code(FILE* object, char* operand)
{
    if(operand[0] == '#')
        fputs("..", object);
    else if(isRegister(operand))
        fputs("//", object);
    else
        fputs("./", object);
}
/* This function is coding all the operands to binary machine code from the source code.
 *
 * This function works by checking multiple occurrences: if there are 2 operands or 1 operand and if the 2 operands are registers.
 * Afterwards, converting the operands to a represented integer and using the "num2code" function to code the operands.
 *
 * @Parameters
 *     object - ".ob" file
 *     labels - the labels list
 *     IC - the instruction counter
 *     op1 - the source operand
 *     op2 - the destination operand
 * @Return
 *     if no error were found the function will return 0. Otherwise, the function returns 1.
 */
int operand2code(FILE* object, Node* labels, int IC, char* op1, char* op2, int lineCount)
{
    unsigned int num;
    char word[MAX_WORD_LEN];
    int errorFlag = 0;

    /* the case the two operands are registers */
    if(op1 != NULL && isRegister(op1) && isRegister(op2))
    {
        /* writing the code of both registers in to a single line */

        /* writing the decimal address of the both registers */
        sprintf(word, "0%d\t", IC + START_ADDRESS);
        fputs(word, object);

        /* writing each register as code in the same line */
        num = atoi(op1 + 1);
        num2code(object, num, 6);
        num = atoi(op2 + 1);
        num2code(object, num, 6);
        fputs("..\n", object);

        IC++;
    }
    /* the case the two operands are not both registers */
    else
    {
        /* checking if there is a source operand */
        if(op1 != NULL)
        {
            /* writing the code of the source operand */

            /* writing the decimal address of the source operand */
            sprintf(word, "0%d\t", IC + START_ADDRESS);
            fputs(word, object);

            /* the case if the operand is a register */
            if (isRegister(op1))
            {
                num = atoi(op1 + 1);
                num2code(object, num, 6);
                fputs("........\n", object);
            }
            /* the case if the operand is a number */
            else if (op1[0] == '#')
            {
                num = atoi(op1 + 1);
                num2code(object, num, 12);
                fputs("..\n", object);
            }
            /* the case if the operand is a symbol */
            else
            {
                /* getting the address of the symbol */
                num = getAdd(labels, op1);

                /* the case no decimal address was found */
                if(num == -2)
                {
                    errorFlag = 1;
                    fprintf(stdout, "Error in line %i: the source operand's symbol or the first parameter is not defined.\n", lineCount);
                }
                /* the case the symbol is external */
                else if (num == -1)
                    fputs("............./\n", object);
                /* the case the symbol is internal */
                else
                {
                    num += START_ADDRESS;
                    num2code(object, num, 12);
                    fputs("/.\n", object);
                }
            }

            IC++;
        }

        /* writing the code of the destination operand */

        /* writing the decimal address of the destination operand */
        sprintf(word, "0%d\t", IC + START_ADDRESS);
        fputs(word, object);

        /* the case if the operand is a register */
        if(isRegister(op2))
        {
            num = atoi(op2 + 1);
            fputs("......", object);
            num2code(object, num, 6);
            fputs("..\n", object);
        }
        /* the case if the operand is a number */
        else if(op2[0] == '#')
        {
            num = atoi(op2 + 1);
            num2code(object, num, 12);
            fputs("..\n", object);
        }
        /* the case if the operand is a symbol */
        else
        {
            /* getting the address of the symbol */
            num = getAdd(labels, op2);

            /* the case no decimal address was found */
            if(num == -2)
            {
                errorFlag = 1;
                fprintf(stdout, "Error in line %i: the destination operand's symbol or the second parameter is not defined.\n", lineCount);
            }
            /* the case the symbol is external */
            else if(num == -1)
                fputs("............./\n", object);
            /* the case the symbol is internal */
            else
            {
                num += START_ADDRESS;
                num2code(object, num, 12);
                fputs("/.\n", object);
            }
        }
        IC++;
    }
    if(errorFlag)
        return 1;
    return 0;
}




