#include "hexutils.h"

#include <QDebug>

uint qHash(const QPoint& pt)
{
    return qHash(QString("%1:%2").arg(pt.x(), pt.y()));
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

