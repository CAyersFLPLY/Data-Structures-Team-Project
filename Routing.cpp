#include "Routing.h"
#include <limits>
#include <iostream>

using namespace std;

static const int INF = numeric_limits<int>::max();


// --------------------------
// MinHeap Helper Functions
// --------------------------

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        // pair = (distance, node)
        if (heap[index].first < heap[parent].first) {
            pair<int, int> temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

void MinHeap::heapifyDown(int index) {
    int size = heap.size();

    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && heap[left].first < heap[smallest].first) {
            smallest = left;
        }

        if (right < size && heap[right].first < heap[smallest].first) {
            smallest = right;
        }

        if (smallest != index) {
            pair<int, int> temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            index = smallest;
        } else {
            break;
        }
    }
}

void MinHeap::push(int dist, int node) {
    heap.push_back(make_pair(dist, node));
    heapifyUp(heap.size() - 1);
}

pair<int, int> MinHeap::pop() {
    pair<int, int> top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) {
        heapifyDown(0);
    }

    return top;
}

bool MinHeap::empty() const {
    return heap.empty();
}


// --------------------------
// Routing Helper Functions
// --------------------------

static vector<int> buildPath(const vector<int>& parent, int end) {
    vector<int> reversed;
    int current = end;

    while (current != -1) {
        reversed.push_back(current);
        current = parent[current];
    }

    vector<int> path;
    for (int i = reversed.size() - 1; i >= 0; i--) {
        path.push_back(reversed[i]);
    }

    return path;
}

static int calculatePathCost(const Graph& g, const vector<int>& path) {
    int totalCost = 0;

    for (int i = 0; i < (int)path.size() - 1; i++) {
        int u = path[i];
        int v = path[i + 1];

        const vector<Edge>& neighbors = g.getAdj()[u];
        for (int j = 0; j < (int)neighbors.size(); j++) {
            if (neighbors[j].toIndex == v) {
                totalCost += neighbors[j].cost;
                break;
            }
        }
    }

    return totalCost;
}


// --------------------------
// Task 2: Standard Dijkstra
// --------------------------

PathResult Routing::shortestPath(const Graph& g, int start, int end) {
    PathResult result;
    result.totalDistance = 0;
    result.totalCost = 0;
    result.found = false;

    int n = g.getNumAirports();

    vector<int> dist(n, INF);
    vector<int> parent(n, -1);

    MinHeap pq;
    dist[start] = 0;
    pq.push(0, start);

    while (!pq.empty()) {
        pair<int, int> current = pq.pop();
        int currentDist = current.first;
        int u = current.second;

        if (currentDist > dist[u]) {
            continue;
        }

        if (u == end) {
            break;
        }

        const vector<Edge>& neighbors = g.getAdj()[u];
        for (int i = 0; i < (int)neighbors.size(); i++) {
            int v = neighbors[i].toIndex;
            int weight = neighbors[i].distance;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(dist[v], v);
            }
        }
    }

    if (dist[end] == INF) {
        return result;
    }

    result.path = buildPath(parent, end);
    result.totalDistance = dist[end];
    result.totalCost = calculatePathCost(g, result.path);
    result.found = true;

    return result;
}


// --------------------------
// Task 3: All shortest paths
// to airports in a given state
// --------------------------

vector<PathResult> Routing::shortestPathsToState(const Graph& g, int start, const string& state) {
    vector<PathResult> results;
    int n = g.getNumAirports();

    vector<int> dist(n, INF);
    vector<int> parent(n, -1);

    MinHeap pq;
    dist[start] = 0;
    pq.push(0, start);

    // Run Dijkstra once from start
    while (!pq.empty()) {
        pair<int, int> current = pq.pop();
        int currentDist = current.first;
        int u = current.second;

        if (currentDist > dist[u]) {
            continue;
        }

        const vector<Edge>& neighbors = g.getAdj()[u];
        for (int i = 0; i < (int)neighbors.size(); i++) {
            int v = neighbors[i].toIndex;
            int weight = neighbors[i].distance;

            if (dist[u] != INF && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push(dist[v], v);
            }
        }
    }

    // Check all airports in requested state
    for (int i = 0; i < n; i++) {
        if (g.getStateOfAirport(i) == state && dist[i] != INF) {
            PathResult result;
            result.path = buildPath(parent, i);
            result.totalDistance = dist[i];
            result.totalCost = calculatePathCost(g, result.path);
            result.found = true;
            results.push_back(result);
        }
    }

    return results;
}


// --------------------------
// Task 4: Shortest path with
// exact number of stops
// --------------------------
//
// exactStops means number of
// intermediate airports
//
// edgesUsed = exactStops + 1
//

PathResult Routing::shortestPathWithStops(const Graph& g, int start, int end, int exactStops) {
    PathResult result;
    result.totalDistance = 0;
    result.totalCost = 0;
    result.found = false;

    int n = g.getNumAirports();
    int exactEdges = exactStops + 1;

    // best[node][edges] = shortest distance to reach node using exactly edges edges
    vector<vector<int>> best(n, vector<int>(exactEdges + 1, INF));
    vector<vector<int>> parent(n, vector<int>(exactEdges + 1, -1));

    best[start][0] = 0;

    for (int edgesUsed = 0; edgesUsed < exactEdges; edgesUsed++) {
        for (int u = 0; u < n; u++) {
            if (best[u][edgesUsed] == INF) {
                continue;
            }

            const vector<Edge>& neighbors = g.getAdj()[u];
            for (int i = 0; i < (int)neighbors.size(); i++) {
                int v = neighbors[i].toIndex;
                int weight = neighbors[i].distance;

                if (best[u][edgesUsed] + weight < best[v][edgesUsed + 1]) {
                    best[v][edgesUsed + 1] = best[u][edgesUsed] + weight;
                    parent[v][edgesUsed + 1] = u;
                }
            }
        }
    }

    if (best[end][exactEdges] == INF) {
        return result;
    }

    // rebuild path from parent[node][edgesUsed]
    vector<int> reversed;
    int current = end;
    int edgesUsed = exactEdges;

    while (current != -1 && edgesUsed >= 0) {
        reversed.push_back(current);
        current = parent[current][edgesUsed];
        edgesUsed--;
    }

    vector<int> path;
    for (int i = reversed.size() - 1; i >= 0; i--) {
        path.push_back(reversed[i]);
    }

    result.path = path;
    result.totalDistance = best[end][exactEdges];
    result.totalCost = calculatePathCost(g, result.path);
    result.found = true;

    return result;
}
