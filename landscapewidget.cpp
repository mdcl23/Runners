#include "landscapewidget.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QMouseEvent>

#include "landscape.h"

const qreal LandscapeWidget::tile_width = 20;
const qreal LandscapeWidget::tile_height = 20;

LandscapeWidget::LandscapeWidget(QWidget *parent)
    : QGraphicsView(parent)
    , world(QPoint(60, 40))
{
    scene = new QGraphicsScene();

    //QPen outline(QColor("blue"));

    // populate scene with tiles
    for (int yi = 0; yi < world.landscape.height; yi++)
    {
        for (int xi = 0; xi < world.landscape.width; xi++)
        {
            scene->addRect(
                xi*tile_width, yi*tile_height,
                tile_width, tile_height,
                QPen(),
                world.landscape.getTile(xi, yi).brush());
        }
    }

    playerItem = scene->addEllipse(tile_width*10, tile_height*10, 20, 20, QColor(255, 255, 255), QColor(255, 255, 255));

    qDebug() << "scene ready";
    this->setScene(scene);
}

LandscapeWidget::~LandscapeWidget()
{
    delete scene;
}

QPoint LandscapeWidget::screenToWorld(QPoint screenPt) const {
    return QPoint{
        int(screenPt.x()/LandscapeWidget::tile_width),
        int(screenPt.y()/LandscapeWidget::tile_height) };
}

void LandscapeWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint worldPt = this->screenToWorld(event->pos());

    qDebug() << "mouse clicked: ("
             << worldPt.x() << ", "
             << worldPt.y() << ") = "
             << world.landscape.getTile(worldPt).type;

    playerItem->setPos(worldPt*tile_width);
}
