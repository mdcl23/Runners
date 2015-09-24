#include "landscape.h"

// Tile class implementation

Tile::Tile() : type(GRASS_TILE) {}

Tile::Tile(Type type)
    : type(type)
{}

Tile::Tile(const Tile& tile)
    : type(tile.type)
{}

Tile Tile::createRandom()
{
    return static_cast<Tile::Type>(qrand() % Tile::TYPE_COUNT);
}

int Tile::walkSpeed() const
{
    switch(type) {
    case GRASS_TILE:
    case BRIDGE_TILE:
        return 2;
    case SAND_TILE:
        return 1;
    case ROAD_TILE:
        return 3;
    case NONE_TILE:
    case WATER_TILE:
    case ROCK_TILE:
        return 0;
    }
}

QBrush Tile::brush() const
{
    switch (type)
    {
    case NONE_TILE: return QColor(0,0,0);
    case GRASS_TILE: return QColor("darkGreen");
    case SAND_TILE: return QColor("darkYellow");
    case ROAD_TILE: return QBrush(QColor("black "), Qt::DiagCrossPattern);
    case WATER_TILE: return QColor("darkBlue");
    case BRIDGE_TILE: return QColor("yellow");
    case ROCK_TILE: return QColor(56, 56, 56);
    }

}

// Landscape class implementation

Landscape::Landscape(int width, int height)
    : width(width)
    , height(height)
{
    tiles.resize(width*height);
}

Tile Landscape::getTile(const QPoint &p) const
{
    return this->getTile(p.x(), p.y());
}

Tile Landscape::getTile(int x, int y) const
{
    return this->tiles[y*width + x];
}

void Landscape::setTile(const QPoint &p, Tile tile)
{
    this->setTile(p.x(), p.y(), tile);
}

void Landscape::setTile(int x, int y, Tile tile)
{
    this->tiles[y*width + x] = tile;
}

QVector<Tile> Landscape::getNeighbours(int x, int y) const
{
    return this->getNeighbours(QPoint(x, y));
}

QVector<Tile> Landscape::getNeighbours(const QPoint& p) const
{
    QPoint neighbour_pts[8]{
        {-1, -1}, {0, -1}, {1, -1},
        {-1, 0}, {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };

    QVector<Tile> ns;
    for (int i = 0; i < 8; i++)
    {
        auto new_point = p + neighbour_pts[i];

        if (new_point.x() < 0 || new_point.y() < 0 ||
            new_point.x() >= this->width || new_point.y() >= this->height)
        {
            ns.append(Tile::NONE_TILE);
        }
        else
        {
            ns.append(this->getTile(new_point));
        }
    }
    return ns;
}

Landscape Landscape::createRandomLandscape(int width, int height)
{
    Landscape landscape(width, height);
    for (int yi = 0; yi < height; yi++) {
        for (int xi = 0; xi < width; xi++) {
            landscape.setTile(xi, yi, Tile::createRandom());
        }
    }
    return landscape;
}
