#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "Tree.h"
#include "../Common/Log.h"
#include "../Common/InputOutputFuncs.h"
#include "../Common/StringFuncs.h"

static TreeErrors TreePrintPrefixFormat (const TreeNodeType* node, FILE* outStream);
static TreeNodeType* TreeReadPrefixFormatFast(const char* const string, const char** stringEndPtr);
static TreeNodeType* TreeReadPrefixFormat(FILE* inStream);

static void TreeNodeDtor(TreeNodeType* node);
static void TreeDtor    (TreeNodeType* node);

static void TreeNodeInit(TreeNodeType* node, const char* value, 
                                             TreeNodeType* left, 
                                             TreeNodeType* right);

static void DotFileCreateNodes(const TreeNodeType* node, FILE* outDotFile);
static void TreeGraphicDump   (const TreeNodeType* node, FILE* outDotFile);
static inline void CreateImgInLogFile(const size_t imgIndex, bool openImg);

static inline void TreeNodeSetEdges(TreeNodeType* node, TreeNodeType* left, TreeNodeType* right);

static const char* TreeReadNodeValuePrefixFormatFast(char* target, const size_t maxTargetLength,
                                                     const char* source);
static void TreeReadNodeValuePrefixFormat(char* target, const size_t maxTargetLength, FILE* inStream);

static bool TreeGetPath(const TreeNodeType* node, const char* const word, StackType* path);

static TreeErrors TreeVerify(const TreeNodeType* node);

#define TREE_CHECK(tree)                       \
do                                             \
{                                              \
    TreeErrors err = TreeVerify(tree);         \
    if (err != TreeErrors::NO_ERR)             \
    {                                          \
        TREE_ERRORS_LOG_ERROR(err);            \
        return err;                            \
    }                                          \
} while (0)

#define NODE_CHECK(node)                       \
do                                             \
{                                              \
    TreeErrors err = TreeNodeVerify(node);     \
    if (err != TreeErrors::NO_ERR)             \
    {                                          \
        TREE_ERRORS_LOG_ERROR(err);            \
        return err;                            \
    }                                          \
} while (0)


TreeErrors TreeCtor(TreeType* tree, size_t treeSize, TreeNodeType* root)
{
    assert(tree);

    if (root != nullptr)
    {
        tree->root = root;
        tree->size = treeSize;

        return TreeErrors::NO_ERR;
    }

    TreeNodeType* newRoot = nullptr;

    tree->root = newRoot;
    tree->size = 0;

    TREE_CHECK(tree);

    return TreeErrors::NO_ERR;
}

TreeErrors TreeDtor(TreeType* tree)
{
    assert(tree);

    TreeDtor(tree->root);
    tree->root = nullptr;
    tree->size = 0;

    return TreeErrors::NO_ERR;
}

TreeErrors TreeNodeSetValue(TreeNodeType* node, const char* value)
{
    assert(node);

    NODE_CHECK(node);

    char* newValueStorage = strdup(value);

    if (newValueStorage == nullptr)
        return TreeErrors::MEM_ERR;

    if (node->value != nullptr)
        free(node->value);
    
    node->value = newValueStorage;

    NODE_CHECK(node);

    return TreeErrors::NO_ERR;
}

TreeErrors TreeLeafSetEdges(TreeNodeType* node, TreeNodeType* left, TreeNodeType* right)
{
    assert(node);
    assert(node->left  == nullptr);
    assert(node->right == nullptr);

    NODE_CHECK(node);

    node->left  = left;
    node->right = right;

    NODE_CHECK(node);

    return TreeErrors::NO_ERR;
}

static void TreeDtor(TreeNodeType* node)
{
    if (node == nullptr)
        return;
    
    TreeDtor(node->left);
    TreeDtor(node->right);

    TreeNodeDtor(node);
}

TreeErrors TreeNodeCtor(TreeNodeType** node, const char* value, TreeNodeType* left, 
                                                                TreeNodeType* right)
{
    assert(node);

    *node = (TreeNodeType*) calloc(1, sizeof(**node));

    if (*node == nullptr)
        return TreeErrors::MEM_ERR;

    (*node)->value = strdup(value);
    (*node)->left  =  left;
    (*node)->right = right;

    NODE_CHECK(*node);

    return TreeErrors::NO_ERR;
}

static void TreeNodeDtor(TreeNodeType* node)
{
    assert(node);

    free(node->value);

    node->value = nullptr;
    node->left  = nullptr;
    node->right = nullptr;

    free(node);
}

#define PRINT(outStream, ...)                          \
do                                                     \
{                                                      \
    if (outStream) fprintf(outStream, __VA_ARGS__);    \
    Log(__VA_ARGS__);                                  \
} while (0)

TreeErrors TreePrintPrefixFormat(const TreeType* tree, FILE* outStream)
{
    assert(tree);
    
    TREE_CHECK(tree);

    TreeErrors err = TreePrintPrefixFormat(tree->root, outStream);

    PRINT(outStream, "\n");

    return err;
}

static TreeErrors TreePrintPrefixFormat(const TreeNodeType* node, FILE* outStream)
{
    if (node == nullptr)
    {
        PRINT(outStream, "nil ");
        return TreeErrors::NO_ERR;
    }

    PRINT(outStream, "(");

    PRINT(outStream, "\"%s\" ", node->value);

    TreeErrors err = TreeErrors::NO_ERR;

    err = TreePrintPrefixFormat(node->left, outStream);
    err = TreePrintPrefixFormat(node->right, outStream);

    PRINT(outStream, ")");
    
    return err;
}

#undef PRINT

TreeErrors TreeReadPrefixFormatFast(TreeType* tree, FILE* inStream)
{
    assert(tree);
    assert(inStream);

    char* inputTree      = nullptr;
    size_t inputTreeSize = 0;
    getline(&inputTree, &inputTreeSize, inStream);

    if (inputTree == nullptr)
        return TreeErrors::MEM_ERR;

    const char* inputTreeEndPtr = inputTree;
    tree->root = TreeReadPrefixFormatFast(inputTree, &inputTreeEndPtr);

    TREE_CHECK(tree);

    free(inputTree);

    return TreeErrors::NO_ERR;
}

TreeErrors TreeReadPrefixFormat(TreeType* tree, FILE* inStream)
{
    assert(tree);
    assert(inStream);

    TREE_CHECK(tree);

    tree->root = TreeReadPrefixFormat(inStream);

    return TreeErrors::NO_ERR;
}

static TreeNodeType* TreeReadPrefixFormat(FILE* inStream)
{
    assert(inStream);

    SkipSymbolsWhileStatement(inStream, isspace);

    static const size_t       maxInputStringLength  = 128;
    static char treeNodeValue[maxInputStringLength] =  "";

    TreeNodeType* node = nullptr;

    int symbol = getc(inStream);
    if (symbol != '(')
    {
        fscanf(inStream, "%*s");
        return nullptr;     
    }

    TreeReadNodeValuePrefixFormat(treeNodeValue, maxInputStringLength, inStream);
    TreeNodeCtor(&node, treeNodeValue);

    TreeNodeType* left  = TreeReadPrefixFormat(inStream);
    TreeNodeType* right = TreeReadPrefixFormat(inStream);

    SkipSymbolsUntilStopChar(inStream, ')');
    getc(inStream);

    TreeNodeSetEdges(node, left, right);

    return node;
}

static TreeNodeType* TreeReadPrefixFormatFast(const char* const string, const char** stringEndPtr)
{
    assert(string);

    const char* stringPtr = string;

    stringPtr = SkipSymbolsWhileStatement(stringPtr, isspace);

    static const size_t       maxInputStringLength  = 128;
    static char treeNodeValue[maxInputStringLength] =  "";

    TreeNodeType* node = nullptr;

    int symbol = *stringPtr;
    stringPtr++;
    if (symbol != '(')
    {
        int shift = 0;
        sscanf(stringPtr, "%*s%n", &shift);
        stringPtr += shift;

        *stringEndPtr = stringPtr;
        return nullptr;
    }

    stringPtr = TreeReadNodeValuePrefixFormatFast(treeNodeValue, maxInputStringLength, stringPtr);
    TreeNodeCtor(&node, treeNodeValue);

    TreeNodeType* left  = TreeReadPrefixFormatFast(stringPtr, &stringPtr);
    TreeNodeType* right = TreeReadPrefixFormatFast(stringPtr, &stringPtr);

    stringPtr = SkipSymbolsUntilStopChar(stringPtr, ')');
    ++stringPtr;

    TreeNodeSetEdges(node, left, right);

    *stringEndPtr = stringPtr;
    return node;
}

static const char* TreeReadNodeValuePrefixFormatFast(char* target, const size_t maxTargetLength, 
                                                     const char* source)
{
    assert(target);
    assert(source);

    if (*source != '\"')
    {
        int numberOfSpaces = 0;
        int shift = 0;
        sscanf(source, "%n%s%n", &numberOfSpaces, target, &shift);
        assert(shift <= (int)maxTargetLength + numberOfSpaces);

        return source + shift;
    }

    const char* stringEnd = strchr(source + 1, '\"');
    assert(stringEnd != nullptr);
    assert((size_t)(stringEnd - source) <= maxTargetLength);

    strncpy(target, source + 1, (size_t)(stringEnd - source));
    target[(size_t)(stringEnd - source) - 1] = '\0';

    return stringEnd + 1;
}

static void TreeReadNodeValuePrefixFormat(char* target, const size_t maxTargetLength, FILE* inStream)
{
    assert(target);
    assert(inStream);

    int symbol = getc(inStream);

    if (symbol != '\"')
    {
        ungetc(symbol, inStream);
        fscanf(inStream, "%s", target);
        return;
    }

    size_t i = 0;
    while (true)
    {
        assert(i < maxTargetLength);

        symbol = getc(inStream);

        if (symbol == EOF || symbol == '\"')
            break;
        
        target[i]  = (char)symbol;
        ++i;
    }

    target[i] = '\0';
}

static inline void TreeNodeSetEdges(TreeNodeType* node, TreeNodeType* left, TreeNodeType* right)
{
    assert(node);

    node->left  = left;
    node->right = right;
}

static inline void TreeNodeInit(TreeNodeType* node, const char* value, 
                                                    TreeNodeType* left, 
                                                    TreeNodeType* right)
{
    assert(node);

    node->value = strdup(value);
    node->left  = left;
    node->right = right;
}

static inline void CreateImgInLogFile(const size_t imgIndex, bool openImg)
{
    static const size_t maxImgNameLength  = 64;
    static char imgName[maxImgNameLength] = "";
    snprintf(imgName, maxImgNameLength, "../imgs/img_%zu_time_%s.png", imgIndex, __TIME__);

    static const size_t     maxCommandLength  = 128;
    static char commandName[maxCommandLength] =  "";
    snprintf(commandName, maxCommandLength, "dot list.dot -T png -o %s", imgName);
    system(commandName);

    snprintf(commandName, maxCommandLength, "<img src = \"%s\">\n", imgName);    
    Log(commandName);

    if (openImg)
    {
        snprintf(commandName, maxCommandLength, "open %s", imgName);
        system(commandName);
    }
}

static inline void DotFileBegin(FILE* outDotFile)
{
    fprintf(outDotFile, "digraph G{\nrankdir=TB;\ngraph [bgcolor=\"#31353b\"];\n");
}

static inline void DotFileEnd(FILE* outDotFile)
{
    fprintf(outDotFile, "\n}\n");
}

void TreeGraphicDump(const TreeType* tree, bool openImg)
{
    assert(tree);

    static const char* dotFileName = "list.dot";
    FILE* outDotFile = fopen(dotFileName, "w");

    if (outDotFile == nullptr)
        return;

    DotFileBegin(outDotFile);

    DotFileCreateNodes(tree->root, outDotFile);

    TreeGraphicDump(tree->root, outDotFile);

    DotFileEnd(outDotFile);

    fclose(outDotFile);

    static size_t imgIndex = 0;
    CreateImgInLogFile(imgIndex, openImg);
    imgIndex++;
}

static void DotFileCreateNodes(const TreeNodeType* node, FILE* outDotFile)
{
    if (node == nullptr)
        return;
    
    fprintf(outDotFile, "node%p"
                        "[shape=Mrecord, style=filled, fillcolor=\"#7293ba\","
                        "label = \"{ %s |"  
                        "{ <left> yes | <right> no } } \" ," 
                        "color = \"#008080\"];\n",
                        node, node->value); 

    DotFileCreateNodes(node->left,  outDotFile);
    DotFileCreateNodes(node->right, outDotFile);
}

static void TreeGraphicDump(const TreeNodeType* node, FILE* outDotFile)
{
    if (node == nullptr)
    {
        fprintf(outDotFile, "\n");
        return;
    }
    
    fprintf(outDotFile, "node%p;\n", node);

    if (node->left != nullptr) fprintf(outDotFile, "node%p:<left>->", node);
    TreeGraphicDump(node->left, outDotFile);

    if (node->right != nullptr) fprintf(outDotFile, "node%p:<right>->", node);
    TreeGraphicDump(node->right, outDotFile);
}

void TreeTextDump(const TreeType* tree, const char* fileName, 
                                        const char* funcName,
                                        const int   line)
{
    assert(tree);
    assert(fileName);
    assert(funcName);

    LogBegin(fileName, funcName, line);

    Log("Tree root: %p, value: %s\n", tree->root, tree->root->value);
    Log("Tree: ");
    TreePrintPrefixFormat(tree, nullptr);

    LOG_END();
}

void TreeDump(const TreeType* tree, const char* fileName,
                                    const char* funcName,
                                    const int   line)
{
    assert(tree);
    assert(fileName);
    assert(funcName);

    TreeTextDump(tree, fileName, funcName, line);

    TreeGraphicDump(tree);
}

bool TreeGetPath(const TreeType* tree, const char* const word, StackType* path)
{
    assert(tree);
    assert(word);
    assert(path);

    if (TreeVerify(tree) != TreeErrors::NO_ERR)
        return false;
    
    return TreeGetPath(tree->root, word, path);
}

static bool TreeGetPath(const TreeNodeType* node, const char* const word, StackType* path)
{
    if (node == nullptr)
        return false;   

    if (strcmp(node->value, word) == 0)
        return true;

    bool found = TreeGetPath(node->left, word, path);

    if (found)
        StackPush(path, 1);
    else
    {
        found = TreeGetPath(node->right, word, path);

        if (found) 
            StackPush(path, 0);
    }

    return found;
}

TreeErrors TreeVerify(const TreeType* tree)
{
    assert(tree);

    return TreeVerify(tree->root);
}

static TreeErrors TreeVerify(const TreeNodeType* node)
{
    if (node == nullptr)
        return TreeErrors::NO_ERR;
    
    TreeErrors err = TreeNodeVerify(node);
    if (err != TreeErrors::NO_ERR)
        return err;
    
    err = TreeVerify(node->left);
    if (err != TreeErrors::NO_ERR)
        return err;

    err = TreeVerify(node->right);
    return err;
} 

TreeErrors TreeNodeVerify(const TreeNodeType* node)
{
    assert(node);

    if (node->left == node->right && node->left != nullptr)
        return TreeErrors::DUPLICATE_EDGES;
    
    if (node->left == node)
        return TreeErrors::LOOP;
    
    if (node->right == node)
        return TreeErrors::LOOP;

    return TreeErrors::NO_ERR;
}

void TreeErrorsLogError(const TreeErrors err, const char* fileName,
                                              const char* funcName,
                                              const int   line)
{
    assert(fileName);
    assert(funcName);

    LogBegin(fileName, funcName, line);

    switch(err)
    {
        case TreeErrors::DUPLICATE_EDGES:
            Log("Duplicating edges. Not a tree\n");
            break;
        case TreeErrors::LOOP:
            Log("Loop edges in tree.\n");
            break;
        case TreeErrors::MEM_ERR:
            Log("Error allocating memory\n");
            break;
        
        case TreeErrors::NO_ERR:
        default:
            break;
    }

    LOG_END();
}
