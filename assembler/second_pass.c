/*
 * This file is the second pass of the assembler.
 */

#include "general_functions.h"
#include "help_second_pass.h"
#include <stdlib.h>
#include <string.h>

/* This function analyzes every piece of information from each line of the source code and writes to the ".ob" file
 * the decimal address and the binary machine code of it.
 *
 * This function works by going each line in the ".am" file and coding the source code to a decimal address and to
 * binary machine code into the ".ob" file.
 * If the function encounters ".data" or ".string" commands, the function writes only the binary machine code
 * to a temporary file and in the end the function writes the decimal address of the data and appends the binary
 * machine code from the temporary file accordingly.
 *
 * @Parameters
 *     fptr - the ".am" file
 *     labels - the labels list
 *     fname - the name of the ".as" file but with the extension of ".ob"
 * @Return
 *     If no errors were found the function returns 0. Otherwise, the function returns 0.
 */
int secondPass(FILE* fptr, Node** labels, char* fname)
{
    int IC = 0;
    int lineCount = 1;
    int errorFlag = 0;
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char wordCpy[MAX_LINE_LEN];
    char op1[MAX_WORD_LEN];
    char op2[MAX_WORD_LEN];
    char* token;
    unsigned int num = 0;
    int i = 0, j = 0;
    FILE* object;
    FILE* dataTemp;

    /* opening the object file in an append mode and a temporary file in write mode*/
    object = fopen(fname,"a+");
    dataTemp = tmpfile();
    if (dataTemp == NULL)
    {
        fprintf(stdout, "Failed to create temporary file at 'second_pass.c'.\n");
        return 1;
    }
    if (object == NULL)
    {
        fprintf(stdout, "Failed to open the object file at 'second_pass.c'.\n");
        return 1;
    }

    /* getting each line from the ".am" file */
    while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
    {
        i = j = 0;
        /* removing the new line character from the line and trimming the line */
        line[strlen(line) - 1] = '\0';
        trim(line);

        /* checking if the line had only white space characters, or it was a comment */
        if(strlen(line) == 0 || line[0] == ';')
        {
            lineCount++;
            continue;
        }
        /* getting the first word from the line */
        getWord(word,line);

        /* the case the first word is a label */
        if(isLabel(word, line))
        {
            /* copying the line without the first word and trimming it and getting the second word */
            setTextAfterIndex(line, strlen(word) + 1);
            trim(line);
            getWord(word,line);
        }
        /* copying the rest of the line without the current first word and trimming it */
        setTextAfterIndex(line, strlen(word));
        trim(line);

        /* the case the word is ".entry" or ".extern" */
        if(strcmp(word, ".entry") == 0 || strcmp(word, ".extern") == 0)
        {
            lineCount++;
            continue;
        }

        /* the case the word is ".data" */
        if(strcmp(word, ".data") == 0)
        {
            /* coding each number it's binary machine code to the dataTemp file */
            token = strtok(line,",");
            while(token != NULL)
            {
                strcpy(word, token);
                trim(word);
                num = atoi(word);
                num2code(dataTemp, num, 14);
                fputs("\n", dataTemp);

                token = strtok(NULL, ",");
            }
        }
        /* the case the word is ".string" */
        else if(strcmp(word, ".string") == 0)
        {
            /* copying to line the string without the quotation mark */
            line[strlen(line) - 1] = '\0';
            setTextAfterIndex(line, 1);

            /* coding each character it's binary machine code to the dataTemp file */
            for(i = 0; i <= strlen(line); i++)
            {
                num = line[i];
                num2code(dataTemp, num, 14);
                fputs("\n", dataTemp);
            }
        }
        /* the case of a command with 2 operands */
        else if((isCommand(word) <= 3 || isCommand(word) == 6) && isCommand(word) != -1)
        {
            /* coding the decimal address and the binary machine code of the command */
            sprintf(wordCpy, "0%d\t....", IC + START_ADDRESS);
            fputs(wordCpy, object);
            num2code(object, isCommand(word), 4);

            /* saving the two operands */
            token = strtok(line,",");
            strcpy(op1,token);
            trim(op1);
            token = strtok(NULL,",");
            strcpy(op2,token);
            trim(op2);

            /* coding the operands according to their addressing mode */
            operandType2code(object, op1);
            operandType2code(object, op2);
            fputs("..\n", object);

            IC++;

            /* coding the operands decimal address and their binary machine code */
            errorFlag = operand2code(object, *labels, IC, op1, op2, lineCount);

            /* if the two operands are registers IC will increase by 1. Otherwise, by 2 */
            if(isRegister(op1) && isRegister(op2))
                IC++;
            else
                IC += 2;
        }
        /* the case of a command with one operand that can have parameters */
        else if((isCommand(word) == 9 || isCommand(word) == 10 || isCommand(word) == 13) && isCommand(word) != -1)
        {
            /* coding the decimal address of the command */
            sprintf(wordCpy, "0%d\t", IC + START_ADDRESS);
            fputs(wordCpy, object);

            /* moving the index i until start of parenthesis are encountered or the end of the line */
            while(line[i] != '(' && line[i] != '\0')
                i++;

            /* the case the operand has parenthesis */
            if(i != strlen(line))
            {
                /* getting the first parameter */
                while(line[j] != ',')
                    j++;
                strncpy(op1, line + i + 1, j - i - 1);
                op1[j - i - 1] = '\0';

                /* getting the second parameter */
                strcpy(op2, line + j + 1);
                op2[strlen(op2) - 1] = '\0';

                /* cutting the line to only the first symbol */
                line[i] = '\0';

                /* coding the operands according to their addressing mode */
                operandType2code(object, op1);
                operandType2code(object, op2);
            }
            /* the case the operand doesn't have parenthesis coding 0 because no parameters */
            else
                fputs("....", object);

            /* coding the binary machine code of the command */
            num2code(object, isCommand(word), 4);

            /* the case the operand has parenthesis coding the machine code of the 2 addressing mode */
            if(j)
                fputs("../...\n", object);
            /* the case the operand doesn't have parenthesis coding the machine code of the 1 addressing mode */
            else
                fputs(".../..\n", object);

            IC++;

            /* coding the operands decimal address and their binary machine code */
            errorFlag = operand2code(object, *labels, IC, NULL, line, lineCount);

            IC++;

            /* the case the operand has parenthesis */
            if(j)
            {
                /* coding the parameters decimal address and their binary machine code */
                errorFlag = operand2code(object, *labels, IC, op1, op2, lineCount);

                /* if both parameters are register increasing IC by 1. Otherwise, by 2 */
                if(isRegister(op1) && isRegister(op2))
                    IC++;
                else
                    IC += 2;
            }

        }
        /* the case of a command with one operand that can't have parameters */
        else if(isCommand(word) != 14 && isCommand(word) != 15 && isCommand(word) != -1)
        {
            /* coding the decimal address of the command and part of its binary machine code */
            sprintf(wordCpy, "0%d\t....", IC + START_ADDRESS);
            fputs(wordCpy, object);

            /* getting the destination operand */
            strcpy(op2, line);

            /* coding the binary machine code of the command */
            num = isCommand(word);
            num2code(object, num, 4);

            /* coding the source operand */
            fputs("..", object);

            /* coding the operands according to their addressing mode */
            operandType2code(object, op2);

            /* coding the E,R,A */
            fputs("..\n", object);

            IC++;

            /* coding the operand's decimal address and their binary machine code */
            errorFlag = operand2code(object, *labels, IC, NULL, op2, lineCount);

            IC++;
        }
        /* the case of a command with no operands */
        else if(isCommand(word) != -1)
        {
            /* coding the decimal address of the command and part of its binary machine code */
            sprintf(wordCpy, "0%d\t....", IC + START_ADDRESS);
            fputs(wordCpy, object);

            /* coding the binary machine code of the command */
            num = isCommand(word);
            num2code(object, num, 4);

            /* coding only zeros because there are no operands and not using memory */
            fputs("......\n", object);

            IC++;
        }
        lineCount++;
    }

    rewind(fptr);
    rewind(dataTemp);

    /* coding the decimal address and the binary machine code of the data in to the object file */
    while(fgets(line, MAX_LINE_LEN, dataTemp) != NULL)
    {
        sprintf(word, "0%d\t%s", IC + START_ADDRESS, line);
        fputs(word, object);
        IC++;
    }

    /* checking if the instruction counter is beyond the memory limit */
    if(IC > 255)
    {
        errorFlag = 1;
        fprintf(stdout, "Error in the object file: the instruction counter its beyond the memory limit.\n");
    }

    /* closing the files and removing the temp file "dataTemp.ob" */
    fclose(object);
    fclose(dataTemp);
    freeList(labels);

    if(errorFlag)
        return 1;

    return 0;
}
