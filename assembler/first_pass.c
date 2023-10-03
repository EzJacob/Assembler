/*
 * This file is the first pass of the assembler.
 */
#include "general_functions.h"
#include "command_errors.h"
#include "syntax_errors.h"
#include "create_extern_data_lists.h"
#include "create_ent_ext.h"
#include <stdio.h>
#include <string.h>

/* This function is getting all the defined labels and adding them with their decimal address to a labels list
 * and this function creates if needed the extern and entry files.
 * The function also checks for errors in each line of code, if an error is found the function will print
 * an explicit error message with a line number of where the error is.
 *
 * This function has multiple steps:
 * 1. Checking for syntax errors and saving the indications of symbols with type entry or extern.
 * 2. Adding the defined labels to a labels list, the uses of extern symbols to an extern labels list and data to the data list.
 * 3. Changing the decimal address for labels that represent data (they might not have a label name).
 * 4. Making the entry file and writes in it all the symbols that are of type entry and their decimal address.
 * 5. Making the extern file and writes in it all the times an external symbol was used and its decimal address.
 * 6. Saving the labels list for the second pass and creating the object file and writing its title - the IC and DC counters.
 */
int firstPass(FILE* fptr, Node** labelsList, char* fname)
{
    int IC = 0, DC = 0;
    int lineCount = 1;
    int ErrorFlag = 0;
    int labelFlag = 0;
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char lineCpy[MAX_LINE_LEN];
    char wordCpy[MAX_LINE_LEN];
    char labelCpy[MAX_WORD_LEN];
    Node* labels = NULL;
    Node* externLabels = NULL;
    Node* entryLabels = NULL;
    Node* dataLabels = NULL;
    FILE* object;

    /* 1. Checking for syntax errors. Saving the indications of symbols with type entry or extern. */

    /* getting each line of code*/
    while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
    {
        /* checking if the line is beyond it's 80 characters limit */
        if(strlen(line) > MAX_LINE_LEN - 2 && line[MAX_LINE_LEN - 1] != '\n')
        {
            fprintf(stdout, "Error in line %i: a line is limited to 80 characters.\n", lineCount);

            /* moving through the line until the new line character detected or until the end of the text */
            while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
                if(line[strlen(line) - 1] == '\n')
                    break;

            lineCount++;
            ErrorFlag = 1;
            continue;
        }

        /* trimming the line */
        trim(line);

        /* checking if the line only had white space characters or if the line is a comment */
        if(line[0] == '\n' || line[0] == ';')
        {
            lineCount++;
            continue;
        }

        /* removing the new line character from the end of the line */
        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        /* getting the first word from the line */
        getWord(word,line);

        /* checking if the word is a label */
        if(isLabel(word, line))
        {
            /* removing the ':' from the end of the word */
            word[strlen(word) - 1] = '\0';

            /* checking if the defining label is a register*/
            if(isRegister(word) || isCommand(word) >= 0)
            {
                ErrorFlag = 1;
                fprintf(stdout, "Error in line %i: label cannot be defined with a saved word.\n", lineCount);
                lineCount++;
                continue;
            }
            /* checking for symbol errors for the label */
            if(!isSymbol(word, lineCount))
            {
                ErrorFlag = 1;
                lineCount++;
                continue;
            }

            /* copying to line the line without the label then trimming the line and getting the second word */
            setTextAfterIndex(line, strlen(word) + 1);
            trim(line);
            getWord(word,line);

            labelFlag = 1;
        }

        /* copying to line the line without the command and trimming it */
        setTextAfterIndex(line, strlen(word));
        trim(line);

        /* copying the command and the rest of the line */
        strcpy(lineCpy, line);
        strcpy(wordCpy, word);

        /* checking for syntax errors in the line of code */
        if(checkSyntax(wordCpy, lineCpy, lineCount))
            ErrorFlag = 1;

        /* the case if the command is ".entry" */
        else if(strcmp(word, ".entry") == 0)
        {
            /* the case the line of code had a label */
            if(labelFlag)
                fprintf(stdout, "Warning in line %i: the label before .entry is useless\n", lineCount);


            /* the case the symbol after .entry is already with the type .entry */
            if(searchInList(entryLabels, line))
            {
                ErrorFlag = 1;
                fprintf(stdout, "Error in line %i: this label is already with type .entry\n", lineCount);
            }
                /* the case the symbol after .entry is not already with the type .entry */
            else if(line[0] != '\0')
            {
                /* adding to the entry's labels-list the symbol the comes after .entry
                 * the value of the decimal address for this list is irrelevant that's why the lineCount is put
                 * as decimal address. This list contains all the symbols that are with type .entry only. */
                addToList(&entryLabels, line, lineCount);
            }
        }
            /* the case if the command is ".extern" */
        else if(strcmp(word, ".extern") == 0)
        {
            /* the case the line of code had a label */
            if(labelFlag)
                fprintf(stdout, "Warning in line %i: the label before .extern is useless\n", lineCount);


            /* the case the symbol after .extern is already defined in the file */
            if(searchInList(labels, line))
            {
                ErrorFlag = 1;
                fprintf(stdout, "Error in line %i: this label is already defined in this file\n", lineCount);
            }
                /* the case the symbol after .extern is not already defined in the file */
            else if(line[0] != '\0')
            {
                /* adding to the labels list the symbol that comes after .extern and setting the
                 * decimal address of the symbol as -1 to indicate the symbol is external */
                addToList(&labels, line, -1);
            }
        }

        labelFlag =  0;
        lineCount++;
    }

    /* rewinding the ".am" file pointer and setting the line counter to 1 */
    rewind(fptr);
    lineCount = 1;

    /* ---------------------------------------end of step 1----------------------------------------------------- */

    /* 2. Saving all the labels and their decimal address to the labels list.
     *    Saving all extern symbol uses and their decimal address to the extern labels list.
     *    Saving all the first use of data and their DC value to the data list.
     *    Updating the IC and DC counters accordingly.
     */

    /* getting each line of code */
    while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
    {
        /* checking if the line is beyond it's 80 characters limit */
        if(strlen(line) > MAX_LINE_LEN - 2 && line[MAX_LINE_LEN - 1] != '\n')
        {
            /* moving through the line until the new line character detected or until the end of the text */
            while(fgets(line, MAX_LINE_LEN, fptr) != NULL)
                if(line[strlen(line) - 1] == '\n')
                    break;

            lineCount++;
            continue;
        }

        /* trimming the line */
        trim(line);

        /* checking if the line only had white space characters or if the line is a comment */
        if(line[0] == '\n' || line[0] == ';')
        {
            lineCount++;
            continue;
        }

        /* removing the new line character from the end of the line */
        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        /* getting the first word from the line */
        getWord(word, line);

        /* checking if the word is a label */
        if(isLabel(word, line))
        {
            /* removing the ':' from the end of the word */
            word[strlen(word) - 1] = '\0';

            /* the case the label is already in the labels list */
            if(searchInList(labels, word))
            {
                ErrorFlag = 1;
                fprintf(stdout, "Error in line %i: this label is already defined\n", lineCount);
            }
            /* the case the label is not already in the labels list */
            else
            {
                /* adding the label to the label list and setting its decimal address as IC */
                addToList(&labels, word, IC);

                /* copying the label to the "labelCpy" variable */
                strcpy(labelCpy, word);
            }

            /* copying to line - the line without the label. Then trimming the line and getting the second word */
            setTextAfterIndex(line, strlen(word) + 1);
            trim(line);
            getWord(word,line);

            labelFlag = 1;
        }

        /* copying to line - the line without the command and trimming it */
        setTextAfterIndex(line, strlen(word));
        trim(line);

        /* the case the command is .entry or .extern */
        if(strcmp(word, ".entry") == 0 || strcmp(word, ".extern") == 0)
        {
            lineCount++;
            continue;
        }

        /*  Adding symbols to the "labels", "externLabels" and "dataLabels" lists
         *  and also updating the IC and DC counters accordingly */
        updateExtAndDataLists(word, line, labelCpy, labelFlag, &IC, &DC, &labels,&externLabels, &dataLabels);

        labelFlag = 0;
        lineCount++;
    }

    /* ---------------------------------------end of step 2----------------------------------------------------- */


    /* 3. changing the decimal address for labels that represent data (they have integers or a string, and they might not have a label name) */
    changeDecAddForData(&labels, &dataLabels, &IC);

    /* 4. making the entry file and writes in it all the symbols that are of type entry and their decimal address */
    ErrorFlag += makeEntryFile(fname, &labels, &entryLabels, &externLabels);

    /* 5. making the extern file and writes in it all the times an external symbol was used and its decimal address */
    makeExternFile(fname, &entryLabels, &externLabels);

    /* freeing all the lists except from the labels list */
    freeList(&dataLabels);
    freeList(&entryLabels);
    freeList(&externLabels);

    /* rewinding the ".am" file pointer */
    rewind(fptr);

    /* the case there are errors */
    if(ErrorFlag)
    {
        freeList(&labels);
        return 1;
    }

    /* 6. Saving the labels list for the second pass. Creating the object file and writing its title - the IC and DC counters. */

    /* saving the labels list */
    *labelsList = labels;

    /* the case "fname" has the extension of ".ext" or ".ent" and if so removing the extension */
    if(strlen(fname) >= 4 && (strcmp(fname + strlen(fname) - 4,".ext") == 0 || strcmp(fname + strlen(fname) - 4,".ent") == 0))
        fname[strlen(fname) - 4] = '\0';

    /* adding the extension ".ob" */
    strcat(fname, ".ob");

    /* creating the object file */
    object = fopen(fname,"w+");
    if (object == NULL)
    {
        fprintf(stdout, "Failed to open the object file at 'first_pass.c'.\n");
        return 1;
    }

    /* writing to the object file the title of it - the IC and DC counters */
    sprintf(word, "\t%d\t%d\t\n", IC, DC);
    fputs(word, object);

    /* closing the object file */
    fclose(object);

    /* ---------------------------------------end of step 6----------------------------------------------------- */

    return 0;
}



