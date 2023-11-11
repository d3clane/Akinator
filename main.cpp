#include "Tree.h"
#include "Log.h"

int main(const int argc, const char* argv[])
{
    //setbuf(stdout, nullptr);
    LogOpen(argv[0]);

    TreeNodeType* node10, *node5, *node17, *node15, *node20, *node12, *node78, *node123, *node1337, *node228;
    TreeType tree;

    TreeNodeCtor(&node20, "big");
    TreeNodeCtor(&node228, "small");
    TreeNodeCtor(&node78, "aboba");
    TreeNodeCtor(&node123, "bebra", node228, node78);
    TreeNodeCtor(&node1337, "ahahh", node123, nullptr);
    
    TreeNodeCtor(&node12, "lol", node1337);
    TreeNodeCtor(&node15, "xxx", node12);
    TreeNodeCtor(&node20, "boofey");
    TreeNodeCtor(&node17, "rkvadro", node15, node20);
    TreeNodeCtor(&node5, "zebra");
    TreeNodeCtor(&node10, "trewww", node5, node17);

    TreeCtor(&tree, 5, node10);

    tree.root = node10;

    //TreePrintPrefixFormat (&tree);
    //TreePrintPostfixFormat(&tree);
    //TreePrintInfixFormat  (&tree);

    TREE_DUMP(&tree);

    TreeDtor(&tree);

    TreeCtor(&tree);
    TreeReadPrefixFormat(&tree);

    //TreePrintPrefixFormat (&tree);
    //TreePrintPostfixFormat(&tree);
    //TreePrintInfixFormat  (&tree);

    TREE_DUMP(&tree);
}