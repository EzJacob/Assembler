#ifndef PROJ1_CREATE_EXTERN_DATA_LISTS_H
#define PROJ1_CREATE_EXTERN_DATA_LISTS_H

#include "linked_list.h"

void updateExtAndDataLists(char* word, char* line, char* labelCpy, int labelFlag, int* IC, int* DC, Node** labelsList, Node** extList, Node** dataList);


void changeDecAddForData(Node** labelsList, Node** dataLabelsList, int* IC);



#endif
