#include "Akinator.h"
#include "../Common/Log.h"
#include "../Common/InputOutputFuncs.h"

static AkinatorErrors AkinatorDefineReadenWord(AkinatorType* akinator);
static AkinatorErrors AkinatorCompareReadenWords(AkinatorType* akinator);

int main(const int argc, const char* argv[])
{
    setbuf(stdout, nullptr);

    LogOpen(argv[0]);

    AkinatorType akinator = {};
    AkinatorLoad(&akinator);

    AkinatorErrors err = AkinatorErrors::NO_ERR;
    while (true)
    {
        AkinatorPrintMenu();
        
        int mode = GetFirstNotSpaceChar();

        bool quitCycle = false;

        switch(mode)
        {
            case 's':
                AkinatorShowTree(&akinator);
                break;
            case 'g':
                err = AkinatorGuessMode(&akinator);
                break;
            case 'd':
                err = AkinatorDefineReadenWord(&akinator);
                break;
            case 'c':
                err = AkinatorCompareReadenWords(&akinator);
                break;
            case 'q':
                quitCycle = true;
                break;
            case 'l':
                quitCycle = true;
                err = AkinatorWriteData(&akinator);
                break;
            
            default:
                break;
        }

        if (err != AkinatorErrors::NO_ERR && err != AkinatorErrors::NO_SUCH_WORD_IN_TREE)
            break;
        
        if (quitCycle)
            break;
    }

    AkinatorDtor(&akinator);

    return (int)err;
}

static AkinatorErrors AkinatorDefineReadenWord(AkinatorType* akinator)
{
    static const size_t maxWordLength  = 256;
    static char    word[maxWordLength] =  "";

    printf("Enter the word: ");
    scanf("%s", word);

    return AkinatorGiveDefinition(akinator, word);
}

static AkinatorErrors AkinatorCompareReadenWords(AkinatorType* akinator)
{
    static const size_t maxWordLength  = 256;
    static char   word1[maxWordLength] =  "";
    static char   word2[maxWordLength] =  "";

    printf("Enter first word: ");
    scanf("%s", word1);

    printf("Enter second word: ");
    scanf("%s", word2);

    return AkinatorCompareWords(akinator, word1, word2);
}
