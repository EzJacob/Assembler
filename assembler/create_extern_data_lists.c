/*
 * This file is for making the extern and data lists
 */
#include "create_extern_data_lists.h"
#include "general_functions.h"
#include <stdlib.h>
#include <string.h>

/* This function adds symbols to the external labels list, data list and to the labels list according to the command
 * and to the type of symbol (if it is extern or not). This function also updating the IC and DC counters from a line
 * of code.
 *
 * This function does multiple tasks:
 * 1. Checking if a symbol is external. If so, adding the symbol name and its decimal address to the external list
 *    according to the current IC.
 * 2. Checking if there is a command of ".data" or ".string" and if there is a label that stores the data, that label
 *    will be added to the data list with the current DC.
 * 3. Updating the IC and DC counter according to the arguments given.
 *
 * @Parameters
 *     word - the command in the line of code
 *     line - the rest of the line after the command
 *     labelCpy - the name of the label at the start of a line of code
 *     labelFlag - set to 1 if there is a label. Otherwise, to 0.
 *     IC - the instruction counter
 *     DC - the data counter
 *     labelsList - the labels list
 *     extList - the extern labels list
 *     entList - the entry labels list
 */
void updateExtAndDataLists(char* word, char* line, char* labelCpy, int labelFlag, int* IC, int* DC, Node** labelsList, Node** extList, Node** dataList)
{
    int i = 0;
    int registerFlag = 0;
    char* token;
    Node* labels = *labelsList;
    Node* externLabels = *extList;
    Node* dataLabels = *dataList;

    /* checking if the command of the line is ".data" or ".string" */
    if(strcmp(word, ".data") == 0 || strcmp(word, ".string") == 0)
    {
        /* the case there is a defined label in the start of the line */
        if(labelFlag)
        {
            /* adding to dataLabels the labels that store data */
            addToList(&dataLabels, labelCpy, *DC);
        }

            /* updating the DC counter */

        /* the case there is a string for .string */
        if(line[0] =='\"')
            (*DC) += strlen(line) - 1;
        /* the case there are parameters for .data */
        else
        {
            token = strtok(line, ",");
            while(token != NULL)
            {
                (*DC)++;
                token = strtok(NULL, ",");
            }
        }

    }
    /* the case of a command with 2 operands */
    else if((isCommand(word) <= 3 || isCommand(word) == 6) && isCommand(word) != -1)
    {
        (*IC)++;

        /* saving to token the operand */
        token = strtok(line, ",");
        while(token != NULL)
        {
            /* saving an operand to "word" and trimming it */
            strcpy(word,token);
            trim(word);

            /* the case the operand is a register */
            if(isRegister(word))
                registerFlag++;
                /* the case if the operand is an extern label. If so, adding the label to the extern labels list */
            else if(getAdd(labels, word) == -1)
                addToList(&externLabels, word, *IC);

            (*IC)++;
            token = strtok(NULL, ",");
        }

        /* the case two operands in a line are registers */
        if(registerFlag == 2)
            (*IC)--;
    }
    /* the case of a command with one operand that can have parameters */
    else if((isCommand(word) == 9 || isCommand(word) == 10 || isCommand(word) == 13) && isCommand(word) != -1)
    {
        (*IC)++;
        i = 0;

        /* if there are parenthesis in the operand getting the index of the start of it.
         * If there is no parenthesis in the operand the index will be set to the length of the operand*/
        while(line[i] != '(' && line[i] != '\0')
            i++;

        /* the case the operand has start of parenthesis */
        if(line[i] == '(')
        {
            /* copying to word the symbol that comes before the parenthesis */
            strncpy(word, line, i);
            word[i] = '\0';

            /* checking if the symbol in word is an extern label. If so, adding it to the extern labels list */
            if(getAdd(labels, word) == -1)
                addToList(&externLabels, word, *IC);

            (*IC)++;

            /* removing the parenthesis from the operand and saving only the parameters */
            line[strlen(line) - 1] = '\0';
            setTextAfterIndex(line, i + 1);

            /* token saves each parameter */
            token = strtok(line, ",");
            while (token != NULL)
            {
                /* saving a parameter to "word" and trimming it */
                strcpy(word, token);
                trim(word);

                /* the case a parameter is a register */
                if (isRegister(word))
                    registerFlag++;
                    /* checking if a parameter is an extern label. If so, adding it to the extern labels list */
                else if (getAdd(labels, word) == -1)
                    addToList(&externLabels, word, *IC);

                (*IC)++;
                token = strtok(NULL, ",");
            }

            /* the case two operands in a line are registers */
            if (registerFlag == 2)
                (*IC)--;
        }
        /* the case there are no parenthesis in the operand - there is only a symbol in the operand */
        else
        {
            /* checking if the operand is and extern label. If so, adding it to the extern labels */
            if (getAdd(labels, line) == -1)
                addToList(&externLabels, line, *IC);

            (*IC)++;
        }

    }

    /* the case the command takes one operand with no parameters */
    else if(isCommand(word) != 14 && isCommand(word) != 15 && isCommand(word) != -1)
    {
        (*IC)++;

        /* checking if the operand is and extern label. If so, adding it to the extern labels list */
        if(getAdd(labels, line) == -1)
            addToList(&externLabels, line, *IC);

        (*IC)++;
    }
    /* the case of a command with no operands */
    else if(isCommand(word) != -1)
    {
        (*IC)++;
    }

    /* saving the lists */
    *labelsList = labels;
    *extList = externLabels;
    *dataList = dataLabels;
}

/* This function adding the IC counter to the DC counter for the labels that store data.
 *
 * This function works by going each label in the label list and checking if they are also in the dataLabels list.
 * If so, the function changes the decimal address for those labels to be the sum of their DC counter and the last IC.
 *
 * @Parameters
 *     labelsList - the labels list
 *     dataLabelsList - the data labels list
 *     IC - the instruction counter
 */
void changeDecAddForData(Node** labelsList, Node** dataLabelsList, int* IC)
{
    Node* labels = *labelsList;
    Node* dataLabels = *dataLabelsList;
    Node* tmp = labels;

    /* going over each label in the labels list */
    while(tmp != NULL)
    {
        /* the case the label is also in the dataLabels */
        if(searchInList(dataLabels, getLabel(tmp)))
        {
            /* changing the label's address to be its DC + the last IC */
            setAdd(&tmp, getAdd(dataLabels, getLabel(tmp)) + *IC);
        }
        tmp = getNext(tmp);
    }
}