#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "InputOutputFuncs.h"

int GetFirstNotSpaceChar(FILE* inStream)
{
    assert(inStream);

    int ch = 0;

    while (true)
    {
        ch = getc(inStream);

        if (ch == EOF || !isspace(ch))
            break;
    }

    return ch;
}

void MyFgets(char* string, const size_t maxStringLen, FILE* inStream)
{
    assert(string);
    assert(inStream);

    fgets(string, maxStringLen, inStream);

    if (string[0] == '\n')
        fgets(string, maxStringLen, inStream);
    
    char* newLinePos = strchr(string, '\n');
    *newLinePos = '\0';
}

const char* SkipSymbolsUntilStopChar(const char* string, const char stopChar)
{
    assert(string);
    
    const char* stringPtr = string;
    while (*stringPtr != stopChar && *stringPtr != '\0') 
        ++stringPtr;
    
    if (*stringPtr == '\0')
        return nullptr;

    return stringPtr;
}

const char* SkipSymbolsWhileStatement(const char* string, int (*statementFunc)(int))
{
    assert(string);
    assert(statementFunc);

    const char* stringPtr = string;

    while (statementFunc(*stringPtr) && *stringPtr != '\0')
        ++stringPtr;
    
    if (*stringPtr == '\0')
        return nullptr;

    return stringPtr;
}

void SkipSymbolsWhileStatement(FILE* inStream, int (*statementFunc)(int))
{
    assert(inStream);
    assert(statementFunc);

    int readenChar = EOF;
    do
    {
        readenChar = getc(inStream);
    } while (statementFunc(readenChar) && readenChar != EOF);
    
    if (readenChar != EOF)
        ungetc(readenChar, inStream);
}

void SkipSymbolsUntilStopChar(FILE* inStream, const char stopChar)
{
    assert(inStream);

    int readenChar = EOF;
    do
    {
        readenChar = getc(inStream);
    } while (readenChar != stopChar && readenChar != EOF);
    
    if (readenChar != EOF)
        ungetc(readenChar, inStream);    
}

