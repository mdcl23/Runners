#include "dijkstra.h"

#include <iostream>
#include <string>
#include <list>

#include <set>
#include <utility> // for pair
#include <algorithm>
#include <iterator>
#include <cmath>

#include "landscape.h"

void DijkstraComputePaths(vertex_t source,
                          const adjacency_list_t &adjacency_list,
                          std::vector<weight_t> &min_distance,
                          std::vector<vertex_t> &previous)
{
    int n = adjacency_list.size();

    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);

    std::set<std::pair<weight_t, vertex_t> > vertex_queue;

    vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor> &neighbors = adjacency_list[u];

        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;

            if (distance_through_u < min_distance[v])
            {
                vertex_queue.erase(std::make_pair(min_distance[v], v));
                min_distance[v] = distance_through_u;
                previous[v] = u;
                vertex_queue.insert(std::make_pair(min_distance[v], v));
            }
        }
    }
}

void print_neighbour(adjacency_list_t adjacency_list, int cnt)
{

    for(int i = 0; i < cnt; i++)
    {
        std :: cout << i << ": ";

        for(unsigned int j = 0; j < adjacency_list[i].size(); j++)
        {
            std :: cout << adjacency_list[i][j].target << " ";
        }

        std :: cout << std :: endl;

    }

}

bool isDiagonal(int i, int j)
{
    return (i == 0 || j == 0);
}

adjacency_list_t make_graph(Landscape landscape)
{

    int cnt = 0;

    adjacency_list_t adjacency_list(landscape.width*landscape.height);

    for(int i = 0; i < landscape.height; i++)
    {
        for(int j = 0; j < landscape.width; j++)
        {
            if (landscape.getTile(j, i).walkSpeed() == 0) {
                cnt++;
                continue;
            }

            if(i == 0)
            {
                /////////////////////////////////
                for(int i1 = 0; i1 < 2; i1++)
                {

                    for(int j1 = -1; j1 < 2; j1++)
                    {
                        if( (j == 0 && j1 == -1) || (j == landscape.width-1 && j1 == 1) )
                        {
                            continue;
                        }

                        double weight = 1;
                        if (isDiagonal(i1, j1)) {
                            weight = 1./sqrt(2.);
                        }

                        if( !( (i1 == 0) && (j1 == 0) ) )
                        {
                            if (landscape.getTile(j+j1, i+i1).walkSpeed() != 0) {
                                adjacency_list[cnt].push_back(neighbor(cnt + j1 + landscape.width*i1, weight));
                            }
                        }
                    }
                }
                //////////////////////////////////
            }
            else if(i == landscape.height - 1)
            {
                /////////////////////////////////
                for(int i1 = -1; i1 < 1; i1++)
                {
                    for(int j1 = -1; j1 < 2; j1++)
                    {
                        double weight = 1;
                        if (isDiagonal(i1, j1)) {
                            weight = 1./sqrt(2.);
                        }

                        if( (j == 0 && j1 == -1) || (j == landscape.width-1 && j1 == 1) )
                        {
                            continue;
                        }
                        if( !( (i1 == 0) && (j1 == 0) ) )
                        {
                            if ( landscape.getTile(j+j1, i+i1).walkSpeed() != 0 )
                            {
                                adjacency_list[cnt].push_back(neighbor(cnt + j1 + landscape.width*i1, weight));
                            }
                        }
                    }
                }
                //////////////////////////////////
            }
            else
            {
                /////////////////////////////////
                for(int i1 = -1; i1 < 2; i1++)
                {
                    for(int j1 = -1; j1 < 2; j1++)
                    {
                        double weight = 1;
                        if (isDiagonal(i1, j1)) {
                            weight = 1./sqrt(2.);
                        }

                        if( (j == 0 && j1 == -1) || (j == landscape.width-1 && j1 == 1) )
                        {
                            continue;
                        }

                        if( !( (i1 == 0) && (j1 == 0) ) )
                        {
                            if ( landscape.getTile(j+j1, i+i1).walkSpeed() != 0)
                            {
                                adjacency_list[cnt].push_back(neighbor(cnt + j1 + landscape.width*i1, weight));
                            }
                        }
                    }
                }
                //////////////////////////////////

            }

        cnt++;

        }

    }

    //print_neighbour(adjacency_list, cnt);

    return adjacency_list;

}

std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t> &previous)
{
    std::list<vertex_t> path;
    for ( ; vertex != -1; vertex = previous[vertex])
    {
        path.push_front(vertex);
    }

    return path;
}

void test_dijkstra()
{
    // remember to insert edges both ways for an undirected graph
    Landscape landscape(5, 5);

    for (int yi = 0; yi < landscape.height; yi++) {
        for (int xi = 0; xi < landscape.width; xi++) {
            QPoint coords = QPoint(xi, yi);

            if (xi == 2) {
                landscape.setTile(coords, Tile::WATER_TILE);
            } else {
                landscape.setTile(coords, Tile::GRASS_TILE);
            }

            std::cout << "+" << landscape.getTile(coords).walkSpeed() << std::endl;
        }
    }
    adjacency_list_t adjacency_list = make_graph(landscape);

    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;

    QPoint start(0, 0);
    QPoint end(4, 0);

    DijkstraComputePaths(landscape.coordsToIndex(start),
                         adjacency_list, min_distance, previous);

    std::cout << "Distance from 0,0 to 11,5: " << min_distance[landscape.coordsToIndex(end)] << std::endl;
    std::list<vertex_t> path = DijkstraGetShortestPathTo(landscape.coordsToIndex(end), previous);
    std::cout << "Path : ";
    std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
    std::cout << std::endl;
}
