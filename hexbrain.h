#ifndef HEXBRAIN_H
#define HEXBRAIN_H

#include "hexgame.h"

namespace HexBrain
{

QPoint nextMove(const HexGame& game, HexGame::Piece color);

}

#endif // HEXBRAIN_H
