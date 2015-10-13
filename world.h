#ifndef WORLD_H
#define WORLD_H

#include <QObject>

#include "landscape.h"
#include <QPoint>

class World: public QObject
{
    Q_OBJECT

public:
    World(QPoint size);
    ~World();

    Landscape landscape;
    QPoint playerPosition;

    QVector<QPoint> playerPath;

    bool hasCrossedBridge;

signals:
    void addWaypoint(QPoint coords);
    void pathCreated(QVector<QPoint>);

public slots:
    void movePlayer(QPoint destination);
    void movementStopped(QPoint stopPosition);
};

#endif // WORLD_H
