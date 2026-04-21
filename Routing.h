#ifndef ROUTING_H
#define ROUTING_H

#include <vector>
#include <string>
#include <utility>
#include "CoreGraph.h"

struct PathResult {
    std::vector<int> path;
    int totalDistance;
    int totalCost;
    bool found;
};

class MinHeap {
private:
    std::vector<std::pair<int, int>> heap; // (distance, node)

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    void push(int dist, int node);
    std::pair<int, int> pop();
    bool empty() const;
};

class Routing {
public:
    // Task 2
    static PathResult shortestPath(
        const Graph& g, int start, int end);

    // Task 3
    static std::vector<PathResult> shortestPathsToState(
        const Graph& g, int start, const std::string& state);

    // Task 4
    static PathResult shortestPathWithStops(
        const Graph& g, int start, int end, int exactStops);
};

#endif