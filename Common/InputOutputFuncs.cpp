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
