/*
 * This file contains all the functions for linked list
 */
#include "linked_list.h"
#include "general_functions.h"
#include <stdlib.h>
#include <string.h>

/* This function is adding nodes to a list
 *
 * This function works by constructing a new node with the given parameters and adding the new node to the start of the list.
 *
 * @Parameters
 *     head - a pointer to the start of the list
 *     data - the label name
 *     decAddress - the decimal address of a label
 */
void addToList(Node** head, char* data, int decAddress)
{
    /* Initializing new node and allocating memory for it and for its label name */
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = (char*)malloc(sizeof(char) * MAX_LINE_LEN);

    /* copying to data the label name and setting the decimal address value */
    strcpy(newNode->data, data);
    newNode->decAddress = decAddress;

    /* pushing the new node to the start of the list and setting it to be the head of the list */
    newNode->next = *head;
    *head = newNode;
}

/* This function is searching for a label name in a list
 *
 * This function works by going through every node in the list and checking if the node's label name is the same at the given label name.
 *
 * @Parameters
 *     head - the head of a list
 *     data - the label name
 *
 * @Return
 *     If the label is found in the list the function returns 1. Otherwise, the function returns 0.
 */
int searchInList(Node* head ,char* data)
{
    Node* tmp = head;

    /* going through every node in the list */
    while(tmp != NULL)
    {
        /* checking if the given name is matched with the node's label name */
        if(strcmp(tmp->data, data) == 0)
            return 1;

        tmp = tmp->next;
    }
    return 0;
}

/* This function is freeing the memory allocated for a list.
 *
 * This function works by going through every node in the list and deallocating memory for the node's label name and then the node itself.
 *
 * @Parameters
 *     head - the head of the list
 */
void freeList(Node** head)
{
    Node* tmp = *head;
    Node* next;

    /* going through every node in a list */
    while (tmp != NULL)
    {
        /* saving the next node from tmp */
        next = tmp->next;

        /* freeing the label name and then the node */
        free(tmp->data);
        free(tmp);

        tmp = next;
    }
    *head = NULL;
}

/* This function is returning the decimal address of a label that is in a list
 *
 * This function works by going through every node in a list and searching for a given label name and returning it's address.
 * If the label name is found in the list the function will return it's decimal address.
 * If the label is extern the function will return -1 to indicate the extern type
 * If the function didn't find the label in the list the function returns -2
 *
 * @Parameters
 *     head - the head of the list
 *     data - the label name
 *
 * @Return
 * If the label name is found in the list the function will return it's decimal address.
 * If the label is extern the function will return -1 to indicate the extern type.
 * If the function didn't find the label in the list the function returns -2.
 */
int getAdd(Node* head, char* data)
{
    Node* tmp = head;

    /* going through every node in the list */
    while(tmp != NULL)
    {
        /* checking if the given name is matched with the node's label name */
        if(strcmp(tmp->data, data) == 0)
            return tmp->decAddress;

        tmp = tmp->next;
    }
    return -2;
}

/* This function sets the node's decimal address
 *
 * @Parameters
 *     node - a node in a list
 *     Add - the decimal address
 */
void setAdd(Node** node, int Add)
{
    (*node)->decAddress = Add;
}

/* This function gets the next node in a list
 *
 * @Parameters
 *     node - a node in a list
 *
 * @Return
 *     returning the next node
 */
Node* getNext(Node* node)
{
    return node->next;
}

/* This functions gets the label name from a node
 *
 * @Parameters
 *     node - a node in a list
 *
 * @Return
 *     returning the label's name of the node
 */
char* getLabel(Node* node)
{
    return node->data;
}

