#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "Tree.h"
#include "../Common/Log.h"

//TODO: TreeVerify

static void TreePrintPrefixFormat (TreeNodeType* node, FILE* outStream);

static void TreeNodeDtor(TreeNodeType* node);
static void TreeDtor    (TreeNodeType* node);

static void TreeNodeInit(TreeNodeType* node, const char* value, 
                                             TreeNodeType* left, 
                                             TreeNodeType* right);

static TreeNodeType* TreeReadPrefixFormat(const char* const string, const char** stringEndPtr);

static void DotFileCreateNodes(TreeNodeType* node, FILE* outDotFile);
static void TreeGraphicDump   (TreeNodeType* node, FILE* outDotFile);
static inline void CreateImgInLogFile(const size_t imgIndex, bool openImg);

static inline void TreeNodeSetEdges(TreeNodeType* node, TreeNodeType* left, TreeNodeType* right);
static const char* ReadTreeNodeValue(char* target, const char* source);

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

    char* newValueStorage = strdup(value);

    if (newValueStorage == nullptr)
        return TreeErrors::MEM_ERR;

    if (node->value != nullptr)
        free(node->value);
    
    node->value = newValueStorage;

    return TreeErrors::NO_ERR;
}

TreeErrors TreeLeafSetEdges(TreeNodeType* node, TreeNodeType* left, TreeNodeType* right)
{
    assert(node);
    assert(node->left  == nullptr);
    assert(node->right == nullptr);

    node->left  = left;
    node->right = right;

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

void TreePrintPrefixFormat(TreeType* tree, FILE* outStream)
{
    assert(tree);
    
    TreePrintPrefixFormat(tree->root, outStream);

    if (outStream) fprintf(outStream, "\n");
                   Log    (           "\n");
}

static void TreePrintPrefixFormat(TreeNodeType* node, FILE* outStream)
{
    if (node == nullptr)
    {
        if (outStream) fprintf(outStream, "nil ");
                       Log    (           "nil "); //not in if
        return;
    }
    if (outStream) fprintf(outStream, "(");
                   Log    (           "("); //not in if

    if (outStream) fprintf(outStream, "\"%s\" ", node->value);
                   Log    (           "\"%s\" ", node->value); //not in if

    TreePrintPrefixFormat(node->left, outStream);
    TreePrintPrefixFormat(node->right, outStream);

    if (outStream) fprintf(outStream, ")");
                   Log    (           ")"); //not in if
}

//TODO: renaming
void TreeReadPrefixFormat(TreeType* tree, FILE* inStream)
{
    assert(tree);
    assert(inStream);

    char* inputTree      = nullptr;
    size_t inputTreeSize = 0;
    getline(&inputTree, &inputTreeSize, inStream);

    //TODO: обработчик ошибки

    if (inputTree == nullptr)
        return;

    const char* inputTreeEndPtr = inputTree;
    tree->root = TreeReadPrefixFormat(inputTree, &inputTreeEndPtr);

    free(inputTree);
}

static TreeNodeType* TreeReadPrefixFormat(const char* const string, const char** stringEndPtr)
{
    assert(string);

    const char* stringPtr = string;

    //TODO: обернуть в другую функцию пропуска символов
    while (isspace(*stringPtr) && *stringPtr != '\0')
        ++stringPtr;
    static const size_t       maxInputStringLength  = 128;
    static char treeNodeValue[maxInputStringLength] =  "";

    TreeNodeType* node = nullptr;

    int symbol = *stringPtr;
    stringPtr++;
    if (symbol == '(')
    {
        stringPtr = ReadTreeNodeValue(treeNodeValue, stringPtr);
        TreeNodeCtor(&node, treeNodeValue);
    }
    else
    {
        int shift = 0;
        sscanf(stringPtr, "%*s%n", &shift);
        stringPtr += shift;

        *stringEndPtr = stringPtr;
        return nullptr;
    }

    TreeNodeType* left  = TreeReadPrefixFormat(stringPtr, &stringPtr);
    TreeNodeType* right = TreeReadPrefixFormat(stringPtr, &stringPtr);

    //TODO: аналогично в функцию мне кажется
    while (*stringPtr != ')') ++stringPtr;
    ++stringPtr;

    TreeNodeSetEdges(node, left, right);

    *stringEndPtr = stringPtr;
    return node;
}

static const char* ReadTreeNodeValue(char* target, const char* source)
{
    assert(target);
    assert(source);

    if (*source != '\"')
    {
        int shift = 0;
        sscanf(source, "%s%n", target, &shift);

        return source + shift;
    }

    const char* stringEnd = strchr(source + 1, '\"');
    assert(stringEnd != nullptr);

    strncpy(target, source + 1, (size_t)(stringEnd - source));
    target[(size_t)(stringEnd - source) - 1] = '\0';


    return stringEnd + 1;
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
    //TODO: fork + exec
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

void TreeGraphicDump(TreeType* tree, bool openImg)
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

static void DotFileCreateNodes(TreeNodeType* node, FILE* outDotFile)
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

static void TreeGraphicDump(TreeNodeType* node, FILE* outDotFile)
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

void TreeTextDump(TreeType* tree, const char* fileName, 
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

void TreeDump(TreeType* tree, const char* fileName,
                              const char* funcName,
                              const int   line)
{
    assert(tree);
    assert(fileName);
    assert(funcName);

    TreeTextDump(tree, fileName, funcName, line);

    TreeGraphicDump(tree);
}
