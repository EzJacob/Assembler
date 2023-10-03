/*
 * This file is the preprocessor of the assembler.
*/
#include "pre_help.h"

/* This function is getting all the defining macros and its text from the file with the extension of ".as"
 * and searches for all the occurrences of macros in the file and pasting the text accordingly to a new file with a ".am" extension.
 *
 * This function has multiple steps that are divided by helping functions:
 * 1. checking for errors when defining macros
 * 2. allocating memory for the macro table
 * 3. saving the macros to the macro table
 * 4. copying the text from the ".as" file without the defining macros to the temp file
 * 5. copying the text from the temp file and pasting the macro in every occurrence of a macro name to the ".am" file
 * 6. closing all the open files, removing the temp file and deallocating the macro table
 *
 * @Parameters
 *     fptr - the file the user gives with the extension of ".as"
 *     fname - the name of the file the user gives
 * @Return
 *     return 0 if the function didn't detect any errors. Otherwise, return 1.
 */
int preprocessor(FILE* fptr, char* fname)
{
    FILE* temp; /* temp file */
    FILE* fname_am; /* the ".am" file */
    mcTable macroTable; /* macro table */

    /* 1. checking for macros errors */
    if(check_macro_errors(fptr))
    {
        return 1;
    }

    /* creating new files to paste every macro */
    fname_am = fopen(fname, "w+");
    temp = tmpfile();
    if (temp == NULL)
    {
        fprintf(stdout, "Failed to create temporary file at 'pre.c'.\n");
        return 1;
    }
    if (fname_am == NULL)
    {
        fprintf(stdout, "Failed to open the '.am' file at 'pre.c' file.\n");
        return 1;
    }

    /* 2. allocating memory for the macro table */
    allocateMacroTable(fptr, &macroTable);

    /* 3. getting the macros from the file and saving them */
    getMacros(fptr, &macroTable);

    /* 4. copying the text without the defining macros to the temp file */
    copyWithoutDefMc(temp, fptr);

    /* 5. pasting the text and every body of the macro instead of the macro occurrence from the temp file to the output */
    PastingMacros(temp, fname_am, &macroTable);

    /* 6. closing files, removing the temp file and deallocate the macro table */
    fclose(temp);
    fclose(fname_am);
    freeMacroTable(&macroTable);

    return 0;
}




