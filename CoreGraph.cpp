#include "CoreGraph.h"
#include <iomanip>

/**
 * Task 1: Load data from airports.csv [cite: 37, 65]
 * Constructs a weighted directed graph where airports are nodes and flights are edges[cite: 27, 30].
 */
bool Graph::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    std::string line;
    // Skip the header row: Origin_airport, Destination_airport, Origin_city, Destination_city, Distance, Cost [cite: 65]
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string origin, dest, originCity, destCity, distStr, costStr;

        // Parse CSV columns [cite: 65, 67]
        std::getline(ss, origin, ',');
        std::getline(ss, dest, ',');
        std::getline(ss, originCity, ',');
        std::getline(ss, destCity, ',');
        std::getline(ss, distStr, ',');
        std::getline(ss, costStr, ',');

        // Helper to add or find airport and return its index
        int u = addOrFindAirport(origin, originCity);
        int v = addOrFindAirport(dest, destCity);

        // Convert strings to double for weights [cite: 38]
        double distance = std::stod(distStr);
        double cost = std::stod(costStr);

        // Add directed edge to adjacency list [cite: 30, 37]
        adj[u].push_back(Edge(v, distance, cost));

        // Increment counts for Task 5 [cite: 47]
        airports[u].outboundCount++;
        airports[v].inboundCount++;
    }

    file.close();
    return true;
}

/**
 * Helper: Ensures each unique airport is added to the graph only once.
 */
int Graph::addOrFindAirport(const std::string& code, const std::string& cityFull) {
    int existingIdx = getAirportIndex(code);
    if (existingIdx != -1) return existingIdx;

    // Split "City, State" from the dataset 
    size_t commaPos = cityFull.find(',');
    std::string city = (commaPos != std::string::npos) ? cityFull.substr(0, commaPos) : cityFull;
    std::string state = (commaPos != std::string::npos) ? cityFull.substr(commaPos + 2) : "";

    airports.push_back(Airport(code, city, state));
    adj.push_back(std::vector<Edge>()); // Add empty list for new node [cite: 20]
    return airports.size() - 1;
}

/**
 * Task 5: Count and display total direct flight connections[cite: 47].
 * List is sorted by total connections (Inbound + Outbound) in descending order.
 */
void Graph::displayConnections() {
    // Create an index vector for sorting to keep original airport order intact
    std::vector<int> sortedIndices;
    for (int i = 0; i < (int)airports.size(); ++i) {
        sortedIndices.push_back(i);
    }

    // Sort based on total connections (highest first) 
    std::sort(sortedIndices.begin(), sortedIndices.end(), [this](int a, int b) {
        int totalA = airports[a].inboundCount + airports[a].outboundCount;
        int totalB = airports[b].inboundCount + airports[b].outboundCount;
        return totalA > totalB;
    });

    std::cout << "\n--- Airport Connection Counts (Task 5) ---\n";
    std::cout << std::left << std::setw(10) << "Airport" << "Connections" << std::endl;
    
    for (int idx : sortedIndices) {
        int total = airports[idx].inboundCount + airports[idx].outboundCount;
        std::cout << std::left << std::setw(10) << airports[idx].code << total << std::endl;
    }
}

/**
 * Helper: Simple linear search for airport code since maps/hashes are STL.
 */
int Graph::getAirportIndex(const std::string& code) {
    for (int i = 0; i < (int)airports.size(); ++i) {
        if (airports[i].code == code) return i;
    }
    return -1;
}
