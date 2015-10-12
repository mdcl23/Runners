#ifndef HEXSCENE_H
#define HEXSCENE_H

#include <QGraphicsScene>
#include "hexgame.h"

class HexScene : public QGraphicsScene
{
    Q_OBJECT

public:
    static int tile_width;
    static int tile_height;

    HexScene(int size);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    QVector<QGraphicsRectItem*> pieces;

    bool blackTurn;

    HexGame game;
private:
    QPoint screenToWorld(QPoint pt) const;
    QPoint worldToScreen(QPoint pt) const;

signals:

public slots:
};

#endif // HEXSCENE_H
