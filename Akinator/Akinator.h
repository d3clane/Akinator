#ifndef AKINATOR_H
#define AKINATOR_H

enum class AkinatorErrors
{
    NO_ERR,

    NO_DATA_BASE,
    ONE_EDGE_IS_NULLPTR,
    TREE_IS_NULLPTR,

    NO_SUCH_WORD_IN_TREE,
    
};

void AkinatorShowTree();

AkinatorErrors AkinatorLoad();

AkinatorErrors AkinatorGuessMode();

AkinatorErrors AkinatorGiveDefinition(const char* const word);
AkinatorErrors AkinatorCompareWords  (const char* const word1, const char* const word2);
AkinatorErrors AkinatorVerify();

AkinatorErrors AkinatorWriteData();

void AkinatorPrintMenu();


#endif // AKINATOR_H