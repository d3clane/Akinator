#ifndef INPUT_OUTPUT_FUNCS_H
#define INPUT_OUTPUT_FUNCS_H

#include "stdio.h"

int GetFirstNotSpaceChar(FILE* inStream = stdin);

//TODO: перенос в другой файл
const char* SkipSymbolsUntilStopChar (const char* string, const char stopChar);
const char* SkipSymbolsWhileStatement(const char* string, int (*statementFunc)(int));

void MyFgets(char* string, const size_t maxStringLen, FILE* inStream = stdin);

#endif