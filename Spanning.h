#include <Graph.h>
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
    // Task 6
    //static std::vector<UndirectedEdge> buildUndirectedGraph(const Graph& g);

    // Task 7 (Prim)
    //static void primMST(const Graph& g);

    // Task 8 (Kruskal)
    //static void kruskalMST(const Graph& g);
};