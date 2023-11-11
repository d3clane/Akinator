#include "Akinator.h"
#include "Log.h"

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