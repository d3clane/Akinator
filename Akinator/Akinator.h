#ifndef AKINATOR_H
#define AKINATOR_H

enum class AkinatorErrors
{
    NO_ERR,

    NO_DATA_BASE,
};

void AkinatorShowTree();

AkinatorErrors AkinatorLoad();

void AkinatorGuessMode();

void AkinatorGiveDefinition(const char* const word);
void AkinatorCompareWords  (const char* const word1, const char* const word2);

AkinatorErrors AkinatorWriteData();



#endif // AKINATOR_H