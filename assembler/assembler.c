/*
    This file is the main
*/
#include "pre.h"
#include "first_pass.h"
#include "second_pass.h"
#include <stdlib.h>
#include <string.h>
#define FREE_CLOSE_CONTINUE \
                free(fname); \
                fclose(fptr); \
                continue;

int main(int argc, char* argv[])
{
    FILE* fptr; /* pointer to a file */
    char* fname; /* file name */
    Node* labels = NULL; /* list of labels */
    int i;
    
    /* the case no file names were given */
    if(argc < 2)
    {
        fprintf(stderr, "Error! File name was not given.\n");
        return 1;
    }
    /* running the program for every file name that was given */
    for(i = 1; i < argc; i++)
    {
        fprintf(stdout, "\nRunning the file: %s\n", argv[i]);

        /* allocating memory and attaching the file extension ".as" */
        fname = (char*)malloc(sizeof(char) * (strlen(argv[i]) + 5));
        strcpy(fname, argv[i]);
        strcat(fname,".as");
        fptr = fopen(fname, "r");

        /* the case the file was not found */
        if (fptr == NULL)
        {
            fprintf(stdout, "Error! The given file %s failed to open.\n", argv[i]);
            free(fname);
            continue;
        }

        /* getting the file name and putting ".am" extension in the name */
        strcpy(fname, argv[i]);
        strcat(fname,".am");

        /* ----- preprocessor ----- */
        if (preprocessor(fptr, fname))
        {
            fprintf(stdout, "All of the errors line are referenced to the '.as' file\n");
            remove(fname);
            FREE_CLOSE_CONTINUE
        }

        /* closing the ".as" file and opening the file ".am" */
        fclose(fptr);
        fptr = fopen(fname, "r+");

        /* copying to fname the original name without extension */
        strcpy(fname, argv[i]);

        /* ----- first pass ----- */
        if (firstPass(fptr, &labels, fname))
        {
            FREE_CLOSE_CONTINUE
        }

        /* copying to fname the original file name and adding the extension ".ob" */
        strcpy(fname, argv[i]);
        strcat(fname,".ob");

        /* ----- second pass ----- */
        if (secondPass(fptr, &labels, fname))
        {
            remove(fname);
            FREE_CLOSE_CONTINUE
        }

        fprintf(stdout, "The program for this file was successfully finished.\n");
        FREE_CLOSE_CONTINUE
    }

    fprintf(stdout, "\nExiting program...\n");

    return 0;
}
