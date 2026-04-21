# Data-Structures-Team-Project
Graph-Based Airport Connectivity and Flight Route Optimization System
Overview

This project is a graph-based system that models airport connections and flight routes using C++. Each airport is treated as a node (vertex), and each direct flight between airports is an edge. Every edge has two weights:

Distance
Cost

The program reads data from a CSV file (airports.csv) and builds a directed graph. Then it uses different graph algorithms to answer routing and optimization questions.

This project was built for a Data Structures final and focuses on applying graph algorithms to a real-world scenario.

What the Program Does

The program supports the following features:

Builds a directed graph from a CSV file
Finds the shortest path between two airports
Finds shortest paths from one airport to all airports in a given state
Finds the shortest path with an exact number of stops
Counts and displays direct flight connections for each airport
Converts the graph into an undirected version (for MST)
Runs Prim’s algorithm to find a Minimum Spanning Tree
Runs Kruskal’s algorithm to find a Minimum Spanning Tree / Forest
Dataset

The program uses a file called airports.csv.

Each row represents a flight and includes:

Origin airport (ex: ATL)
Destination airport (ex: MIA)
Origin city/state
Destination city/state
Distance
Cost

Data Structures Used
Airport

Stores:

airport code
city
state
inbound flight count
outbound flight count
Edge

Represents a flight:

destination index
distance
cost
Graph

Main structure using an adjacency list:

vector of airports
vector of edge lists
MinHeap

Used for Dijkstra’s algorithm (shortest path)

Disjoint Set (Union-Find)

Used for Kruskal’s algorithm

File Structure
CoreGraph.h
CoreGraph.cpp
Routing.h
Routing.cpp
Spanning.h
Spanning.cpp
main.cpp
airports.csv
README.md
What each file does
CoreGraph → builds and stores the graph
Routing → shortest path algorithms
Spanning → MST (Prim + Kruskal)
main.cpp → runs the program and menu
How It Works
Task 1: Build Graph
Reads CSV file
Adds airports as nodes
Adds flights as directed edges
Stores distance and cost
Task 2: Shortest Path (Dijkstra)

Finds shortest path between two airports.

Uses distance as the main weight
Also calculates total cost

Output:

Path (ex: ATL → ORD → MIA)
Total distance
Total cost
Or “None” if no path exists
Task 3: Shortest Paths to a State

From one airport, finds shortest paths to all airports in a given state.

How it works:

Run Dijkstra once
Check all airports that match the state
Print paths to each
Task 4: Path with Exact Stops

Finds shortest path with a specific number of stops.


The algorithm tracks:

number of edges used
best distance
Task 5: Connection Counts

Counts direct connections:

total = inbound + outbound

Then sorts airports from highest to lowest connections.

Task 6: Undirected Graph

Converts directed graph to undirected using rules:

If only one direction exists → keep it
If both directions exist → keep the cheaper one
Ignore distance (use cost only)
Task 7: Prim’s Algorithm

Finds a Minimum Spanning Tree using cost.

Output:

edges in MST
total cost

If graph is disconnected → prints message

Task 8: Kruskal’s Algorithm

Also finds MST using:

sorted edges
union-find

