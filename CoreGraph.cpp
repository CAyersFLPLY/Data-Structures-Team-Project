#include "CoreGraph.h"

/**
 * Task 1: Load graph from CSV [cite: 37, 65]
 * Reads airports.csv to build the weighted directed graph.
 */
bool Graph::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    // Skip the header row [cite: 65]
    if (!std::getline(file, line)) return false;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string origin, dest, originCity, destCity, distStr, costStr;

        // Tokenize the CSV columns [cite: 65]
        std::getline(ss, origin, ',');
        std::getline(ss, dest, ',');
        std::getline(ss, originCity, ',');
        std::getline(ss, destCity, ',');
        std::getline(ss, distStr, ',');
        std::getline(ss, costStr, ',');

        // Handle Origin Airport: extract state for Task 3 logic [cite: 42, 67]
        int u = getAirportIndex(origin);
        if (u == -1) {
            airports.push_back(Airport(origin, originCity, extractState(originCity)));
            adj.push_back(std::vector<Edge>());
            u = airports.size() - 1;
        }

        // Handle Destination Airport
        int v = getAirportIndex(dest);
        if (v == -1) {
            airports.push_back(Airport(dest, destCity, extractState(destCity)));
            adj.push_back(std::vector<Edge>());
            v = airports.size() - 1;
        }

        // Task 1: Add edge with two weights: Distance and Cost [cite: 37, 38]
        try {
            double distance = std::stod(distStr);
            double cost = std::stod(costStr);
            adj[u].push_back(Edge(v, distance, cost));

            // Task 5: Track inbound/outbound for connection counts [cite: 47, 48]
            airports[u].outboundCount++;
            airports[v].inboundCount++;
        } catch (...) {
            continue; 
        }
    }
    file.close();
    return true;
}

/**
 * Task 5: Display total direct connections sorted by highest count [cite: 48, 95]
 */
void Graph::displayConnections() {
    std::vector<int> indices;
    for (int i = 0; i < (int)airports.size(); ++i) {
        indices.push_back(i);
    }

    // Sort based on total direct connections (inbound + outbound) [cite: 48]
    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        int totalA = airports[a].inboundCount + airports[a].outboundCount;
        int totalB = airports[b].inboundCount + airports[b].outboundCount;
        return totalA > totalB;
    });

    std::cout << "\"Airport\t\",\"Connections\"" << std::endl; // [cite: 95]
    for (int idx : indices) {
        int total = airports[idx].inboundCount + airports[idx].outboundCount;
        std::cout << "\"" << airports[idx].code << "\t\",\"" << total << "\"" << std::endl;
    }
}

/**
 * Helper: Find airport index by its code (e.g., "PIT")
 */
int Graph::getAirportIndex(const std::string& code) {
    for (int i = 0; i < (int)airports.size(); ++i) {
        if (airports[i].code == code) return i;
    }
    return -1;
}

/**
 * Helper: Extracts the state abbreviation from city string [cite: 67]
 */
std::string Graph::extractState(const std::string& cityStr) {
    size_t lastSpace = cityStr.find_last_of(' ');
    if (lastSpace != std::string::npos) {
        return cityStr.substr(lastSpace + 1);
    }
    return "";
}