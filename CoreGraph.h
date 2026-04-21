#ifndef COREGRAPH_H
#define COREGRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

// Fundamental Structs
struct Edge {
    int toIndex;
    double distance;
    double cost;

    Edge(int to, double dist, double cst) : toIndex(to), distance(dist), cost(cst) {}
};

struct Airport {
    std::string code;  // e.g., "ATL"
    std::string city;  // e.g., "Atlanta"
    std::string state; // e.g., "GA"
    int inboundCount = 0;
    int outboundCount = 0;

    Airport(std::string c, std::string ct, std::string st) 
        : code(c), city(ct), state(st) {}
};

class Graph {
private:
    std::vector<Airport> airports;
    std::vector<std::vector<Edge>> adj;

    // Helper to find airport index by its code (e.g., "PIT")
    int getAirportIndex(const std::string& code) {
        for (int i = 0; i < airports.size(); ++i) {
            if (airports[i].code == code) return i;
        }
        return -1;
    }

public:
    /**
     * Task 1 & 5: CSV Parser and Graph Construction
     * Reads airports.csv to build the weighted directed graph.
     */
    bool loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line, origin, dest, originCity, destCity, distStr, costStr;
        // Skip header
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            
            // Expected Format: Origin_airport, Destination_airport, Origin_city, Destination_city, Distance, Cost
            std::getline(ss, origin, ',');
            std::getline(ss, dest, ',');
            std::getline(ss, originCity, ',');
            std::getline(ss, destCity, ',');
            std::getline(ss, distStr, ',');
            std::getline(ss, costStr, ',');

            // Add unique airports to the list
            int u = getAirportIndex(origin);
            if (u == -1) {
                airports.push_back(Airport(origin, originCity, "")); // State logic can be parsed from City string
                adj.push_back(std::vector<Edge>());
                u = airports.size() - 1;
            }

            int v = getAirportIndex(dest);
            if (v == -1) {
                airports.push_back(Airport(dest, destCity, ""));
                adj.push_back(std::vector<Edge>());
                v = airports.size() - 1;
            }

            // Create weighted directed edge [cite: 37, 38]
            double distance = std::stod(distStr);
            double cost = std::stod(costStr);
            adj[u].push_back(Edge(v, distance, cost));

            // Track connections for Task 5 [cite: 47]
            airports[u].outboundCount++;
            airports[v].inboundCount++;
        }
        file.close();
        return true;
    }

    /**
     * Task 5: Display sorted connection counts
     */
    void displayConnections() {
        // Create a copy for sorting as per project requirements [cite: 48]
        std::vector<int> indices;
        for(int i = 0; i < airports.size(); ++i) indices.push_back(i);

        // Sorting based on total (inbound + outbound) [cite: 48]
        std::sort(indices.begin(), indices.end(), [&](int a, int b) {
            return (airports[a].inboundCount + airports[a].outboundCount) > 
                   (airports[b].inboundCount + airports[b].outboundCount);
        });

        std::cout << "Airport\tConnections\n";
        for (int idx : indices) {
            int total = airports[idx].inboundCount + airports[idx].outboundCount;
            std::cout << airports[idx].code << "\t" << total << "\n";
        }
    }
};

#endif
