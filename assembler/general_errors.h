#ifndef PROJ1_GENERAL_ERRORS_H
#define PROJ1_GENERAL_ERRORS_H


/* checking for general errors in a line of code. If there is an error returns 1. Otherwise, returns 0. */
int checkGeneralErrors(char* command, char* line, int lineCount);

/* checking for a comma between the command and an operand, if so returns 1. Otherwise, returns 0. */
int checkStartComma(char* command, char* line, int lineCount);

/* checking for a comma at the end of the operand. Returns 1 if so, otherwise returns 0 */
int checkEndComma(char* line, int lineCount);

/* checking for consecutive commas in a line code. If that's the case returns 1, Otherwise returns 0 */
int checkConsComma(char* line, int lineCount);






#endif
