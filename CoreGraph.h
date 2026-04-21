#ifndef COREGRAPH_H
#define COREGRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

struct Edge {
    int toIndex;
    double distance;
    double cost;

    Edge(int to, double dist, double cst);
};

struct Airport {
    std::string code;
    std::string city;
    std::string state;
    int inboundCount;
    int outboundCount;

    Airport(std::string c, std::string ct, std::string st);
};

class Graph {
private:
    std::vector<Airport> airports;
    std::vector<std::vector<Edge>> adj;

    int getAirportIndex(const std::string& code);
    int addOrFindAirport(const std::string& code, const std::string& cityFull);

public:
    bool loadFromCSV(const std::string& filename);
    void displayConnections();

    // REQUIRED FOR ROUTING + SPANNING
    int getNumAirports() const;
    const std::vector<std::vector<Edge>>& getAdj() const;
    const std::vector<Airport>& getAirports() const;

    std::string getStateOfAirport(int index) const;
    std::string getCodeOfAirport(int index) const;
    std::string extractState(const std::string& cityStr) const;

    int findAirportIndex(const std::string& code) const;
};

#endif
