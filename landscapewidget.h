#ifndef LANDSCAPEWIDGET_H
#define LANDSCAPEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsRectItem>
#include "World.h"

class GameScene;

class LandscapeWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LandscapeWidget(QWidget *parent = 0);
    ~LandscapeWidget();

    QPoint screenToWorld(QPoint screenPt) const;
    QPoint worldToScreen(QPoint worldPt) const;

signals:

public slots:

private:
    World world;

    QVector<QGraphicsRectItem*> tiles;
    QGraphicsItem* playerItem;
    GameScene* scene;

};

#endif // LANDSCAPEWIDGET_H
