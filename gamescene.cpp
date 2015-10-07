#include "gamescene.h"

#include <QSet>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

// helper for move animations
class MoveAnim {
public:
    MoveAnim(QGraphicsItem* target, QPoint destination, QPoint worldDestination, double speed)
        : finished(false)
        , target(target)
        , time(0)
        , speed(speed)
        , delta(speed*(destination-target->scenePos()))
        , worldDestination(worldDestination)
    {
    }

    bool finished;
    QGraphicsItem* target;
    double time; // 0 to 1
    double speed;

    QPointF delta;
    QPoint worldDestination;

    void advance() {
        if (time >= 1.) {
            finished = true;
        } else {
            target->setPos(target->scenePos() + delta);
            time += speed;
        }
    }
};

const qreal GameScene::tile_width = 20;
const qreal GameScene::tile_height = 20;

GameScene::GameScene(const World& world)
    : animTicker()
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
    this->movePlayer(world.playerPosition, 0.05);

    animTicker.setInterval(30);
    connect(&animTicker, SIGNAL(timeout()), this, SLOT(refreshAnimations()));
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

void GameScene::movePlayer(QPoint worldPt, double speed)
{
    if (moves.empty())
    {
        animTicker.start();
    }

    moves.append(new MoveAnim(player, this->worldToScreen(worldPt), worldPt, speed));
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
    QSet<QGraphicsItem*> movedItems;
    QMutableListIterator<MoveAnim*> it(moves);
    while (it.hasNext())
    {
        MoveAnim* anim = it.next();
        if (anim->finished) {
            it.remove();
            emit playerArrivedAt(anim->worldDestination);
            delete anim;
        } else if (!movedItems.contains(anim->target)) {
            anim->advance();
        }
    }

    if (moves.empty()) {
        animTicker.stop();
    }
}

// GameScene events

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mpress " << event->scenePos();
    QPoint worldPos(screenToWorld(event->scenePos().toPoint()));
    this->movePlayer(worldPos, 0.05);

    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPoint worldPos(this->screenToWorld(event->scenePos().toPoint()));
    QPoint hoverPos(this->worldToScreen(worldPos));

    static int d = 0;
    if (!((d++) % 10)) {
        qDebug() << "scene: " << event->scenePos();
        qDebug() << "tile:" << worldPos;
        qDebug() << "hover: " << hoverPos;
    }

    hover->setPos(hoverPos);

    QGraphicsScene::mouseMoveEvent(event);

}

