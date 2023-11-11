#ifndef AKINATOR_H
#define AKINATOR_H

enum class AkinatorErrors
{
    NO_ERR,

    NO_DATA_BASE,
};

AkinatorErrors AkinatorLoad();
void AkinatorGuessMode();

AkinatorErrors AkinatorWriteData();

void AkinatorShowTree();

#endif // AKINATOR_H