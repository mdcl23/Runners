#ifndef LANDSCAPEWIDGET_H
#define LANDSCAPEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsRectItem>
#include "World.h"

class LandscapeWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LandscapeWidget(QWidget *parent = 0);
    ~LandscapeWidget();

    QPoint screenToWorld(QPoint screenPt) const;

signals:

public slots:

private:
    World world;

    QVector<QGraphicsRectItem*> tiles;
    QGraphicsItem* playerItem;
    QGraphicsScene* scene;

    static const qreal tile_width;
    static const qreal tile_height;
protected:
    virtual void mousePressEvent(QMouseEvent* event);

};

#endif // LANDSCAPEWIDGET_H
