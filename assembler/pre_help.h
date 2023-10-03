

#ifndef PROJ1_PRE_HELP_H
#define PROJ1_PRE_HELP_H

#include <stdio.h>

/* This typedef is defining the macro table that will store 3 variables:
 *     mcCount - The amount of defining macros in the file
 *     mcName - The name of the defining macro
 *     mcText - The Text of the defining macro
 */
typedef struct mcTable{
    int* mcCount;
    char** mcName;
    char** mcText;
} mcTable;



/* checking for macro errors. If no errors were found the function returns 0. Otherwise, returns 1 */
int check_macro_errors(FILE* fptr);

/* copying all the text without the defining macro form the source file to the destination file */
void copyWithoutDefMc(FILE* dest, FILE* src);

/* getting the macros from a file and saving them into the variables macro_name and macro_text */
void getMacros(FILE* fptr, mcTable* macroTable);

/* copying the text from the temp file and pasting the macros into the fname_am file */
void PastingMacros(FILE* temp, FILE* fname_am, mcTable* macroTable);

/* checking if a word is a macro name. If the word is macro the function returns the index of where it is in the macro_name. Otherwise, returns -1 */
int isWordMacro(char* word, char** macro_name, int mc_count);

/* freeing the memory from a macro table */
void freeMacroTable(mcTable* macroTable);

/* allocating memory for a macro table */
void allocateMacroTable(FILE* fptr, mcTable* macroTable);




#endif
