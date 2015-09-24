#include "world.h"

World::World(QPoint size)
    : landscape(Landscape::createRandomLandscape(size.x(), size.y()))
    , playerPosition(0,0)
{

}

