#include "landscapewidget.h"

#include <QGraphicsRectItem>
#include <QDebug>

#include "landscape.h"

LandscapeWidget::LandscapeWidget(QWidget *parent)
    : QGraphicsView(parent)
    , world(QPoint(60, 40))
{
    scene = new QGraphicsScene();

    //QPen outline(QColor("blue"));

    // populate scene with tiles
    const qreal tile_width = 20;
    const qreal tile_height = 20;
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

    qDebug() << "scene ready";
    this->setScene(scene);
}

LandscapeWidget::~LandscapeWidget()
{
    delete scene;
}
