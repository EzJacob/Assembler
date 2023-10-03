/*
 * This file searches for syntax errors for a line of code
 */

#include "syntax_errors.h"
#include "general_functions.h"
#include "command_errors.h"
#include "general_errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This function checks for syntax errors in a line of code.
 *
 * This function works by first checking for general errors and then according to the command saving the operands and
 * checking for multiple errors for the ".data", ".string", ".extern" and ".entry" commands and if the command has
 * op-code the function checks errors for the operands and errors that could occur in them.
 *
 * @Parameters
 *     command - the command in the line of code
 *     line - the line of code
 *     lineCount - the number of the line of code
 *
 * @Return
 *     The function returns 0 if no errors were found. Otherwise, the function returns 1.
 */
int checkSyntax(char* command, char* line, int lineCount)
{
    char op1[MAX_LINE_LEN];
    char op2[MAX_LINE_LEN];
    char *token;
    int tokenCount = 0; /* number of arguments */

    /* checking for general errors in the line */
    if(checkGeneralErrors(command, line, lineCount))
        return 1;

    /* the case the command takes 2 operands */
    if(isCommand(command) != -1 && (isCommand(command) <= 3 || isCommand(command) == 6))
    {
        /* saving in token each argument seperated by a comma */
        token = strtok(line, ",");
        while(token != NULL)
        {
            tokenCount++;

            /* the first token is the first operand */
            if(tokenCount == 1)
            {
                strcpy(op1, token);
                trim(op1);
            }
            /* the second token is the second operand */
            if(tokenCount == 2)
            {
                strcpy(op2, token);
                trim(op2);
            }

            /* the case there are too many arguments */
            if(tokenCount > 2)
            {
                fprintf(stdout, "Error in line %i: too many arguments.\n", lineCount);
                return 1;
            }

            token = strtok(NULL, ",");
        }
        /* the case there are not enough arguments */
        if(tokenCount < 2)
        {
            fprintf(stdout, "Error in line %i: not enough arguments.\n", lineCount);
            return 1;
        }
        /* the case an operand is a saved word of a command name */
        if(isCommand(op1) != -1 || isCommand(op2) != -1)
        {
            fprintf(stdout, "Error in line %i: a symbol can't be with a name of a command.\n", lineCount);
            return 1;
        }
    }
    /* the case the command takes at most one operand */
    else if(isCommand(command) != -1)
    {
        /* copying to op1 the operand from the line of code */
        strcpy(op1,line);

        /* the case the command takes one operand */
        if(isCommand(command) <= 13 && line[0] == '\0')
        {
            fprintf(stdout, "Error in line %i: no argument after command\n", lineCount);
            return 1;
        }

        /* the case the operand is a saved word of a command */
        if(isCommand(op1) != -1)
        {
            fprintf(stdout, "Error in line %i: a symbol can't be with a name of a command.\n", lineCount);
            return 1;
        }

        /* the case the command doesn't take parameters */
        if(isCommand(command) != 9 && isCommand(command) != 10 && isCommand(command) != 13)
        {
            /* checking if the operand that could have parameters have more than 2 arguments */
            token = strtok(line, ",");
            token = strtok(NULL, ",");
            if (token != NULL)
            {
                fprintf(stdout, "Error in line %i: too many arguments.\n", lineCount);
                return 1;
            }
        }
    }

    /* checking if the command is not an opcode */
    if(isCommand(command) == -1)
    {
        /* the case the command is ".data" or ".string" */
        if(strcmp(command,".data") == 0 || strcmp(command,".string") == 0)
        {
            /* checking for syntax errors respected to the command */
            return checkSyntaxDataString(command, line, lineCount);
        }
            /* the case the command is ".entry" or".extern" */
        else if(strcmp(command,".entry") == 0 || strcmp(command,".extern") == 0)
        {
            /* checking for syntax errors respected to the command */
            return checkSyntaxEntryExtern(line, lineCount);
        }
            /* the case the command is not found */
        else
        {
            fprintf(stdout, "Error in line %i: command not found.\n", lineCount);
            return 1;
        }
    }
        /* the case of the command is "cmp" */
    else if(isCommand(command) == 1)
    {
        return checkCmpCommandErrors(op1, op2, lineCount);
    }
        /* the case of the command is "mov" or "add" or "sub" */
    else if(isCommand(command) <= 3)
    {
        return checkMovAddSubCommandErrors(op1,op2,lineCount);
    }
        /* the case of the command is "lea" */
    else if(isCommand(command) == 6)
    {
        return checkLeaCommandErrors(op1, op2, lineCount);
    }
        /* the case of the command is "jmp" or "bne" or "jsr" */
    else if(isCommand(command) == 9 || isCommand(command) == 10 || isCommand(command) == 13)
    {
        return checkJmpBneJsrCommandErrors(op1,lineCount);
    }
        /* the case of the command is "prn" */
    else if(isCommand(command) == 12)
    {
        return checkPrnCommandErrors(op1, lineCount);
    }
        /* the case of the command is "not" or "clr" or "inc" or "dec" or "red" */
    else if(isCommand(command) <= 13)
    {
        return check1OpCommandErrors(op1, lineCount);
    }
        /* the case of the command is "rts" or "stop" */
    else
    {
        return CheckRtsStopCommandErrors(op1, lineCount);
    }
}

/* This function checks syntax errors for the commands ".data" or ".string"
 *
 * This function works by first checking if there's an argument after the command.
 * If the command is ".data" it checks each variable that is seperated by a comma if it's a legitimate integer number
 * that is not beyond its max or min limits.
 * If the command is ".string" the function checks if there is for the defining string if it starts with quotation
 * mark and ends with a quotation mark.
 *
 * @Parameters
 *     command - the command in the line of code
 *     line - the line of code
 *     lineCount - the number of the line of code
 *
 * @Return
 *     The function returns 0 if no errors were found. Otherwise, the function returns 1.
 */
int checkSyntaxDataString(char* command, char* line, int lineCount)
{
    char* token;
    char number[MAX_LINE_LEN];
    char lineCpy[MAX_LINE_LEN];
    int len = strlen(line);

    /* copying the line to lineCpy */
    strcpy(lineCpy,line);

    /* checking if no text after ".data" or ".string" */
    if(len == 0)
    {
        fprintf(stdout, "Error in line %i: no argument after \".data\" or \".string\".\n", lineCount);
        return 1;
    }
    /* checking for errors if the command is .data */
    if(strcmp(command, ".data") == 0)
    {
        /* token stores each time a variable seperated by a comma */
        token = strtok(lineCpy, ",");
        while(token != NULL)
        {
            /* saving the variable from 'token' to 'number' and trimming it */
            strcpy(number, token);
            trim(number);

            /* checking if the variable is not an integer number */
            if(!isNum(number))
            {
                fprintf(stdout, "Error in line %i: illegal number was found when defining data.\n", lineCount);
                return 1;
            }
                /* checking if the number is beyond the value limit */
            else if(atoi(number) > 8191 || atoi(number) < -8192)
            {
                fprintf(stdout, "Error in line %i: a number is beyond its value limit.\n", lineCount);
                return 1;
            }
            token = strtok(NULL, ",");
        }
    }
    /* checking for errors if the command is .string */
    if(strcmp(command, ".string") == 0)
    {
        /* the case the defining string doesn't start with a quotation mark */
        if(line[0] != '\"')
        {
            fprintf(stdout, "Error in line %i: no \'\"\' at the start of defining string.\n", lineCount);
            return 1;
        }
            /* the case the defining string doesn't end with a quotation mark */
        else if(line[len - 1] != '\"')
        {
            fprintf(stdout, "Error in line %i: no \'\"\' at the end of defining string.\n", lineCount);
            return 1;
        }
    }

    return 0;
}

/* This function checks for syntax errors for a line of code that contains the command ".entry" or ".extern".
 *
 * This function works by checking if the command takes a legitimate symbol and there is no extra text afterwards
 *
 * @Parameters
 *     line - the line of code
 *     lineCount - the number of the line of code
 *
 * @Return
 *     The function returns 0 if no errors were found. Otherwise, the function returns 1.
 */
int checkSyntaxEntryExtern(char* line, int lineCount)
{
    char word[MAX_LINE_LEN];
    char lineCpy[MAX_LINE_LEN];

    /* copying to lineCpy the line of code that comes after the command and getting the first word of it. */
    strcpy(lineCpy, line);
    getWord(word, lineCpy);

    /* the case no argument after .extern */
    if(line[0] == '\0')
    {
        fprintf(stdout, "Error in line %i: no argument after command\n", lineCount);
        return 1;
    }

    /* checking if the word is not a symbol */
    if(!isSymbol(word, lineCount))
        return 1;

    /* checking if the word is a register */
    if(isRegister(word))
    {
        fprintf(stdout, "Error in line %i: a register can't be of type \".extern\" or \".entry\".\n", lineCount);
        return 1;
    }

    /* copying to lineCpy the text that comes after the word and trimming it */
    setTextAfterIndex(lineCpy, strlen(word));
    trim(lineCpy);

    /* checking if there is extra text after the symbol that comes after ".extern" or ".entry". */
    if(strlen(lineCpy) != 0)
    {
        fprintf(stdout, "Error in line %i: extra text after \".extern\" or \".entry\".\n", lineCount);
        return 1;
    }

    return 0;
}
