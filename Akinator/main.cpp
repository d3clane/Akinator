#include "Akinator.h"
#include "../Common/Log.h"
#include "../Common/InputOutputFuncs.h"

static AkinatorErrors AkinatorDefineReadenWord();
static AkinatorErrors AkinatorCompareReadenWords();

int main(const int argc, const char* argv[])
{
    setbuf(stdout, nullptr);

    LogOpen(argv[0]);

    AkinatorLoad();

    AkinatorErrors err = AkinatorErrors::NO_ERR;
    while (true)
    {
        AkinatorPrintMenu();
        
        int mode = GetFirstNotSpaceChar();

        bool quitCycle = false;

        switch(mode)
        {
            case 's':
                AkinatorShowTree();
                break;
            case 'g':
                err = AkinatorGuessMode();
                break;
            case 'd':
                err = AkinatorDefineReadenWord();
                break;
            case 'c':
                err = AkinatorCompareReadenWords();
                break;
            case 'q':
                quitCycle = true;
                break;
            case 'l':
                quitCycle = true;
                err = AkinatorWriteData();
                break;
            
            default:
                break;
        }

        if (err != AkinatorErrors::NO_ERR && err != AkinatorErrors::NO_SUCH_WORD_IN_TREE)
            break;
        
        if (quitCycle)
            break;
    }

    AkinatorDtor();
    
    return (int)err;
}

static AkinatorErrors AkinatorDefineReadenWord()
{
    static const size_t maxWordLength  = 256;
    static char    word[maxWordLength] =  "";

    printf("Enter the word: ");
    scanf("%s", word);

    return AkinatorGiveDefinition(word);
}

static AkinatorErrors AkinatorCompareReadenWords()
{
    static const size_t maxWordLength  = 256;
    static char   word1[maxWordLength] =  "";
    static char   word2[maxWordLength] =  "";

    printf("Enter first word: ");
    scanf("%s", word1);

    printf("Enter second word: ");
    scanf("%s", word2);

    return AkinatorCompareWords(word1, word2);
}
