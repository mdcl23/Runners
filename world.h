#ifndef WORLD_H
#define WORLD_H

#include <QObject>

#include "landscape.h"
#include <QPoint>

class World
{
public:
    World(QPoint size);

    Landscape landscape;
    QPoint playerPosition;

signals:

public slots:
};

#endif // WORLD_H
