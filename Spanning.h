#ifndef SPANNING_H
#define SPANNING_H

#include "CoreGraph.h"
#include <vector>

class UndirectedEdge {
public:
    int u, v;
    double cost;

    UndirectedEdge(int u, int v, double c);
};

class DisjointSet {
private:
    std::vector<int> parent, rank;

public:
    DisjointSet(int n);

    int find(int x);
    void unite(int x, int y);
};

class Spanning {
public:
    static std::vector<UndirectedEdge> buildUndirectedGraph(const Graph& g);
    static void primMST(const Graph& g);
    static void kruskalMST(const Graph& g);
};

#endif
