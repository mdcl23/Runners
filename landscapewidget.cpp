#include "landscapewidget.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

#include "landscape.h"

class GameScene : public QGraphicsScene {

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

    void movePlayer(QPoint worldPt);

private:
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};

const qreal GameScene::tile_width = 20;
const qreal GameScene::tile_height = 20;


GameScene::GameScene(const World& world)
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
    this->movePlayer(world.playerPosition);
}

GameScene::~GameScene()
{
}

void GameScene::addTile(QPoint worldPt, Tile tile)
{
    this->addRect(tile_width*worldPt.x(), tile_height*worldPt.y(),
                  tile_width, tile_height,
                  QPen(),
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

void GameScene::movePlayer(QPoint worldPt)
{
    qDebug() << "movePlayer: " << worldPt;
    player->setPos(this->worldToScreen(worldPt));
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

// GameScene events

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mpress " << event->scenePos();
    QPoint worldPos(screenToWorld(event->scenePos().toPoint()));
    this->movePlayer(worldPos);

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

// LandscapeWidget

LandscapeWidget::LandscapeWidget(QWidget *parent)
    : QGraphicsView(parent)
    , zoom(1.0)
    , world(QPoint(80, 60))
{
    scene = new GameScene(world);
    scene->setMap(world.landscape);

    this->setScene(scene);
    this->setMouseTracking(true); // activate mouse tracking for landscapewidget,
                                  // otherwise scene does not get mouse events
                                  // without a button press
}

LandscapeWidget::~LandscapeWidget()
{
    delete scene;
}

void LandscapeWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Plus) {
        zoom = 0.9;
        this->scale(zoom, zoom);
    } else if (event->key() == Qt::Key_Minus) {
        zoom = 1.1;
        this->scale(zoom, zoom);
    }
}
