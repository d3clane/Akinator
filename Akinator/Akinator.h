#ifndef AKINATOR_H
#define AKINATOR_H

#include "../Tree/Tree.h"

enum class AkinatorErrors
{
    NO_ERR,

    NO_DATA_BASE,
    ONE_EDGE_IS_NULLPTR,
    TREE_IS_NULLPTR,

    NO_SUCH_WORD_IN_TREE,
    
};

struct AkinatorType
{
    TreeType tree;
};

void AkinatorShowTree(AkinatorType* akinator);

AkinatorErrors AkinatorLoad(AkinatorType* akinator);

AkinatorErrors AkinatorGuessMode(AkinatorType* akinator);

AkinatorErrors AkinatorGiveDefinition(AkinatorType* akinator, const char* const word);
AkinatorErrors AkinatorCompareWords  (AkinatorType* akinator, const char* const word1, const char* const word2);
AkinatorErrors AkinatorVerify(AkinatorType* akinator);

AkinatorErrors AkinatorWriteData(AkinatorType* akinator);

void AkinatorDtor(AkinatorType* akinator);

void AkinatorPrintMenu();


#endif // AKINATOR_H