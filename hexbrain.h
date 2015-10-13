#ifndef HEXBRAIN_H
#define HEXBRAIN_H

#include "hexgame.h"

class HexBrain
{
public:
    HexBrain();

    static int score(const HexGame& game);
};

#endif // HEXBRAIN_H
