#include "hexbrain.h"

HexBrain::HexBrain()
{

}

struct Template {
    enum TemplatePiece { NoneT, MatchT, OtherT, AnyT };

    int score;

    bool canReverse;
    bool canTranspose;

    uint size;
    TemplatePiece templ[9];

    TemplatePiece getPiece(QPoint pt, bool rev, bool trans) const
    {
        if (trans) { pt = QPoint(pt.y(), pt.x()); }
        if (rev) { pt = QPoint(size-1 - pt.x(), pt.y()); }
        return templ[pt.x() + pt.y()*size];
    }

    bool matchPiece(TemplatePiece tPiece, HexGame::Piece bPiece, HexGame::Piece color) const
    {
        if (tPiece == NoneT) {
            return bPiece == HexGame::NonePiece;
        } else if (tPiece == MatchT) {
            return bPiece == color;
        } else if (tPiece == OtherT) {
            return (color == HexGame::BlackPiece && bPiece == HexGame::WhitePiece) ||
                   (color == HexGame::WhitePiece && bPiece == HexGame::BlackPiece);
        } else if (tPiece == AnyT) {
            return true;
        }
        return false;
    }

    int matchTemplate(const HexGame& game, QPoint pos, bool reverse, bool transpose, HexGame::Piece color) const
    {
        for (uint yi = 0; yi < this->size; yi++)
        {
            for (uint xi = 0; xi < this->size; xi++)
            {
                HexGame::Piece bPiece = game.getPiece(pos + QPoint(xi, yi));
                TemplatePiece tPiece = this->getPiece(QPoint(xi, yi), reverse, transpose);
                if (!matchPiece(tPiece, bPiece, color))
                {
                    return 0;
                }
            }
        }
        return this->score;
    }

    int match(const HexGame& game, bool reverse, bool transpose, HexGame::Piece color) const
    {
        int matchScore = 0;
        for (uint xi = 0; xi < game.size - this->size; xi++)
        {
            for (uint yi = 0; yi < game.size - this->size; yi++)
            {
                matchScore += matchTemplate(game, QPoint(xi, yi), reverse, transpose, color);
            }
        }
        return matchScore;
    }
};

int HexBrain::score(const HexGame &game)
{

    // rev, no trans
    // +0
    // XO
    // OX

    // rev, trans
    // +1
    // XX
    // X?

    // rev, no trans
    // +1
    // X.
    // .X

    // rev, trans
    // +1
    // ?XX
    // ?..
    // ?XX

    // no rev, trans
    // ?X?
    // ?X?
    // ?X?

    return 1;
}
