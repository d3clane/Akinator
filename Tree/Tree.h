#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include "../Stack/Stack.h"

struct TreeNodeType
{
    char* value;

    TreeNodeType*  left;
    TreeNodeType* right;
};

struct TreeType
{
    TreeNodeType* root;
}; 

enum class TreeErrors
{
    NO_ERR,

    MEM_ERR,

    DUPLICATE_EDGES,
    LOOP,
};

TreeErrors TreeCtor(TreeType* tree, TreeNodeType* root = nullptr);
TreeErrors TreeDtor(TreeType* tree);

TreeErrors TreeNodeCtor(TreeNodeType** node, const char* value, TreeNodeType* left  = nullptr, 
                                                                TreeNodeType* right = nullptr);
TreeErrors TreeVerify    (const TreeType*     tree);
TreeErrors TreeNodeVerify(const TreeNodeType* node);

TreeErrors TreeNodeSetValue(TreeNodeType* node, const char* value);
TreeErrors TreeLeafSetEdges(TreeNodeType* node, TreeNodeType* left, TreeNodeType* right);

TreeErrors TreePrintPrefixFormat(const TreeType* tree, FILE* outStream = stdout);

TreeErrors TreeReadPrefixFormatFast(TreeType* tree, FILE* inStream = stdin);
TreeErrors TreeReadPrefixFormat    (TreeType* tree, FILE* inStream = stdin);

#define TREE_TEXT_DUMP(tree) TreeTextDump((tree), __FILE__, __func__, __LINE__)
void TreeTextDump(const TreeType* tree, const char* fileName, 
                                        const char* funcName,
                                        const int   line);
void TreeGraphicDump(const TreeType* tree, bool openImg = false);

#define TREE_DUMP(tree) TreeDump((tree), __FILE__, __func__, __LINE__)
void TreeDump(const TreeType* tree, const char* fileName,
                                    const char* funcName,
                                    const int   line);

bool TreeGetPath(const TreeType* tree, const char* const word, StackType* path);

#define TREE_ERRORS_LOG_ERROR(err) TreeErrorsLogError(err, __FILE__, __func__, __LINE__);
void TreeErrorsLogError(const TreeErrors err, const char* fileName,
                                              const char* funcName,
                                              const int   line);

#endif