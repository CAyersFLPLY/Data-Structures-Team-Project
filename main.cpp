#include <iostream>
#include <string>
#include <vector>
#include "CoreGraph.h"
#include "Routing.h"

using namespace std;

// Prints one path result
void printSinglePathResult(const Graph& g, const PathResult& result, const string& label) {
    cout << "\n" << label << "\n";

    if (!result.found) {
        cout << "None\n";
        return;
    }

    for (int i = 0; i < (int)result.path.size(); i++) {
        cout << g.getCodeOfAirport(result.path[i]);
        if (i < (int)result.path.size() - 1) {
            cout << " -> ";
        }
    }

    cout << "\nTotal Distance: " << result.totalDistance << "\n";
    cout << "Total Cost: " << result.totalCost << "\n";
}

// Prints multiple state path results
void printStatePaths(const Graph& g, const vector<PathResult>& results, const string& originCode, const string& state) {
    cout << "\nShortest paths from " << originCode << " to airports in state " << state << ":\n";

    if (results.empty()) {
        cout << "No such paths exist.\n";
        return;
    }

    for (int i = 0; i < (int)results.size(); i++) {
        if (!results[i].found) {
            continue;
        }

        for (int j = 0; j < (int)results[i].path.size(); j++) {
            cout << g.getCodeOfAirport(results[i].path[j]);
            if (j < (int)results[i].path.size() - 1) {
                cout << " -> ";
            }
        }

        cout << " | Distance: " << results[i].totalDistance;
        cout << " | Cost: " << results[i].totalCost << "\n";
    }
}

int main() {
    Graph g;

    // Load CSV file
    if (!g.loadFromCSV("airports.csv")) {
        cout << "Failed to load airports.csv\n";
        return 1;
    }

    cout << "Airport graph loaded successfully.\n";
    cout << "Total airports loaded: " << g.getNumAirports() << "\n";

    int choice;

    do {
        cout << "\n=============================\n";
        cout << " Airport Routing System Menu\n";
        cout << "=============================\n";
        cout << "1. Find shortest path between two airports\n";
        cout << "2. Find shortest paths to all airports in a state\n";
        cout << "3. Find shortest path with exact number of stops\n";
        cout << "4. Display airport connection counts\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string originCode, destCode;

            cout << "Enter origin airport code: ";
            cin >> originCode;
            cout << "Enter destination airport code: ";
            cin >> destCode;

            int start = g.findAirportIndex(originCode);
            int end = g.findAirportIndex(destCode);

            if (start == -1 || end == -1) {
                cout << "Invalid airport code entered.\n";
                continue;
            }

            PathResult result = Routing::shortestPath(g, start, end);
            printSinglePathResult(g, result, "Shortest Path Result");
        }
        else if (choice == 2) {
            string originCode, state;

            cout << "Enter origin airport code: ";
            cin >> originCode;
            cout << "Enter destination state abbreviation (ex: FL): ";
            cin >> state;

            int start = g.findAirportIndex(originCode);

            if (start == -1) {
                cout << "Invalid origin airport code.\n";
                continue;
            }

            vector<PathResult> results = Routing::shortestPathsToState(g, start, state);
            printStatePaths(g, results, originCode, state);
        }
        else if (choice == 3) {
            string originCode, destCode;
            int stops;

            cout << "Enter origin airport code: ";
            cin >> originCode;
            cout << "Enter destination airport code: ";
            cin >> destCode;
            cout << "Enter exact number of stops: ";
            cin >> stops;

            int start = g.findAirportIndex(originCode);
            int end = g.findAirportIndex(destCode);

            if (start == -1 || end == -1) {
                cout << "Invalid airport code entered.\n";
                continue;
            }

            PathResult result = Routing::shortestPathWithStops(g, start, end, stops);
            printSinglePathResult(g, result, "Shortest Path With Stops Result");
        }
        else if (choice == 4) {
            g.displayConnections();
        }
        else if (choice == 5) {
            cout << "Exiting program.\n";
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 5);

    return 0;
}
