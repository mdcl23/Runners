#include "gamescene.h"

#include <cmath>
#include <QSet>
#include <QGraphicsSceneMouseEvent>

// helper for move animations
class MoveAnim
{
public:
    MoveAnim(QPoint start, QPoint end)
        : start(start)
        , end(end)
        , currentPos(start)
        , finished(false)
    {}

    QPointF start;
    QPointF end;
    QPointF currentPos;

    bool finished;

    QGraphicsLineItem* line;

    void advance()
    {
        QPointF unit = (end - start)/sqrt(QPointF::dotProduct(end - start, end - start));
        currentPos += 4.*unit;

        QLineF l = line->line();
        l.setP1(currentPos + QPoint(GameScene::tile_width/2, GameScene::tile_height/2));
        line->setLine(l);

        if ((currentPos - end).manhattanLength() < 4.) {
            finished = true;
        }
    }
};

const qreal GameScene::tile_width = 20;
const qreal GameScene::tile_height = 20;

GameScene::GameScene(const World& world)
    : animTicker()
    , portal(new QGraphicsPixmapItem(QPixmap(":/portal.png")))
{
    hover = this->addRect(0, 0,
                          tile_width, tile_height,
                          QColor(255, 64, 255),
                          QColor(128,128,255,64));
    hover->setZValue(100);

    player = this->addEllipse(0,
                              0,
                               20, 20,
                               QColor(255, 0, 0),
                               QColor(128, 32, 32));
    player->setZValue(99);
    this->player->setPos(worldToScreen(world.playerPosition));

    animTicker.setInterval(10);
    animTicker.start();
    connect(&animTicker, SIGNAL(timeout()), this, SLOT(refreshAnimations()));

    portal->setPos(worldToScreen(QPoint(world.landscape.width - 22,
                                        world.landscape.height - 18)));
    portal->setZValue(50);
    this->addItem(portal);

}

GameScene::~GameScene()
{
}

void GameScene::addTile(QPoint worldPt, Tile tile)
{
    this->addRect(tile_width*worldPt.x(), tile_height*worldPt.y(),
                  tile_width, tile_height,
                  QColor(0,0,0,64),
                  tile.brush());
}

void GameScene::setMap(const Landscape& lc)
{
    for (int yi = 0; yi < lc.height; yi++)
    {
        for (int xi = 0; xi < lc.width; xi++)
        {
            this->addTile(QPoint(xi, yi), lc.getTile(xi, yi));
        }
    }
}

void GameScene::clearPath()
{
    while (moves.count() > 1) {
        moves.pop_back();
    }

    foreach(QGraphicsItem* i, pathItems)
    {
        i->setVisible(false);
    }
    pathItems.clear();
}

void GameScene::showPath(QVector<QPoint> newPath)
{
    if (!moves.empty()) {
        player->setPos(moves.front()->start);
    }

    this->clearPath();

    QPoint prev = newPath.front();
    newPath.pop_front();

    const QPoint offs(tile_width/2, tile_height/2);
    foreach(QPoint cs, newPath) {
        QPoint start = worldToScreen(prev);
        QPoint end = worldToScreen(cs);

        pathItems.append(this->addLine(QLineF(start + offs, end + offs), QPen(QColor("black"))));
        prev = cs;
        moves.append(new MoveAnim(start, end));
        moves.back()->line = pathItems.back();
    }
}

QPoint GameScene::screenToWorld(QPoint screenPt) const
{
    return QPoint(
               screenPt.x()/tile_width,
               screenPt.y()/tile_height);
}

QPoint GameScene::worldToScreen(QPoint worldPt) const
{
    return QPoint(
                worldPt.x()*tile_width,
                worldPt.y()*tile_height);
}

// GameScene animations

void GameScene::refreshAnimations()
{
    if (moves.empty()) return;

    moves.front()->advance();
    player->setPos(moves[0]->currentPos.toPoint());

    if (moves.front()->finished) {
        this->removeItem(moves.front()->line);
        pathItems.removeAll(moves.front()->line);
        delete moves.front()->line;

        QPoint arrivalPt = screenToWorld(moves.front()->end.toPoint());
        emit playerArrivedAt(arrivalPt);


        delete moves.front();
        moves.pop_front();

        if (player->collidesWithItem(portal)) {
            emit playerCollidesWithPortal();
        }
    }
}

// GameScene events

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint worldPos(screenToWorld(event->scenePos().toPoint()));

    emit tileClicked(worldPos);

    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint worldPos(this->screenToWorld(event->scenePos().toPoint()));
    QPoint hoverPos(this->worldToScreen(worldPos));

    hover->setPos(hoverPos);

    QGraphicsScene::mouseMoveEvent(event);

}

