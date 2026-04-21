#include "CoreGraph.h"

/**
 * Task 1: Load graph from CSV [cite: 37, 65]
 * Reads airports.csv to build the weighted directed graph.
 */
bool Graph::loadFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> fields;
        std::string current;
        bool inQuotes = false;

        for (int i = 0; i < (int)line.size(); i++) {
            char c = line[i];

            if (c == '"') {
                inQuotes = !inQuotes;
            }
            else if (c == ',' && !inQuotes) {
                fields.push_back(current);
                current.clear();
            }
            else {
                current += c;
            }
        }
        fields.push_back(current);

        if (fields.size() != 6) {
            std::cerr << "Skipping bad row: " << line << std::endl;
            continue;
        }

        std::string origin = fields[0];
        std::string dest = fields[1];
        std::string originCity = fields[2];
        std::string destCity = fields[3];
        std::string distStr = fields[4];
        std::string costStr = fields[5];

        int u = addOrFindAirport(origin, originCity);
        int v = addOrFindAirport(dest, destCity);

        double distance = std::stod(distStr);
        double cost = std::stod(costStr);

        adj[u].push_back(Edge(v, distance, cost));

        airports[u].outboundCount++;
        airports[v].inboundCount++;
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
std::string Graph::extractState(const std::string& cityStr) const {
    size_t lastSpace = cityStr.find_last_of(' ');
    if (lastSpace != std::string::npos) {
        return cityStr.substr(lastSpace + 1);
    }
    return "";
}

Edge::Edge(int to, double dist, double cst) : toIndex(to), distance(dist), cost(cst) {}

Airport::Airport(std::string c, std::string ct, std::string st)
    : code(c), city(ct), state(st), inboundCount(0), outboundCount(0) {}

int Graph::getNumAirports() const {
    return airports.size();
}

const std::vector<std::vector<Edge>>& Graph::getAdj() const {
    return adj;
}

const std::vector<Airport>& Graph::getAirports() const {
    return airports;
}

std::string Graph::getStateOfAirport(int index) const {
    return airports[index].state;
}

std::string Graph::getCodeOfAirport(int index) const {
    return airports[index].code;
}

int Graph::findAirportIndex(const std::string& code) const {
    for (int i = 0; i < (int)airports.size(); ++i) {
        if (airports[i].code == code) return i;
    }
    return -1;
}
