#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "Akinator.h"
#include "../Tree/Tree.h"
#include "../Common/InputOutputFuncs.h"

static const char* const AkinatorDataFile = "AkinatorTree.txt";

//из-за этого не может в одной программе существовать два акинатора, но я честно не понимаю а нафига может понадобиться два акинатора с разными деревьями в одной проге
static TreeType tree = {.root = nullptr, .size = 0};

static void AkinatorGuessMode (TreeNodeType* node);
static void AkinatorNewInfoAdd(TreeNodeType* node);
static bool CheckAnswer(const char* answer);

static void AkinatorPrintDescription(const char* const word, StackType* path, 
                                     const TreeNodeType* beginNode, const int beginPos);

static void AkinatorSayString(const char* format, ...);

#define AKINATOR_PRINT_STRING(...)     \
do                                     \
{                                      \
    printf(__VA_ARGS__);               \
    AkinatorSayString(__VA_ARGS__);    \
} while (0)

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
    AKINATOR_PRINT_STRING("Programm is going to ask you questions. You should answer yes or no.\n");

    AkinatorGuessMode(tree.root);
}

static void AkinatorGuessMode(TreeNodeType* node)
{
    AKINATOR_PRINT_STRING("%s?\n", node->value);

    static const size_t maxStringLength  =  5;
    static char  answer[maxStringLength] = "";

    if (node->left == nullptr || node->right == nullptr) //вообще достаточно проверять одно, если нет бага в построении
    {
        MyFgets(answer, maxStringLength, stdin);
        if (strcasecmp(answer, "yes") == 0)
            AKINATOR_PRINT_STRING("SOSAMBA\n");
        else
            AkinatorNewInfoAdd(node);
        
        return;
    }
    
    while (true)
    {
        MyFgets(answer, maxStringLength, stdin);
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

        AKINATOR_PRINT_STRING("Invalid answer. Try again:\n");
    }
}

static void AkinatorNewInfoAdd(TreeNodeType* node)
{
    assert(node);
    assert(node->left  == nullptr);
    assert(node->right == nullptr);

    static const size_t maxAnswerLength  = 128;
    static char  answer[maxAnswerLength] =  "";

    AKINATOR_PRINT_STRING("Who is this?\n");
    MyFgets(answer, maxAnswerLength, stdin);

    TreeNodeType* left  = nullptr;
    TreeNodeCtor(&left, answer);

    TreeNodeType* right = nullptr;
    TreeNodeCtor(&right, node->value);

    while (true)
    {
        AKINATOR_PRINT_STRING("How differs from %s?\n"
               "In your answer you shouldn't use doesn't / don't\n", node->value);
        MyFgets(answer, maxAnswerLength, stdin);

        if (CheckAnswer(answer))
            break;
        
        AKINATOR_PRINT_STRING("Invalid answer. Try again:\n");
    }

    TreeNodeSetValue(node, answer);
    TreeLeafSetEdges(node, left, right);

    AKINATOR_PRINT_STRING("I'll win next time\n");
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

void AkinatorGiveDefinition(const char* const word)
{
    assert(word);

    StackType path;
    StackCtor(&path);

    bool found = TreeGetPath(&tree, word, &path);

    if (!found)
    {
        AKINATOR_PRINT_STRING("Can't give a definition, unknown word.\n");
        return;
    }

    AKINATOR_PRINT_STRING("%s is: ", word);
    AkinatorPrintDescription(word, &path, tree.root, (int)path.size - 1);

    StackDtor(&path);
}

void AkinatorCompareWords  (const char* const word1, const char* const word2)
{
    assert(word1);
    assert(word2);

    StackType path1, path2;
    StackCtor(&path1);
    StackCtor(&path2);

    bool found  = TreeGetPath(&tree, word1, &path1);
         found |= TreeGetPath(&tree, word2, &path2);

    if (!found)
    {
        AKINATOR_PRINT_STRING("Some words can't be found in my tree of wisdom. Can't compare.\n");
        return;
    }

    int edgeIdPath1 = (int)path1.size - 1;
    int edgeIdPath2 = (int)path2.size - 1;

    if (path1.data[edgeIdPath1] != path2.data[edgeIdPath2])
        AKINATOR_PRINT_STRING("These words have nothing in common.\n");
    else
        AKINATOR_PRINT_STRING("%s and %s are: ", word1, word2);

    TreeNodeType* node = tree.root;
    while (path1.data[edgeIdPath1] == path2.data[edgeIdPath2])
    {
        if (path1.data[edgeIdPath1] == 0)
            AKINATOR_PRINT_STRING("not ");
        
        AKINATOR_PRINT_STRING("%s, ", node->value);

        if (path1.data[edgeIdPath1] == 0)
            node = node->right;
        else
            node = node->left;

        --edgeIdPath1;
        --edgeIdPath2;
    }

    printf("\n");
    AKINATOR_PRINT_STRING("But %s is: ", word1); 
    AkinatorPrintDescription(word1, &path1, node, edgeIdPath1);

    AKINATOR_PRINT_STRING("And %s is: ", word2);
    AkinatorPrintDescription(word2, &path2, node, edgeIdPath2);

    StackDtor(&path1);
    StackDtor(&path2);
}

static void AkinatorPrintDescription(const char* const word, StackType* path, 
                                     const TreeNodeType* beginNode, const int beginPos)
{
    assert(word);
    assert(path);
    assert(beginPos != -1);

    const TreeNodeType* node = beginNode;
    for (int i = beginPos; i > -1; --i)
    {
        if (path->data[i] == 0)
            AKINATOR_PRINT_STRING("not ");

        AKINATOR_PRINT_STRING("%s, ", node->value);

        if (path->data[i] == 0)
            node = node->right;
        else
            node = node->left;
    }

    printf("\n");
}

static void AkinatorSayString(const char* format, ...)
{
    va_list argList;

    va_start(argList, format);

    static const size_t  maxStringLength  =   512;
    static char  command[maxStringLength] =  "say ";
    static const size_t sayShift = 4;

    vsnprintf(command + sayShift, maxStringLength, format, argList);
    system(command);

    va_end(argList);
}
