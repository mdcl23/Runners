#ifndef LANDSCAPEWIDGET_H
#define LANDSCAPEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsRectItem>
#include <QTimer>
#include "World.h"

class MoveAnim; // Helper class for movements

class GameScene : public QGraphicsScene {
    Q_OBJECT

public:
    // constants for tile sizes
    static const qreal tile_width;
    static const qreal tile_height;

    GameScene(const World& world);
    virtual ~GameScene();

    void setMap(const Landscape& lc);
    void addTile(QPoint worldPt, Tile tile);

    QPoint screenToWorld(QPoint screenPt) const;
    QPoint worldToScreen(QPoint worldPt) const;

    QGraphicsItem* hover;
    QGraphicsItem* player;

    void movePlayer(QPoint worldPt, double speed);

public slots:
    void refreshAnimations();

signals:
     // emitted when animated item arrives at certain point
    void playerArrivedAt(QPoint worldPt);

private:

    // variables to handle movement animation
    QTimer animTicker;
    QList<MoveAnim*> moves; // this will store animations of moves

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};

class LandscapeWidget : public QGraphicsView
{
    Q_OBJECT
public:
    explicit LandscapeWidget(QWidget *parent = 0);
    ~LandscapeWidget();

    QPoint screenToWorld(QPoint screenPt) const;
    QPoint worldToScreen(QPoint worldPt) const;

    qreal zoom;

signals:

public slots:

private:
    World world;

    QVector<QGraphicsRectItem*> tiles;
    QGraphicsItem* playerItem;
    GameScene* scene;

protected:
    void keyReleaseEvent(QKeyEvent* event);

};

#endif // LANDSCAPEWIDGET_H
