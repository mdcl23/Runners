#include "landscapewidget.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QKeyEvent>

#include "landscape.h"
#include "gamescene.h"

#include "hexscene.h"

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

    connect(&world, SIGNAL(pathCreated(QVector<QPoint>)), scene, SLOT(showPath(QVector<QPoint>)));
    connect(scene, SIGNAL(tileClicked(QPoint)), &world, SLOT(movePlayer(QPoint)));
    connect(scene, SIGNAL(playerArrivedAt(QPoint)), &world, SLOT(movementStopped(QPoint)));

    connect(scene, SIGNAL(playerArrivedAt(QPoint)), this, SLOT(checkForBridge(QPoint)));
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

namespace {

static QWidget* hexWidgetPtr;

void createHexWidget(QWidget* parent)
{
    QWidget* bgWidget = new QWidget(parent);
    bgWidget->setStyleSheet("background-color: rgba(128, 128, 128, 75%)");
    bgWidget->setGeometry(
        0, 0,
        parent->width(), parent->height()
    );
    bgWidget->show();

    QGraphicsView* hexWidget = new QGraphicsView(bgWidget);
    hexWidget->setObjectName("hexgame");
    hexWidget->setScene(new HexScene(10));
    hexWidget->show();
    hexWidget->move(
        (bgWidget->width() - hexWidget->width())/2,
        (bgWidget->height() - hexWidget->height())/2);


    hexWidgetPtr = bgWidget;

    QObject::connect(hexWidget->scene(), SIGNAL(playerWins()), parent, SLOT(playerWonHex()));
    QObject::connect(hexWidget->scene(), SIGNAL(computerWins()), parent, SLOT(computerWonHex()));
}

void destroyHexWidget()
{
    QGraphicsView* hexW = hexWidgetPtr->findChild<QGraphicsView*>("hexgame");
    HexScene* hs = dynamic_cast<HexScene*>(hexW->scene());

    QObject::disconnect(hs, SIGNAL(playerWins()), hexWidgetPtr->parent(), SLOT(playerWonHex()));
    QObject::disconnect(hs, SIGNAL(computerWins()), hexWidgetPtr->parent(), SLOT(computerWonHex()));

    hexW->setScene(0);
  //  delete hs;

    hexWidgetPtr->hide();
    //delete hexWidgetPtr;
    //hexWidgetPtr = 0;
}

}

void LandscapeWidget::checkForBridge(QPoint worldPt)
{
    if (world.hasCrossedBridge == false &&
        world.landscape.getTile(worldPt).type == Tile::BRIDGE_TILE)
    {
        scene->clearPath();
        createHexWidget(this);
    }
}

void LandscapeWidget::playerWonHex()
{
    qDebug() << "YOU WIN";

    destroyHexWidget();
    world.hasCrossedBridge = true;
}

void LandscapeWidget::computerWonHex()
{
    qDebug() << "computer WIN";
    destroyHexWidget();

    for (int yi = -1; yi < 2; yi++)
    {
        for (int xi = -1; xi < 2; xi++)
        {
            Tile tile = world.landscape.getTile(world.playerPosition + QPoint(xi, yi));
            if (tile.walkSpeed() != 0 && tile.type != Tile::BRIDGE_TILE)
            {
                world.playerPosition += QPoint(xi, yi);
                scene->player->setPos(scene->worldToScreen(world.playerPosition));
            }
        }
    }
}
