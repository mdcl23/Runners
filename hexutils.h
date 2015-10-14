#ifndef HEXUTILS_H
#define HEXUTILS_H

#include <QVector>
#include <QPoint>
#include <QHash>
#include "hexgame.h"

uint qHash(const QPoint& pt);
QVector<QPoint> neighbours4(QPoint pt);
void debugBoard(const HexGame& g);

#endif // HEXUTILS_H
