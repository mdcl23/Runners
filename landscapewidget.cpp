#include "landscapewidget.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QKeyEvent>

#include "landscape.h"
#include "gamescene.h"

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
