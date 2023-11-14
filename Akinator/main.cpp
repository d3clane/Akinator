#include "Akinator.h"
#include "../Common/Log.h"

static void AkinatorDefineReadenWord();
static void AkinatorCompareReadenWords();

int main(const int argc, const char* argv[])
{
    setbuf(stdout, nullptr);
    LogOpen(argv[0]);

    AkinatorLoad();

    while (true)
    {
        printf("Choose mode: \n");

        printf("1) s - show the wise tree of knowledge\n");
        printf("2) g - guessing mode\n");
        printf("3) d - giving the definition of the word\n");
        printf("4) с - comparing definitions of two word\n");
        printf("5) q - quit without saving new info\n");
        printf("6) l - quit with saving new info\n");
        
        int mode = getchar();

        bool quitCycle = false;
        switch(mode)
        {
            case 's':
                AkinatorShowTree();
                break;
            case 'g':
                AkinatorGuessMode();
                break;
            case 'd':
                AkinatorDefineReadenWord();
                break;
            case 'c':
                AkinatorCompareReadenWords();
                break;
            case 'q':
                quitCycle = true;
                break;
            case 'l':
                quitCycle = true;
                AkinatorWriteData();
                break;
            
            default:
                break;
        }
        
        if (quitCycle)
            break;
    }
}

static void AkinatorDefineReadenWord()
{
    static const size_t maxWordLength  = 256;
    static char    word[maxWordLength] =  "";

    printf("Enter the word: ");
    scanf("%s", word);

    AkinatorGiveDefinition(word);

    //TODO: замена на функцию
    while (getchar() != '\n');
}

static void AkinatorCompareReadenWords()
{
    static const size_t maxWordLength  = 256;
    static char   word1[maxWordLength] =  "";
    static char   word2[maxWordLength] =  "";

    printf("Enter first word: ");
    scanf("%s", word1);

    printf("Enter second word: ");
    scanf("%s", word2);

    AkinatorCompareWords(word1, word2);

    //TODO: замена на функцию
    while (getchar() != '\n'); 
}