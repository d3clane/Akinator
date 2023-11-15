#include <assert.h>
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

    char* newLinePos = strchr(string, '\n');
    *newLinePos = '\0';
}
