#ifndef INPUT_OUTPUT_FUNCS_H
#define INPUT_OUTPUT_FUNCS_H

#include "stdio.h"

int GetFirstNotSpaceChar(FILE* inStream = stdin);
void MyFgets(char* string, const size_t maxStringLen, FILE* inStream = stdin);

#endif