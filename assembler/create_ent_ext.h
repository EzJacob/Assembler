#ifndef PROJ1_CREATE_ENT_EXT_H
#define PROJ1_CREATE_ENT_EXT_H

#include "linked_list.h"

int makeEntryFile(char* fname, Node** labelsList, Node** entLabels, Node** extLabels);

void makeExternFile(char* fname, Node** entLabels, Node** extLabels);

#endif
