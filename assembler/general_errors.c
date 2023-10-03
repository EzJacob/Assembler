/*
 * This file checks for general errors in a line of code
 */
#include "general_errors.h"
#include <stdio.h>
#include <string.h>


/* This function checks for general errors in a line of code.
 *
 * This function works by checking if there is a comma between the command and an operand, if in the end of the line
 * there is a comma and if there are consecutive commas in the line.
 *
 * @Parameters
 *     command - the command in the line of code
 *     line - the line of code
 *     lineCount - the number of the line of code
 *
 * @Return
 *     The function returns 0 if no errors were found. Otherwise, the function returns 1.
 */
int checkGeneralErrors(char* command, char* line, int lineCount)
{
    if(checkStartComma(command, line, lineCount))
        return 1;
    if(checkEndComma(line, lineCount))
        return 1;
    if(checkConsComma(line, lineCount))
        return 1;

    return 0;
}


/* This function check if there is a comma between a command and an operand in a line of code.
 *
 * This function works by checking if there is a comma at the end of the command, if there is a comma at the start of
 * the other arguments after the command and if there is a comma between the command and the other arguments when each
 * variable is not seperated by a white space character.
 *
 * @Parameters
 *     command - the command in a line of code
 *     line - the other arguments after the command
 *     lineCount - the number of the line of code
 *
 * @Return
 *     The function returns 1 if it detects a comma between the command and the rest of the line. Otherwise, returns 0.
 */
int checkStartComma(char* command, char* line, int lineCount)
{
    if(line[0] == ',' || command[0] == ',' || command[strlen(command) - 1] == ',' )
    {
        fprintf(stdout, "Error in line %i: illegal comma\n" , lineCount);
        return 1;
    }

    return 0;
}
/* This function checks for a comma in the end of a line of code.
 *
 * This function works by checking if the last character of a trimmed line of code has a comma.
 *
 * @Parameters
 *     line - line of code
 *     lineCount - the number of a line of code
 *
 * @Returns
 *     The function returns 1 if there is a comma in the end of a line of code. Otherwise, returns 0.
 */
int checkEndComma(char* line, int lineCount)
{
    if(strlen(line) > 0 && line[strlen(line) - 1] == ',')
    {
        fprintf(stdout, "Error in line %i: illegal comma\n", lineCount);
        return 1;
    }
    return 0;
}
/* This function checks if there are consecutive commas in a line of code.
 *
 * This function work by comparing two consecutive characters and checks if they are a comma in a line of code.
 *
 * @Parameters
 *     line - line of code
 *     lineCount - the number of the line
 *
 * @Returns
 *     The function returns 1 if there are consecutive commas in a line of code. Otherwise, returns 0.
 */
int checkConsComma(char* line, int lineCount)
{
    int i, j;
    int len = strlen(line);

    for(i = 1; i < len; i++)
    {
        if(line[i] == ',')
        {
            if (line[i] == line[i - 1])
            {
                fprintf(stdout, "Error in line %i: consecutive commas.\n", lineCount);
                return 1;
            }
            for(j = i - 1; j >= 0; j--)
            {
                if(line[j] != ' ' && line[j] != '\t' && line[j] != ',')
                    break;
                if(line[j] == ',')
                {
                    fprintf(stdout, "Error in line %i: consecutive commas.\n", lineCount);
                    return 1;
                }
            }
        }
    }
    return 0;
}
