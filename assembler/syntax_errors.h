#ifndef PROJ1_SYNTAX_ERRORS_H
#define PROJ1_SYNTAX_ERRORS_H


/* checking errors for a line in the file. returning 1 if there is an error. Otherwise, returning 0 */
int checkSyntax(char* command, char* line, int lineCount);

/* checking syntax errors for a line of code that has the command .data or .string. returns 1 if there is an error. Otherwise, returns 0 */
int checkSyntaxDataString(char* command, char* line, int lineCount);

/* checking syntax errors for a line of code that has the command .entry or .extern. returns 1 if there is an error. Otherwise, returns 0 */
int checkSyntaxEntryExtern(char* line, int lineCount);




#endif
