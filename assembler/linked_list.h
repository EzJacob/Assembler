#ifndef PROJ1_LINKED_LIST_H
#define PROJ1_LINKED_LIST_H

/* Constructing list data structure:
 *     data - the name of a label
 *     decAddress - the decimal address of a label
 *     next - the next node in the list
 */
typedef struct Node {
    char* data;
    int decAddress;
    struct Node* next;
} Node;

/* adding nodes to a list. */
void addToList(Node** head, char* data, int decAddress);

/* searching a node according to a label name. Returning 1 if the node is found. Otherwise, returning 0. */
int searchInList(Node* head ,char* data);

/* deallocating memory for a list */
void freeList(Node** head);

/* returns the decimal address of a given node according to its label name. If the label is extern returns -1. If the label not found returns -2. */
int getAdd(Node* head, char* data);

/* setting the decimal address of a node */
void setAdd(Node** node, int Add);

/* returning the next node */
Node* getNext(Node* node);

/* returning the label's name if a node */
char* getLabel(Node* node);


#endif


