/*
 * This file is for making the ".ent" and ".ext" files
 */
#include "create_ent_ext.h"
#include "general_functions.h"
#include <stdio.h>
#include <string.h>

/* This function makes the entry file.
 *
 * This function works by going over each label that is in the entry labels list and searching them in the labels list.
 * If the label was found in the labels list, the function will write to the entry file the label's name and their
 * decimal address.
 *
 * @Parameters
 *     fname - the name of the file the user gave
 *     labelsList - the labels list
 *     entLabels - the entry labels list
 *     extLabels - the extern labels list
 *
 * @Return
 *     The function returns 0 if no errors were found. Otherwise, the function returns 1.
 */
int makeEntryFile(char* fname, Node** labelsList, Node** entLabels, Node** extLabels)
{
    int ErrorFlag = 0;
    char line[MAX_LINE_LEN];
    FILE* ent;
    Node* tmp;
    Node* labels = *labelsList;
    Node* entryLabels = *entLabels;
    Node* externLabels = *extLabels;

    /* checking if there are labels in the entry labels list */
    if(entryLabels == NULL)
        return 0;

    /* adding the extension ".ent" to the file name the user gave and opening the entry file */
    strcat(fname, ".ent");
    ent = fopen(fname,"w+");

    /* going over each label in the entry labels list */
    tmp = entryLabels;
    while(tmp != NULL)
    {
        /* the case the label in the entry labels list is not in the labels list */
        if(!searchInList(labels, getLabel(tmp)))
        {
            ErrorFlag = 1;
            fprintf(stdout, "Error in line %i: this label type is entry but the label is not defined.\n", getAdd(tmp, getLabel(tmp)));
        }
        /* the case a label in the entry label list is also in the extern labels list */
        else if(searchInList(externLabels, getLabel(tmp)))
        {
            ErrorFlag = 1;
            fprintf(stdout, "Error in line %i: a label's type cannot be entry and extern.\n", getAdd(tmp, getLabel(tmp)));
        }
        /* the case the label in the entry labels list is in the labels list and not in the extern labels list */
        else
        {
            /* saving the name of the label and their decimal address and writing it to the entry file */
            sprintf(line, "%s\t%d\n", getLabel(tmp) ,getAdd(labels, getLabel(tmp)) + START_ADDRESS);
            fputs(line, ent);
        }
        tmp = getNext(tmp);
    }


    /* the case nothing was written in the entry file */
    if(ftell(ent) == 0)
    {
        fclose(ent);
        remove(fname);
    }
    else
        fclose(ent);

    if(ErrorFlag)
        return 1;
    return 0;
}

/* This function makes the extern file.
 *
 * This function works by going over each label that is in the extern labels and writes to the extern file the
 * label's name and their decimal address.
 *
 * @Parameters
 *     fname - the name of the file the user gave with or without the extension ".ent" (depends on the makeEntryFile function)
 *     entLabels - the entry labels list
 *     extLabels - the extern labels list
 */
void makeExternFile(char* fname, Node** entLabels, Node** extLabels)
{
    char line[MAX_LINE_LEN];
    FILE* ext;
    Node* tmp;
    Node* entryLabels = *entLabels;
    Node* externLabels = *extLabels;

    /* the case there are no labels in the extern labels list */
    if(externLabels == NULL)
        return;

    /* the case fname has the extension of ".ent" */
    if(entryLabels != NULL)
        fname[strlen(fname) - 2] = 'x';
    /* the case fname is the file name the user gave */
    else
        strcat(fname,".ext");
    ext = fopen(fname,"w+");

    tmp = externLabels;
    /* going over each label in the extern labels list */
    while(tmp != NULL)
    {
        /* saving the name of the label and their decimal address and writing it to the extern file */
        sprintf(line, "%s\t%d\n", getLabel(tmp) ,getAdd(tmp, getLabel(tmp)) + START_ADDRESS);
        fputs(line, ext);

        tmp = getNext(tmp);
    }

    /* the case nothing was written in the extern file */
    if(ftell(ext) == 0)
    {
        fclose(ext);
        remove(fname);
    }
    else
        fclose(ext);
}

