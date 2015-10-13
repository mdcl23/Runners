#include "world.h"

#include <QDebug>

World::World(QPoint size)
    : landscape(Landscape::createRandomLandscape(size.x(), size.y()))
    , playerPosition(10,20)
    , hasCrossedBridge(false)
{
    landscape.setTile(playerPosition, Tile::GRASS_TILE);
}

World::~World()
{}

void World::movePlayer(QPoint destination)
{
    playerPath = landscape.getPath(playerPosition, destination);
    if (playerPath.empty()) return;
    emit pathCreated(playerPath);
}

void World::movementStopped(QPoint newPosition)
{
    playerPosition = newPosition;
    if (playerPath.empty()) return;

    QPoint next = playerPath.front();
    playerPath.pop_front();
    emit addWaypoint(next);
}
