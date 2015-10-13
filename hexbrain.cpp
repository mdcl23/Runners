#include "hexbrain.h"

HexBrain::HexBrain()
{

}

namespace {
    int howCloseToWin(const HexGame& game, HexGame::Piece color)
    {
        int length = game.size;

        for (int yi = 0; yi < game.size; yi++)
        {
            for (int xi = 0; xi < game.size; xi++)
            {

            }
        }




    }
}


int HexBrain::score(const HexGame &game)
{
    enum Direction { TopBottom, LeftRight };




    // hypothesis TopBottom

    // closest to top edge

    // closest to left edge

    // hypothesis LeftRight
}
