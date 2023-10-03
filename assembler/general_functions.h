#ifndef PROJ1_GENERAL_FUNCTIONS_H
#define PROJ1_GENERAL_FUNCTIONS_H

/* the max line length is 80 so in the index 81 there could be '\n' and in index 82 there could be the '\0' */
#define MAX_LINE_LEN 82

/* label is limited to 30 characters so in the index 31 there could be ':' and in index 32 there could be '\0'
 * also a number that has more than 30 characters would always be beyond its limit so any word in a line has
 * max 30 characters.
 * There could be a word that contains for example: MAIN01234567890123456789012345:.string so sometimes there would be
 * an array of word that contains more than 30 characters.
*/
#define MAX_WORD_LEN 32

/* the binary machine code will start from the decimal address of 100 */
#define START_ADDRESS 100

void setTextAfterIndex(char* text, int i);

/* trimming a string from consecutive white space characters from the start and from the end of the string */
void trim(char* str);

/* getting a word from a text and saving it into the 'word' variable. Returns 1 if a word is saved. Otherwise, returns 0 */
int getWord(char* word, char* text);

/* checking if a word in line is a command. if a word is command return the amount of operands the command needs else return -1. */
int isCommand(char* word);

/* checking if a word is a register, if so the function returns 1 or 2. Otherwise, the function returns 0. */
int isRegister(char* word);

/* checking if a word is a label, returns 1 if so. Otherwise, returns 0. */
int isLabel(char* word, char* line);



#endif
