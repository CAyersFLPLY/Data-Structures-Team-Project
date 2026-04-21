#ifndef SPANNING_H
#define SPANNING_H

#include "Spanning.h"
#include <iostream>

// Undirected Edge Implemenatation.

UndirectedEdge::UndirectedEdge(int u, int v, double c) : u(u), v(v), cost(c) {}

// Disjoint Set Implementation.

DisjointSet::DisjointSet(int n){
    parent.resize(n);
    rank.resize(n, 0);
    // Initially, every node is its own parent (representing a set of size 1)
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

// Find with Path Compression for near O(1) lookups.
int DisjointSet::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]); 
    }
    return parent[x];
}

// Union by Rank to keep the tree shallow
void DisjointSet::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);

    if (rootX != rootY) {
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
}

// Helper Function: Custom QuickSort to avoid <algorithm> std::sort
void customEdgeSort(std::vector<UndirectedEdge>& edges, int left, int right) {
    if (left >= right) return;
    
    double pivot = edges[right].cost;
    int i = left - 1;
    
    for (int j = left; j < right; ++j) {
        if (edges[j].cost <= pivot) {
            i++;
            // Swap
            UndirectedEdge temp = edges[i];
            edges[i] = edges[j];
            edges[j] = temp;
        }
    }
    // Swap pivot into place
    UndirectedEdge temp = edges[i + 1];
    edges[i + 1] = edges[right];
    edges[right] = temp;
    
    int partitionIndex = i + 1;
    customEdgeSort(edges, left, partitionIndex - 1);
    customEdgeSort(edges, partitionIndex + 1, right);
}


// Task 6: Build Undirected Graph G_u based on cost rules
std::vector<UndirectedEdge> Spanning::buildUndirectedGraph(const Graph& g) {
    int V = g.getAirports().size(); 
    std::vector<UndirectedEdge> undirectedEdges;
    
    // Create a 2D matrix initialized to -1.0 to track the minimum cost between any u and v
    std::vector<std::vector<double>> edgeMatrix(V, std::vector<double>(V, -1.0));

    const auto& adjList = g.getAdj(); 

    for (int u = 0; u < V; ++u) {
        for (size_t i = 0; i < adjList[u].size(); ++i) {
            int v = adjList[u][i].toIndex; 
            double currentCost = adjList[u][i].cost;

            // If an edge already exists in the matrix, keep the minimum cost (Rule b)
            // Otherwise, store this cost (Rule a)
            if (edgeMatrix[u][v] == -1.0 || currentCost < edgeMatrix[u][v]) {
                edgeMatrix[u][v] = currentCost;
                edgeMatrix[v][u] = currentCost; // Ensure symmetry
            }
        }
    }

    // Extract the final undirected edges from the upper triangle of the matrix to avoid duplicates
    for (int u = 0; u < V; ++u) {
        for (int v = u + 1; v < V; ++v) {
            if (edgeMatrix[u][v] != -1.0) {
                undirectedEdges.push_back(UndirectedEdge(u, v, edgeMatrix[u][v]));
            }
        }
    }

    return undirectedEdges;
}


// Task 7: Prim's Algorithm
void Spanning::primMST(const Graph& g) {
    int V = g.getAirports().size();
    if (V == 0) return;
    const auto& airports = g.getAirports();

    std::vector<UndirectedEdge> edges = buildUndirectedGraph(g);

    // Build a quick adjacency matrix for O(V^2) Prim's
    std::vector<std::vector<double>> adjMatrix(V, std::vector<double>(V, -1.0));
    for (size_t i = 0; i < edges.size(); ++i) {
        adjMatrix[edges[i].u][edges[i].v] = edges[i].cost;
        adjMatrix[edges[i].v][edges[i].u] = edges[i].cost;
    }

    std::vector<bool> inMST(V, false);
    std::vector<double> minEdgeCost(V, 1e9); // 1e9 acts as infinity
    std::vector<int> parent(V, -1);
    
    std::vector<UndirectedEdge> mst;
    double totalCost = 0;

    // Loop through all nodes to ensure we catch disconnected forests
    for (int startNode = 0; startNode < V; ++startNode) {
        if (inMST[startNode]) continue;

        minEdgeCost[startNode] = 0;

        for (int count = 0; count < V; ++count) {
            int u = -1;
            double minVal = 1e9;

            // Find the unvisited vertex with the minimum edge cost
            for (int i = 0; i < V; ++i) {
                if (!inMST[i] && minEdgeCost[i] < minVal) {
                    minVal = minEdgeCost[i];
                    u = i;
                }
            }

            if (u == -1) break; // End of a connected component

            inMST[u] = true;

            // If it has a parent, it's a valid MST edge
            if (parent[u] != -1) {
                mst.push_back(UndirectedEdge(parent[u], u, adjMatrix[parent[u]][u]));
                totalCost += adjMatrix[parent[u]][u];
            }

            // Update adjacent vertices
            for (int v = 0; v < V; ++v) {
                if (adjMatrix[u][v] != -1.0 && !inMST[v] && adjMatrix[u][v] < minEdgeCost[v]) {
                    parent[v] = u;
                    minEdgeCost[v] = adjMatrix[u][v];
                }
            }
        }
    }

    if (mst.size() < V - 1) {
        std::cout << "Graph is disconnected. MST cannot be formed. Displaying Forest:\n";
    } else {
        std::cout << "Minimal Spanning Tree (Prim):\n";
    }
    
    std::cout << "Edge\tWeight\n";
    for(size_t i = 0; i < mst.size(); ++i) {
        std::cout << airports[mst[i].u].code << " - " << airports[mst[i].v].code << "\t" << mst[i].cost << "\n";
    }
    std::cout << "Total Cost of MST: " << totalCost << "\n\n";
}


// Task 8: Kruskal's Algorithm
void Spanning::kruskalMST(const Graph& g) {
    int V = g.getAirports().size();
    if (V == 0) return;
    const auto& airports = g.getAirports();

    std::vector<UndirectedEdge> edges = buildUndirectedGraph(g);

    // Sort edges by cost using custom QuickSort
    if (!edges.empty()) {
        customEdgeSort(edges, 0, edges.size() - 1);
    }

    DisjointSet ds(V);
    std::vector<UndirectedEdge> mst;
    double totalCost = 0;

    for (size_t i = 0; i < edges.size(); ++i) {
        int u = edges[i].u;
        int v = edges[i].v;

        // If including this edge does not cause a cycle, add it
        if (ds.find(u) != ds.find(v)) {
            ds.unite(u, v);
            mst.push_back(edges[i]);
            totalCost += edges[i].cost;
        }
    }

    if (mst.size() < V - 1) {
        std::cout << "Graph is disconnected. Displaying Minimum Spanning Forest (Kruskal):\n";
    } else {
        std::cout << "Minimal Spanning Tree (Kruskal):\n";
    }

    std::cout << "Edge\tWeight\n";
    for (size_t i = 0; i < mst.size(); ++i) {
        std::cout << airports[mst[i].u].code << " - " << airports[mst[i].v].code << "\t" << mst[i].cost << "\n";
    }
    std::cout << "Total Cost of MST: " << totalCost << "\n\n";
}

#endif