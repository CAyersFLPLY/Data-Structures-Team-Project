#ifndef COREGRAPH_H
#define COREGRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

// Fundamental Structs [cite: 19, 20]
struct Edge {
    int toIndex;
    double distance;
    double cost;

    Edge(int to, double dist, double cst) : toIndex(to), distance(dist), cost(cst) {}
};

struct Airport {
    std::string code;  // e.g., "ATL"
    std::string city;  // e.g., "Atlanta"
    std::string state; // e.g., "GA" [cite: 67]
    int inboundCount = 0;
    int outboundCount = 0;

    Airport(std::string c, std::string ct, std::string st) 
        : code(c), city(ct), state(st) {}
};

class Graph {
private:
    std::vector<Airport> airports;
    std::vector<std::vector<Edge>> adj; // Adjacency list representation [cite: 20]

    // Private Helpers
    int getAirportIndex(const std::string& code);
    std::string extractState(const std::string& cityStr);

public:
    // Task 1 & 5: CSV Parser and Graph Construction [cite: 37, 47]
    bool loadFromCSV(const std::string& filename);

    // Task 5: Display sorted connection counts [cite: 48]
    void displayConnections();

    // Getters for external algorithms (Dijkstra, MST, etc.)
    const std::vector<Airport>& getAirports() const { return airports; }
    const std::vector<std::vector<Edge>>& getAdjList() const { return adj; }
};

#endif