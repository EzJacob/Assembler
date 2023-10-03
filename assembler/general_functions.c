/*
 * This file includes all the general functions that are being used on the most of the other files.
 */
#include <string.h>
#include <ctype.h>

/* This function sets the string to start from a given index from the string
 *
 * This function works by changing each character in the string to start from the given index and accordingly moving
 * the index one position forward and changing the next character until the null character.
 *
 * @parameters
 *     str - the string
 *     i - the index
 */
void setTextAfterIndex(char* str, int i)
{
    int j = 0;
    while(str[i] != '\0')
    {
        str[j] = str[i];
        i++;
        j++;
    }
    str[j] = '\0';
}

/* This function is trimming a string from consecutive white space characters from the start and from the end of the string
 *
 * This function achieves its goal by getting the first and last index where a non-white character is encountered from the string
 * and modifying the string accordingly.
 *
 * @Parameters
 *     str - the string
 */
void trim(char* str)
{
    int start = 0;
    int end = strlen(str) - 1;

    /* the case the length of the string is 0 */
    if(end == -1)
        return;

    /* setting the starting index to be the first character that is not space or tab */
    while(str[start] == ' ' || str[start] == '\t')
        start++;

    /* the case every character is space or tab */
    if(start == end + 1)
    {
        str[0] = '\0';
        return;
    }

    /* setting the end to be the last character that is not space or tab*/
    while(str[end] == ' ' || str[end] == '\t')
        end--;

    str[end + 1] = '\0';
    setTextAfterIndex(str, start);
}

/* This function gets the first word from a text.
 *
 * This function achieves its goal by saving each character from the text until space, tab, newline,
 * end of file or null character is encountered.
 *
 * @Parameters
 *     word - the first word from the text
 *     text - the text
 * @Return
 *     if no word was saved the function will return 0.
 *     Otherwise, the function will return 1.
 */
int getWord(char* word, char* text)
{
    int i = 0;
    char ch = text[i];

    while(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\0')
    {
        word[i++] = ch;
        ch = text[i];
    }
    word[i] = '\0';
    if(word[0] == '\0')
        return 0;
    return 1;
}

/* This function checks if a word is a command.
 *
 * This function works by comparing the word to a command string.
 *
 * @Parameters
 *     word - a word from a line of code
 *
 * @Return
 *     The function returns an integer from 0 to 15 if the word is a command. Otherwise, returns -1.
 */
int isCommand(char* word)
{
    if(strcmp(word, "mov") == 0)
        return 0;
    if(strcmp(word, "cmp") == 0)
        return 1;
    if(strcmp(word, "add") == 0)
        return 2;
    if(strcmp(word, "sub") == 0)
        return 3;
    if(strcmp(word, "not") == 0)
        return 4;
    if(strcmp(word, "clr") == 0)
        return 5;
    if(strcmp(word, "lea") == 0)
        return 6;
    if(strcmp(word, "inc") == 0)
        return 7;
    if(strcmp(word, "dec") == 0)
        return 8;
    if(strcmp(word, "jmp") == 0)
        return 9;
    if(strcmp(word, "bne") == 0)
        return 10;
    if(strcmp(word, "red") == 0)
        return 11;
    if(strcmp(word, "prn") == 0)
        return 12;
    if(strcmp(word, "jsr") == 0)
        return 13;
    if(strcmp(word, "rts") == 0)
        return 14;
    if(strcmp(word, "stop") == 0)
        return 15;

    return -1;
}

/* This function checks if a word is a register.
 *
 * This function works by comparing if a word matches a register name.
 *
 * @Parameter
 *     word - a word from the line of code
 *
 * @Return
 *     If the word is a register the function returns 1 or 2. Otherwise the function returns 0.
 */
int isRegister(char* word)
{
    if(word[0] == 'r' && isdigit(word[1]) && word[1] != '8' && word[1] != '9' && word[2] == '\0')
        return 1;
    if(strcmp(word, "PSW") == 0)
        return 2;

    return 0;
}

/* This function checks if a word is a label.
 *
 * This function works by checking for if the character ':' is in the first word of a line of code.
 * For example the function gets: "HELLO:.entry" because there are no space between the label and the command
 * the function will put in the variable word only the label and the line will change to this: "HELLO .entry".
 *
 * @Parameters
 *     word - the first word in the line of code
 *     line - the line of code
 * @Return
 *     if the word is a label the function returns 1. Otherwise, the function returns 0.
 */
int isLabel(char* word, char* line)
{
    int i;

    if(word[strlen(word) - 1] == ':')
        return 1;

    for(i = 0; i < strlen(word); i++)
    {
        if(word[i] == ':')
        {
            strcpy(word, line);
            line[i + 1] = ' ';
            strcpy(line + i + 2, word + i + 1);
            getWord(word,line);

            return 1;
        }
    }
    return 0;
}





