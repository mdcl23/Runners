#include "hexgame.h"

#include <QVector>
#include <QMap>
#include <QSet>
#include <QStack>

#include <qDebug>
#include <iterator>

HexGame::HexGame(int size)
    : board(size)
    , size(size)
{

}

HexGame::HexGame(const HexGame& game)
    : board(game.size)
    , size(game.size)
{
    std::copy(std::begin(game.board), std::end(game.board), std::begin(this->board));
}

HexGame::~HexGame()
{
}

HexGame::Piece HexGame::getPiece(int x, int y) const
{
    return board[x + y*size];
}

HexGame::Piece HexGame::getPiece(QPoint cs) const
{
    return this->getPiece(cs.x(), cs.y());
}

void HexGame::setPiece(int x, int y, HexGame::Piece piece)
{
    this->setPiece(QPoint(x,y), piece);
}

void HexGame::setPiece(QPoint cs, HexGame::Piece piece)
{
    if (QRect(0, 0, size, size).contains(cs))
    {
        board[cs.x() + cs.y()*size]  = piece;
    }
}

uint qHash(const QPoint& pt)
{
    return qHash(QString("%1:%2").arg(pt.x(), pt.y()));
}

namespace {
void debugBoard(const HexGame& g) {
    qDebug() << "++";
    for (int yi = 0; yi < g.size; yi++) {
        QString s;
        for (int xi = 0; xi < g.size; xi++) {
            s += (g.getPiece(xi, yi) == HexGame::BlackPiece)
                ? "X"
                : (g.getPiece(xi, yi) == HexGame::WhitePiece)
                  ? "O"
                  : " ";
        }
        qDebug() << s;
    }
}

    void testPt(const HexGame& game, QPoint pt, QMap<HexGame::Piece, QSet<QPoint>>& cases)
    {
        HexGame::Piece p(game.getPiece(pt));
        if (p != HexGame::NonePiece) {
            cases[p].insert(pt);
        }
    }

    QVector<QPoint> neighbours4(QPoint pt)
    {
        QVector<QPoint> ns;
        for (int i = -1; i < 2; i += 2)
        {
            ns.append(pt + QPoint(i,0));
            ns.append(pt + QPoint(0,i));
        }
        return ns;
    }
}

HexGame::Piece HexGame::checkWin() const
{
    QMap<Piece, QSet<QPoint>> borderPts;

    for (int i = 0; i < size; i++)
    {
        testPt(*this, QPoint(i, 0),      borderPts);
        testPt(*this, QPoint(i, size-1), borderPts);
        testPt(*this, QPoint(0, i),      borderPts);
        testPt(*this, QPoint(size-1, i), borderPts);
    }

    QRect boardRect(0,0,size,size);
    QMapIterator<Piece, QSet<QPoint>> ptsIter(borderPts);
    while (ptsIter.hasNext())
    {
        ptsIter.next();
        Piece plr = ptsIter.key();
        foreach (QPoint startPt, ptsIter.value())
        {
            QSet<QPoint> visited;
            QStack<QPoint> toVisit;

            toVisit.push(startPt);
            while(!toVisit.empty())
            {
                QPoint current = toVisit.pop();
                visited.insert(current);

                if (this->getPiece(current) != plr)
                    continue;

                if (abs(startPt.x() - current.x()) == size-1 ||
                    abs(startPt.y() - current.y()) == size-1)
                {
                    return plr;
                }

                // next to visit
                foreach (QPoint n, neighbours4(current)) {
                    if (!visited.contains(n) &&
                        !toVisit.contains(n) &&
                        boardRect.contains(n))
                    {
                        toVisit.push(n);
                    }
                }
            }
        }
    }

    return NonePiece;
}

