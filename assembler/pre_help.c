/*
 * This file contains all the helping function that are used in the preprocessor function.
 */

#include "general_functions.h"
#include "pre_help.h"
#include <string.h>
#include <stdlib.h>

/* This function checks for errors in the definition of a macro.
 *
 * This function achieves its goal by going over each line in the file and checking for multiple errors.
 *
 * @Parameters
 *     fptr - the given file
 * @Return
 *     if an error is found the function will return 1.
 *     if no errors were found the function will return 0.
*/
int check_macro_errors(FILE* fptr)
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    int lineCount = 1;
    int flag = 0;

    while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
    {
        trim(line);
        getWord(word,line);
        if(strcmp("mcr", word) == 0)
        {
            setTextAfterIndex(line, strlen("mcr"));
            trim(line);
            if(line[0] == '\n')
            {
                fprintf(stdout, "Error in line %i: No name after defining macro.\n", lineCount);
                flag++;
            }
            else
            {
                getWord(word,line); /* macro name */
                setTextAfterIndex(line, strlen(word));
                trim(line);
                if(line[0] != '\n')
                {
                    fprintf(stdout, "Error in line %i: Extra text after defining macro.\n", lineCount);
                    flag++;
                }
                if(isCommand(word) != -1 || isRegister(word))
                {
                    fprintf(stdout, "Error in line %i: a macro name can't be a saved word such as a command name or a register.\n", lineCount);
                    flag++;
                }
            }
        }
        else if(strcmp("endmcr", word) == 0)
        {
            setTextAfterIndex(line, strlen("endmcr"));
            trim(line);
            if(line[0] != '\n' && line[0] != '\0' )
            {
                fprintf(stdout, "Error in line %i: Extra text after 'endmcr' command.\n", lineCount);
                flag++;
            }
        }
        lineCount++;
    }
    rewind(fptr);
    if(flag)
        return 1;
    return 0;
}
/* This function copies the contents of one file without the definition of a macro and its body to another file.
 *
 * This function achieves its goal by going each line in the source file and copying each line to the new file
 * unless encounter by the start of a macro definition until the end of the definition.
 *
 * @Parameters
 *     dest - the file that the contents will be copied to
 *     src - the file with the contents
 */
void copyWithoutDefMc(FILE* dest, FILE* src)
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    int flag = 0 , j;

    while(fgets(line, MAX_LINE_LEN, src) != NULL)
    {
        j = 0;
        while(line[j] == ' ' || line[j] == '\t')
            j++;

        getWord(word,line + j);
        if(strcmp("mcr", word) != 0 && flag == 0)
            fprintf(dest, "%s", line);
        else
            flag = 1;
        if(strcmp("endmcr", word) == 0)
            flag = 0;
    }
    fputc('\n', dest);
    rewind(dest);
    rewind(src);
}

/* This function gets every macro name and its body from a file.
 *
 * This function accomplishes its goal by going over each line from the file and searching for the key word "mcr",
 * getting the name of the macro that comes after "mcr" word and then copying the body of the macro until the key word "endmcr"
 * is encountered
 *
 * @Parameters
 *     fptr - pointer to the file the user gave
 *     macro_name - 2 dimensional char array to save the macros name's
 *     macro_text - 2 dimensional char array to save the macros body
 */
void getMacros(FILE* fptr, mcTable* macroTable)
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char** macro_name = macroTable->mcName;
    char** macro_text = macroTable->mcText;
    int flag = 0;
    int i = 0;

    while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
    {
        trim(line);
        getWord(word,line);
        if(strcmp("mcr", word) == 0)
        {
            /* copying the macro name */
            setTextAfterIndex(line, strlen("mcr"));
            trim(line);
            getWord(word, line);
            strcpy(macro_name[i] , word);

            flag = 1;
        }
        else if(strcmp("endmcr", word) == 0)
        {
            if(strlen(macro_text[i]) > 0 && macro_text[i][strlen(macro_text[i]) - 1] == '\n')
                macro_text[i][strlen(macro_text[i]) - 1] = '\0';
            i++;
            flag = 0;
        }
        else if(flag)
        {
            /* copying the macro text */
            strcat(macro_text[i],line);
        }
    }
    rewind(fptr);
}
/* This function replaces every encountered macro name with its body.
 *
 * This function achieves its goal by going every word in the file (there are no definitions of macros in this file)
 * and if the word is a macro name so the function will paste its body in to the new file. Otherwise, the function will just
 * write the word in to the new file.
 *
 * @Parameters
 *      temp - the old file without the definitions of macros
 *      fname_am - the new file with the extension ".am"
 *      macro_name - 2 dimensional char array that has all the defined macro names
 *      macro_text - 2 dimensional char array that has all the defined macro bodies
 *      mc_count - the amount of defined macros
 */
void PastingMacros(FILE* temp, FILE* fname_am, mcTable* macroTable)
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char** macro_name = macroTable->mcName;
    char** macro_text = macroTable->mcText;
    int mc_count = *(macroTable->mcCount);
    int i = -1;
    int j = 0;



    while(fgets(line, MAX_LINE_LEN, temp) != NULL)
    {
        while(line[0] != '\n' && line[0] != '\0')
        {
            /* getting the index of non-white character */
            j = 0;
            while(line[j] == ' ' || line[j] == '\t')
                j++;

            /* setting the line to start from a non-white character */
            setTextAfterIndex(line, j);

            getWord(word, line);

            /* setting the line to start from after the word */
            setTextAfterIndex(line, strlen(word));

            /* checking if the word is a macro name and saving to index where the macro is in the macro table */
            i = isWordMacro(word, macro_name, mc_count);

            /* the case the word is a macro */
            if(i != -1)
                fputs(macro_text[i], fname_am);
            /* the case a word isn't a macro */
            else
                fputs(word, fname_am);

            /* the case the line doesn't end with '\n'  */
            if(line[0] != '\n' && line[0] != '\0')
            {
                /* writing the white characters of the line until non-white character appears in the line */
                j = 0;
                while(line[j] == ' ' || line[j] == '\t')
                    fputc(line[j++], fname_am);
            }
        }
        /* the case the line ended with a new line */
        if(line[0] == '\n')
            fputc('\n', fname_am);
    }
    fputc('\n', fname_am);

    rewind(temp);
    rewind(fname_am);
}
/* This function is checking if a word is a saved macro name.
 *
 * This function achieves its goal by going over every saved macro name and comparing it to the given word
 *
 * @Parameters
 *     word - the given word to compare to every saved macro name
 *     macro_name - 2 dimensional char array that has all the defined macro names
 *     mc_count - the amount of defined macros that are in the file
 *
 * @Return
 *     If a word is a macro name the function will return the index of where it is saved in the 'macro_name' 2D array.
 *     Otherwise, the function will return -1
*/
int isWordMacro(char* word, char** macro_name, int mc_count)
{
    int i;

    for(i = 0; i < mc_count; i++)
    {
        if(strcmp(word, macro_name[i]) == 0)
            return i;
    }
    return -1;
}
/* This function allocates memory for the macro table
 *
 * This function achieves its goal first going over each line in the file and counting the number of occurrences of "mcr".
 * According to the amount that was counted the function will allocate memory for "mcCount" and "mcName".
 * Afterwards, the function again going over each line and counting how much lines there are between "mcr" and "endmcr"
 * and according to that the function will allocate memory to "mcText" that is correlated to a specific "mcName".
 *
 * @Parameters
 *     fptr - the file the user gave with the extension of ".as"
 *     macroTable - a pointer to the macro table
 */
void allocateMacroTable(FILE* fptr, mcTable* macroTable)
{
    int i = 0;
    int lineCount = 1;
    int flag = 0;
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];

    /* getting the amount of macros that are being defined in the file */
    macroTable->mcCount = (int*) malloc(sizeof(int));
    *(macroTable->mcCount) = 0;
    while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
    {
        trim(line);
        getWord(word,line);
        if(strcmp("mcr", word) == 0)
            (*(macroTable->mcCount))++;
    }
    rewind(fptr);

    /* allocating memory to the macro text and macro name */
    macroTable->mcText = (char**) malloc(sizeof(char*) * (*(macroTable->mcCount) + 1));
    macroTable->mcName = (char**) malloc(sizeof(char*) * (*(macroTable->mcCount) + 1));

    while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
    {
        trim(line);
        getWord(word,line);
        if(strcmp("mcr", word) == 0)
        {
            setTextAfterIndex(line, strlen(word));
            trim(line);
            getWord(word, line);
            macroTable->mcName[i] = (char *) malloc(sizeof(char) * (strlen(word) + 1));

            flag = 1;
        }
        else if(strcmp("endmcr", word) == 0)
        {
            macroTable->mcText[i] = (char *) malloc(sizeof(char) * MAX_LINE_LEN * lineCount);
            strcpy(macroTable->mcText[i++],"");

            flag = 0;
			lineCount = 1;
        }
        else if(flag)
            lineCount++;
    }
    rewind(fptr);
}
/* This function frees the memory for a macro table
 *
 * This function accomplishes its goal by going over each macro name and macro body and freeing the memory for those variables
 * and then freeing the memory for the double char pointer of the macro name and macro text
 * and in the end freeing the int pointer of the amount of macros
 *
 * @Parameters
 *     macroTable - a pointer to the macro table
 */
void freeMacroTable(mcTable* macroTable)
{
    int i;
    for(i = 0; i < *(macroTable->mcCount); i++)
    {
        free(macroTable->mcName[i]);
        free(macroTable->mcText[i]);
    }
    free(macroTable->mcName);
    free(macroTable->mcText);
    free(macroTable->mcCount);
}



