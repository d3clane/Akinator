#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "Akinator.h"
#include "../Tree/Tree.h"
#include "../Common/InputOutputFuncs.h"

static const char* const AkinatorDataFile = "AkinatorTree.txt";

static TreeType tree = {.root = nullptr, .size = 0};

static void AkinatorGuessMode (TreeNodeType* node);
static bool CheckAnswer(const char* answer);

static AkinatorErrors AkinatorAddNewInfo(TreeNodeType* node);
static AkinatorErrors AkinatorPrintDescription(const char* const word, StackType* path, 
                                               const TreeNodeType* beginNode, const int beginPos);

static void AkinatorSayString(const char* format, ...);
static AkinatorErrors AkinatorVerify(TreeNodeType* node);

#define AKINATOR_PRINT_STRING(...)     \
do                                     \
{                                      \
    printf(__VA_ARGS__);               \
    AkinatorSayString(__VA_ARGS__);    \
} while (0)

#define AKINATOR_CHECK()                    \
do                                          \
{                                           \
    AkinatorErrors err = AkinatorVerify();  \
    if (err != AkinatorErrors::NO_ERR)      \
        return err;                         \
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

    AKINATOR_CHECK();

    return AkinatorErrors::NO_ERR;
}

AkinatorErrors AkinatorGuessMode()
{
    AKINATOR_CHECK();

    AKINATOR_PRINT_STRING("Programm is going to ask you questions. You should answer yes or no.\n");

    AkinatorGuessMode(tree.root);

    return AkinatorErrors::NO_ERR;
}

static void AkinatorGuessMode(TreeNodeType* node)
{
    AKINATOR_PRINT_STRING("%s?\n", node->value);

    static const size_t maxStringLength  =  5;
    static char  answer[maxStringLength] = "";

    if (node->left == nullptr || node->right == nullptr)
    {
        MyFgets(answer, maxStringLength, stdin);
        if (strcasecmp(answer, "yes") == 0)
            AKINATOR_PRINT_STRING("SOSAMBA\n");
        else
            AkinatorAddNewInfo(node);
        
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

static AkinatorErrors AkinatorAddNewInfo(TreeNodeType* node)
{
    assert(node);
    assert(node->left  == nullptr);
    assert(node->right == nullptr);

    AKINATOR_CHECK();

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
        AKINATOR_PRINT_STRING("How differs from %s?\n", node->value);
        AKINATOR_PRINT_STRING("In your answer you shouldn't use doesn't or don't\n");
        MyFgets(answer, maxAnswerLength, stdin);

        if (CheckAnswer(answer))
            break;
        
        AKINATOR_PRINT_STRING("Invalid answer. Try again:\n");
    }

    TreeNodeSetValue(node, answer);
    TreeLeafSetEdges(node, left, right);

    AKINATOR_PRINT_STRING("I'll win next time\n");

    return AkinatorErrors::NO_ERR;
}

static bool CheckAnswer(const char* answer)
{
    assert(answer);

    return (strcasestr(answer, "doesn't") == nullptr) && (strcasestr(answer, "don't") == nullptr);
}

AkinatorErrors AkinatorWriteData()
{
    AKINATOR_CHECK();
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

AkinatorErrors AkinatorGiveDefinition(const char* const word)
{
    assert(word);

    AKINATOR_CHECK();

    StackType path;
    StackCtor(&path);

    bool found = TreeGetPath(&tree, word, &path);

    if (!found)
    {
        AKINATOR_PRINT_STRING("Can't give a definition, unknown word.\n");
        return AkinatorErrors::NO_SUCH_WORD_IN_TREE;
    }

    AKINATOR_PRINT_STRING("%s is: ", word);
    AkinatorPrintDescription(word, &path, tree.root, (int)path.size - 1);

    StackDtor(&path);

    return AkinatorErrors::NO_ERR;
}

AkinatorErrors AkinatorCompareWords(const char* const word1, const char* const word2)
{
    assert(word1);
    assert(word2);

    AKINATOR_CHECK();

    StackType path1, path2;
    StackCtor(&path1);
    StackCtor(&path2);

    bool found  = TreeGetPath(&tree, word1, &path1);
         found |= TreeGetPath(&tree, word2, &path2);

    if (!found)
    {
        AKINATOR_PRINT_STRING("Some words can't be found in my tree of wisdom. Can't compare.\n");
        return AkinatorErrors::NO_SUCH_WORD_IN_TREE;
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

    return AkinatorErrors::NO_ERR;
}

static AkinatorErrors AkinatorPrintDescription(const char* const word, StackType* path, 
                                               const TreeNodeType* beginNode, const int beginPos)
{
    assert(word);
    assert(path);
    assert(beginPos != -1);

    AKINATOR_CHECK();

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

    return AkinatorErrors::NO_ERR;
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

void AkinatorPrintMenu()
{
    printf("Choose mode: \n");

    printf("1) s - show the wise tree of knowledge\n");
    printf("2) g - guessing mode\n");
    printf("3) d - giving the definition of the word\n");
    printf("4) с - comparing definitions of two word\n");
    printf("5) q - quit without saving new info\n");
    printf("6) l - quit with saving new info\n");
}

AkinatorErrors AkinatorVerify()
{
    if (tree.root == nullptr)
        return AkinatorErrors::TREE_IS_NULLPTR;

    return AkinatorVerify(tree.root);
}

static AkinatorErrors AkinatorVerify(TreeNodeType* node)
{
    if (node == nullptr)
        return AkinatorErrors::NO_ERR;
    
    if ((node->left  == nullptr && node->right != nullptr) ||
        (node->right == nullptr && node->left  != nullptr))
        return AkinatorErrors::ONE_EDGE_IS_NULLPTR;
    
    AkinatorErrors error = AkinatorVerify(node->left);
    if (error != AkinatorErrors::NO_ERR)
        return error;
    
    error = AkinatorVerify(node->right);

    return error;
}

void AkinatorDtor()
{
    TreeDtor(&tree);
}
