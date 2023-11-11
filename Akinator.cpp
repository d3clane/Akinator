#include <string.h>
#include <assert.h>

#include "Akinator.h"
#include "Tree.h"

static const char* const AkinatorDataFile = "AkinatorTree.txt";

TreeType tree = {.root = nullptr, .size = 0};

static void AkinatorGuessMode (TreeNodeType* node);
static void AkinatorNewInfoAdd(TreeNodeType* node);
static bool CheckAnswer(const char* answer);

//TODO: перенести в другой файл
static void MyFgets(char* string, const size_t maxStringLen, FILE* inStream = stdin);

AkinatorErrors AkinatorLoad()
{
    TreeCtor(&tree);

    FILE* dataFile = fopen(AkinatorDataFile, "r");

    if (dataFile == nullptr)
        return AkinatorErrors::NO_DATA_BASE;

    TreeReadPrefixFormat(&tree, dataFile);

    TREE_DUMP(&tree);

    fclose(dataFile);

    return AkinatorErrors::NO_ERR;
}

void AkinatorGuessMode()
{
    printf("Programm is going to ask you questions. You should answer yes or no.\n");

    //TODO: обертка в другую функцию, проверка EOF
    while (getchar() != '\n');

    AkinatorGuessMode(tree.root);
}

static void AkinatorGuessMode(TreeNodeType* node)
{
    printf("%s?\n", node->value);

    static const size_t maxStringLength  =  5;
    static char  answer[maxStringLength] = "";

    if (node->left == nullptr || node->right == nullptr) //вообще достаточно проверять одно, если нет бага в построении
    {
        MyFgets(answer, maxStringLength, stdin);
        if (strcasecmp(answer, "yes") == 0)
            printf("SOSAMBA)))\n");
        else
            AkinatorNewInfoAdd(node);
        
        return;
    }
    
    while (true)
    {
        MyFgets(answer, maxStringLength, stdin);
        printf("I read: %s, len: %zu\n", answer, strlen(answer));
        if (strcasecmp(answer, "yes") == 0)
        {
            AkinatorGuessMode(node->left);
            break;
        }
        else if (strcasecmp(answer, "no") == 0)
        {
            AkinatorGuessMode(node->right);
            break;
        }

        printf("Invalid answer. Try again:\n");
    }
}

static void AkinatorNewInfoAdd(TreeNodeType* node)
{
    assert(node);
    assert(node->left  == nullptr);
    assert(node->right == nullptr);

    static const size_t maxAnswerLength  = 128;
    static char  answer[maxAnswerLength] =  "";

    printf("Who is this?\n");
    MyFgets(answer, maxAnswerLength, stdin);

    TreeNodeType* left  = nullptr;
    TreeNodeCtor(&left, answer);

    TreeNodeType* right = nullptr;
    TreeNodeCtor(&right, node->value);

    while (true)
    {
        printf("How differs from %s?\n"
               "In your answer you shouldn't use doesn't / don't\n", node->value);
        MyFgets(answer, maxAnswerLength, stdin);

        if (CheckAnswer(answer))
            break;
        
        printf("Invalid answer. Try again:\n");
    }

    TreeNodeSetValue(node, answer);
    TreeLeafSetEdges(node, left, right);

    printf("I'll win next time\n");
}

//TODO: TreeNodeType typedef-ом сделать TreeNodeType* так сильно логичнее (чем каждый раз делать указатель казалось бы без причин)

static bool CheckAnswer(const char* answer)
{
    assert(answer);

    return (strcasestr(answer, "doesn't") == nullptr) && (strcasestr(answer, "don't") == nullptr);
}

AkinatorErrors AkinatorWriteData()
{
    FILE* dataFile = fopen(AkinatorDataFile, "w");

    if (dataFile == nullptr)
        return AkinatorErrors::NO_DATA_BASE;
    
    TreePrintPrefixFormat(&tree, dataFile);

    return AkinatorErrors::NO_ERR;
}

void AkinatorShowTree()
{
    TreeGraphicDump(&tree, true);
}

static void MyFgets(char* string, const size_t maxStringLen, FILE* inStream)
{
    assert(string);
    assert(inStream);

    fgets(string, maxStringLen, inStream);

    char* newLinePos = strchr(string, '\n');
    *newLinePos = '\0';
}
