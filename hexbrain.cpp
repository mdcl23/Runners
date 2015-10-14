#include "hexbrain.h"
#include "hexutils.h"

#include <QVector>

namespace
{

HexGame::Piece oppositeColor(HexGame::Piece color)
{
    return  (color == HexGame::BlackPiece)
            ? HexGame::WhitePiece
            : HexGame::BlackPiece;
}

enum TemplatePiece { NoneT, MatchT, OtherT, AnyT };

struct Template {
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
            return (color != bPiece && bPiece != HexGame::NonePiece);
            //return (color == HexGame::BlackPiece && bPiece == HexGame::WhitePiece) ||
            //       (color == HexGame::WhitePiece && bPiece == HexGame::BlackPiece);
        } else if (tPiece == AnyT) {
            return true;
        }
        return false;
    }

    int matchTemplate(const HexGame& game, QPoint pos, bool rev, bool trans, HexGame::Piece color) const
    {
        for (uint yi = 0; yi < this->size; yi++)
        {
            for (uint xi = 0; xi < this->size; xi++)
            {
                QPoint pt(xi, yi);
                HexGame::Piece bPiece = game.getPiece(pos + pt);
                TemplatePiece tPiece = this->getPiece(pt, rev, trans);
                if (!matchPiece(tPiece, bPiece, color))
                {
                    return 0;
                }
            }
        }
        return this->score;
    }

    int match(const HexGame& game, HexGame::Piece color) const
    {
        int matchScore = 0;
        for (uint xi = 0; xi < game.size - this->size; xi++)
        {
            for (uint yi = 0; yi < game.size - this->size; yi++)
            {
                QPoint pt(xi, yi);

                matchScore += matchTemplate(game, pt, false, false, color);

                if (this->canReverse) {
                    matchScore += matchTemplate(game, pt, true, false, color);

                    if (this->canTranspose) {
                        matchScore += matchTemplate(game, pt, true, true, color);
                    }
                }

                if (this->canTranspose) {
                    matchScore += matchTemplate(game, pt, false, true, color);
                }
            }
        }
        return matchScore;
    }
};

int score(const HexGame &game, HexGame::Piece color)
{
    QVector<Template> templates;
    templates << Template {-10, true, false, 2,
                           { MatchT, OtherT,
                             OtherT, MatchT }}
              << Template { 1, true, true, 2,
                           { MatchT, MatchT,
                             MatchT, AnyT }}
              << Template { 3, true, true, 3,
                           { AnyT, MatchT, MatchT,
                             AnyT, NoneT, NoneT,
                             AnyT, MatchT, MatchT }}
              << Template { 7, false, true, 3,
                           { AnyT, MatchT, AnyT,
                             AnyT, MatchT, AnyT,
                             AnyT, MatchT, AnyT }};

    HexGame::Piece opposite = oppositeColor(color);

    int score = 0;
    foreach (const Template& t, templates)
    {
        score += t.match(game, color);
        score -= t.match(game, opposite);
    }

    return score;
}

QVector<QPoint> candidatePts(const HexGame& game)
{
    QVector<QPoint> pts;
    for (int yi = 0; yi < game.size; yi++)
    for (int xi = 0; xi < game.size; xi++)
    {
        QPoint pt(xi, yi);
        if (game.getPiece(pt) == HexGame::NonePiece)
        {
            foreach (QPoint n, neighbours4(pt))
            {
                if (game.getPiece(n) != HexGame::NonePiece) {
                    pts << pt;
                    break;
                }
            }
        }
    }

    return pts;
}

int minimax(const HexGame& game, uint depth, HexGame::Piece color) {
    // actually negamax

    HexGame::Piece didWin = game.checkWin();
    if (didWin == color) return INT_MAX;
    if (didWin == oppositeColor(color)) return INT_MIN;
    if (depth == 0) return score(game, color);

    int maxScore = INT_MIN;
    QPoint bestPt;

    foreach (QPoint pt, candidatePts(game))
    {
        HexGame::Piece opponent = oppositeColor(color);

        HexGame g(game);
        g.setPiece(pt, opponent);

        int score = minimax(g, depth-1, opponent);

        if (score > maxScore)
        {
            maxScore = score;
            bestPt = pt;

            static int dbg_i = 0;
            if (((dbg_i++) % 20) == 0)
                debugBoard(g);
        }
    }

    if (maxScore == INT_MIN) maxScore = INT_MAX;
    else if (maxScore == INT_MAX) maxScore = INT_MIN;
    else maxScore = -maxScore;

    return maxScore;
}

}

namespace HexBrain {

QPoint nextMove(const HexGame& game, HexGame::Piece color)
{
    int cnt = 0;
    for (int yi = 0; yi < game.size; yi++)
    for (int xi = 0; xi < game.size; xi++)
        if (game.getPiece(xi, yi) != HexGame::NonePiece) {
            cnt++;
        }

    // if less than few moves played, dont bother calculating
    if (cnt < 6) {
        while (true) {
            QPoint pt(qrand() % game.size, qrand() % game.size);
            if (game.getPiece(pt) == HexGame::NonePiece)
                return pt;
        }
    }

    int maxScore = INT_MIN;
    QPoint bestPt;
    foreach (QPoint pt, candidatePts(game))
    {
        HexGame g(game);
        g.setPiece(pt, color);

        int score = minimax(g, 2, color);
        if (score > maxScore) {
            maxScore = score;
            bestPt = pt;
        }
    }

    return bestPt;
}

}
