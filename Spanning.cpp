#include <Spanning.h>
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