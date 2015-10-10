#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <QVector>
#include <QBrush>
#include <QMap>

class Tile {
public:
    enum Type {
        NONE_TILE = 0,
        GRASS_TILE,
        SAND_TILE,
        ROAD_TILE,
        WATER_TILE,
        BRIDGE_TILE,
        ROCK_TILE,
    };
    static const int TYPE_COUNT = 7; // number of tile types

    Tile();
    Tile(Type type);
    Tile(const Tile& tile);

    Type type;

    // createRandom() can be used to create a random
    // tile with uniform probability for all tiles.
    static Tile createRandom();

    // this randomizes a tile with a given distribution
    static Tile createRandom(const QMap<Tile::Type, double>& probDist);


    // walkSpeed() returns how fast can a game entity
    // walk on the tile.
    // 0 means the tile cannot be passed.
    int walkSpeed() const;

    // how the tile will be presented in the landscapeview
    QBrush brush() const;
};

class Landscape {
public:
    const int width;
    const int height;
    QVector<Tile> tiles;

    Landscape(int width, int height);

    Tile getTile(const QPoint& p) const;
    Tile getTile(int x, int y) const;

    void setTile(const QPoint& p, Tile tile);
    void setTile(int x, int y, Tile tile);

    // returns neighbours in clockwise order
    QVector<Tile> getNeighbours(const QPoint& p) const;
    QVector<Tile> getNeighbours(int x, int y) const;

    QPoint indexToCoords(int offs) const;
    int coordsToIndex(QPoint coords) const;

    QVector<QPoint> getPath(QPoint start, QPoint end) const;

    static Landscape createRandomLandscape(int width, int height);
};

#endif // LANDSCAPE_H

