#ifndef HEXGAME_H
#define HEXGAME_H

#include <QObject>
#include <QPoint>
#include <QRect>

class HexGame : QObject
{
    Q_OBJECT
public:
    enum Piece { NonePiece = 0, BlackPiece, WhitePiece };

    HexGame(int size);
    ~HexGame();

    Piece* board;
    int size;

    Piece getPiece(int x, int y) const;
    Piece getPiece(QPoint cs) const;

    void setPiece(int x, int y, Piece piece);
    void setPiece(QPoint cs, Piece piece);

    Piece checkWin() const;
signals:

public slots:
};

#endif // HEXGAME_H
