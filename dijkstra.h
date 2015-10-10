#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <limits> // for numeric_limits

#include "landscape.h"

typedef int vertex_t;
typedef double weight_t;

const weight_t max_weight = std::numeric_limits<double>::infinity();

struct neighbor
{
    vertex_t target;
    weight_t weight;

    // constructor
    neighbor(vertex_t arg_target, weight_t arg_weight)
        : target(arg_target), weight(arg_weight) { }
};

typedef std::vector<std::vector<neighbor> > adjacency_list_t;

void DijkstraComputePaths(vertex_t source,
                          const adjacency_list_t &adjacency_list,
                          std::vector<weight_t> &min_distance,
                          std::vector<vertex_t> &previous);

adjacency_list_t make_graph(Landscape landscape);

std::list<vertex_t> DijkstraGetShortestPathTo(
    vertex_t vertex, const std::vector<vertex_t> &previous);

void test_dijkstra();

#endif // DIJKSTRA_H

