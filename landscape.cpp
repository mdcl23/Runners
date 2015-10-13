#include "landscape.h"

#include <cmath>
#include <QTime>
#include <QStack>

#include <iostream>

#include "dijkstra.h"

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

Tile Tile::createRandom(const QMap<Tile::Type, double>& probDist)
{
    double probSum = 0.0;
    auto i = probDist.constBegin();
    for (; i != probDist.constEnd(); i++) {
        probSum += i.value();
    }

    // randomize a value and compare it to a cumulative random distribution.
    double rnd = probSum * double(qrand())/RAND_MAX;
    double probAccum = 0.0;
    for (i = probDist.constBegin(); i != probDist.constEnd(); i++) {
        probAccum += i.value();
        if (rnd < probAccum) {
            return i.key(); // returns the randomized tile.
        }
    }

    // actually this should never happen as
    // the assertion rnd < probSum should always be true
    return Tile::NONE_TILE;
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
    case GRASS_TILE: return QColor(24, 140, 24);
    case SAND_TILE: return QColor(142, 136, 0);
    case ROAD_TILE: return QBrush(QColor("black"), Qt::DiagCrossPattern);
    case WATER_TILE: return QColor(64, 64, 214);
    case BRIDGE_TILE: return QColor(146, 96, 0);
    case ROCK_TILE: return QColor(100, 110, 112);
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
    if (x >= 0 && y >= 0 && x < width && y < height) {
        return this->tiles[y*width + x];
    }
    return Tile::NONE_TILE;
}

void Landscape::setTile(const QPoint &p, Tile tile)
{
    this->setTile(p.x(), p.y(), tile);
}

void Landscape::setTile(int x, int y, Tile tile)
{
    if (x >= 0 && y >= 0 && x < width && y < height) {
        this->tiles[y*width + x] = tile;
    }
}

QVector<Tile> Landscape::getNeighbours(int x, int y) const
{
    return this->getNeighbours(QPoint(x, y));
}

QVector<Tile> Landscape::getNeighbours(const QPoint& p) const
{
    QPoint neighbour_pts[8] =
    {
        QPoint(-1, -1), QPoint(0, -1), QPoint(1, -1),
        QPoint(-1, 0), QPoint(1, 0),
        QPoint(-1, 1), QPoint(0, 1), QPoint(1, 1)
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

int Landscape::coordsToIndex(QPoint coords) const{
    return coords.y()*width + coords.x();
}

QPoint Landscape::indexToCoords(int offs) const {
    int yi = offs / width;
    int xi = offs - yi*width;
    return QPoint(xi, yi);
}

QVector<QPoint> Landscape::getPath(QPoint start, QPoint end) const
{
    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;

    adjacency_list_t adj = make_graph(*this);
    DijkstraComputePaths(this->coordsToIndex(start), adj, min_distance, previous);
    std::list<vertex_t> path = DijkstraGetShortestPathTo(this->coordsToIndex(end), previous);

    if (min_distance[this->coordsToIndex(end)] > 2000000) {
        return QVector<QPoint>();
    }

    QVector<QPoint> retPath;
    foreach(vertex_t vx, path) {
        retPath.append(this->indexToCoords(vx));
    }
    return retPath;
}

namespace {
    int countTiles(QVector<Tile> tiles, Tile::Type ttype) {
        int count = 0;
        foreach (Tile t, tiles) {
            if (t.type == ttype) {
                count++;
            }
        }
        return count;
    }

    QPoint randomDirection(QPoint c)
    {
        QVector<QPoint> dirs;
        dirs.append(QPoint(-1, -1));
        dirs.append(QPoint( 0, -1));
        dirs.append(QPoint( 1, -1));
        dirs.append(QPoint( 1,  0));
        dirs.append(QPoint(-1,  1));
        dirs.append(QPoint( 0,  1));
        dirs.append(QPoint( 1,  1));
        dirs.append(QPoint(-1,  0));

        return c + dirs[qrand() % 8];
    }
}

Landscape Landscape::createRandomLandscape(int width, int height)
{
    qsrand(QTime::currentTime().msec());

    Landscape landscape(width, height);
    for (int yi = 0; yi < height; yi++) {
        for (int xi = 0; xi < width; xi++) {
            landscape.setTile(xi, yi, Tile::GRASS_TILE);
        }
    }

    int riverPosition = width/5 + qrand() % ((2*width)/5);
    int riverAmplitude = 4 + qrand() % (width/20);
    double riverFreq = .1 + 1.2*double(qrand() % 1000)/1000.;
    double riverPhase = 2*3.14*double(qrand() % 1000)/1000.;
    int riverWidth = 2 + qrand() % 20;

    int bridge1Pos = qrand() % (height/2);
    int bridge2Pos = height - qrand() % (height/2);

    QMap<Tile::Type, double> landscapeDist;
    landscapeDist[Tile::GRASS_TILE] = 0.85;
    //landscapeDist[Tile::ROAD_TILE] = 0.05;
    landscapeDist[Tile::ROCK_TILE] = 0.3;
    landscapeDist[Tile::SAND_TILE] = 0.05;
    landscapeDist[Tile::WATER_TILE] = 0.05;

    for (int yi = 0; yi < height; yi++) {
        for (int xi = 0; xi < width; xi++) {
            Tile t = Tile::createRandom(landscapeDist);
            landscape.setTile(xi, yi, t);
        }
    }

    QVector<QPoint> visited;

    // smooth water
    for (int yi = 0; yi < height; yi++) {
        for (int xi = 0; xi < width; xi++) {
            if (visited.contains(QPoint(xi, yi))) continue;

            Tile current = landscape.getTile(xi, yi);

            auto ns = landscape.getNeighbours(xi, yi);
            int waterNeighbourCount = countTiles(ns, Tile::WATER_TILE);

            if (current.type == Tile::WATER_TILE) {
                if (qrand() % 100 > (1+waterNeighbourCount*3))
                {
                    int amount = (1 + qrand() % 3)*(1+waterNeighbourCount);

                    QStack<QPoint> cursors;
                    cursors.push(QPoint(xi, yi));

                    while (amount > 0)
                    {
                        if (cursors.empty()) break;

                        QPoint c = cursors.pop();
                        auto c1 = randomDirection(c);
                        auto c2 = randomDirection(c);

                        cursors.push(c1);
                        landscape.setTile(c1, Tile::WATER_TILE);
                        visited.push_back(c1);

                        cursors.push(c2);
                        landscape.setTile(c2, Tile::WATER_TILE);
                        visited.push_back(c2);

                        amount--;
                    }
                }
                else
                {
                    landscape.setTile(xi, yi, Tile::GRASS_TILE);
                }
            }
        }
    }

    // smooth rocks and mountains and water
    for(int yi = 0; yi < height; yi++) {
        for (int xi = 0; xi < width; xi++) {
            Tile current = landscape.getTile(xi, yi);
            int neighbour_rocks = 0;
            auto ns = landscape.getNeighbours(xi, yi);
            for (int n = 0; n < 8; n++) {
                neighbour_rocks += int(ns[n].type == Tile::ROCK_TILE);
            }

            if (neighbour_rocks == 0 &&
                current.type == Tile::SAND_TILE)
            {
                int neighbour_sands = 0;
                for (int n = 0; n < 8; n++) {
                    neighbour_sands += int(ns[n].type == Tile::SAND_TILE);
                }
                if (neighbour_sands == 0) {
                    landscape.setTile(xi, yi, Tile::GRASS_TILE);
                }
            }

            if (neighbour_rocks == 0 &&
                current.type != Tile::ROCK_TILE)
            {
                continue;
            }

            double rnd = double(qrand()%100)/100.;
            double cumRndF = 1./(1 + exp(-neighbour_rocks+4)*0.6);
            if (rnd < cumRndF) {
                landscape.setTile(xi, yi, Tile::ROCK_TILE);
            } else if (current.type == Tile::ROCK_TILE) {
                landscape.setTile(xi, yi, Tile::SAND_TILE);
            }
       }
    }

    // create river and the bridges

    for (int yi=0; yi < height; yi++) {
        int rp = riverPosition + riverAmplitude*sin(riverPhase + 2*3.14*riverFreq*double(yi)/height);
        if (yi == bridge1Pos || yi == bridge2Pos) {
            for (int xi = 0; xi < riverWidth+2; xi++) {
                landscape.setTile(xi-1+rp, yi, Tile::BRIDGE_TILE);
            }
        } else {
            for (int xi = 0; xi < riverWidth; xi++) {
                landscape.setTile(xi+rp, yi, Tile::WATER_TILE);
            }
        }
    }

    return landscape;
}
