# Graph-Based Airport Connectivity and Flight Route Optimization System

## Overview

This project is a graph-based system that models airport connections and flight routes using C++. Each airport is treated as a node (vertex), and each direct flight between airports is an edge. Every edge has two weights:

- Distance  
- Cost  

The program reads data from a CSV file (airports.csv) and builds a directed graph. Then it uses different graph algorithms to answer routing and optimization problems.

This project was built for a Data Structures final and focuses on applying graph algorithms to a real-world scenario.

---

## What the Program Does

The program supports the following features:

1. Builds a directed graph from a CSV file  
2. Finds the shortest path between two airports  
3. Finds shortest paths from one airport to all airports in a given state  
4. Finds the shortest path with an exact number of stops  
5. Counts and displays direct flight connections for each airport  
6. Converts the graph into an undirected version (for MST)  
7. Runs Prim’s algorithm to find a Minimum Spanning Tree  
8. Runs Kruskal’s algorithm to find a Minimum Spanning Tree / Forest  

---

## Dataset

The program uses a file called airports.csv.

Each row represents a flight and includes:

- Origin airport (ex: ATL)  
- Destination airport (ex: MIA)  
- Origin city/state  
- Destination city/state  
- Distance  
- Cost  

Example:
ATL,MIA,Atlanta, GA,Miami, FL,595,123

This means there is a flight from ATL to MIA with:
- Distance = 595  
- Cost = 123  

The graph is directed, so ATL -> MIA does NOT automatically mean MIA -> ATL.

---

## Data Structures Used

Airport:
- airport code  
- city  
- state  
- inbound flight count  
- outbound flight count  

Edge:
- destination index  
- distance  
- cost  

Graph:
- vector of airports  
- adjacency list (vector of vectors of edges)  

MinHeap:
- used for Dijkstra’s algorithm  

Disjoint Set (Union-Find):
- used for Kruskal’s algorithm  

---

## File Structure

CoreGraph.h  
CoreGraph.cpp  
Routing.h  
Routing.cpp  
Spanning.h  
Spanning.cpp  
main.cpp  
airports.csv  
README.md  

CoreGraph → builds and stores the graph  
Routing → shortest path algorithms  
Spanning → MST (Prim + Kruskal)  
main.cpp → runs the program  

---

## How It Works

Task 1: Build Graph  
- Reads CSV file  
- Adds airports as nodes  
- Adds flights as directed edges  
- Stores distance and cost  

Task 2: Shortest Path (Dijkstra)  
- Uses distance as the main weight  
- Also calculates total cost  

Output:
- Path (ex: ATL -> ORD -> MIA)  
- Total distance  
- Total cost  
- Or "None" if no path exists  

Task 3: Shortest Paths to a State  
- Run Dijkstra once  
- Check all airports in the given state  
- Print shortest paths to each  

Task 4: Path with Exact Stops  
Example:
IAD -> ORD -> DFW -> ATL -> MIA  

This has:
- 3 stops  
- 4 edges  

The algorithm tracks:
- number of edges used  
- best distance  

Task 5: Connection Counts  
total = inbound + outbound  

Then sorts airports from highest to lowest connections  

Task 6: Undirected Graph  
- If one direction exists → keep it  
- If both exist → keep the cheaper one  
- Ignore distance, use cost  

Task 7: Prim’s Algorithm  
- Builds MST using cost  
- Outputs edges and total cost  
- If disconnected → prints message  

Task 8: Kruskal’s Algorithm  
- Uses sorted edges + union-find  
- Outputs MST or forest  

---

## Algorithms Used

- Dijkstra’s Algorithm  
- Dynamic programming (for stops constraint)  
- Prim’s Algorithm  
- Kruskal’s Algorithm  
- Union-Find  

---

## Time Complexity (Simple)

- Dijkstra: O((V + E) log V)  
- Kruskal: O(E log E)  
- Prim: O((V + E) log V)  
- Graph build: O(E * V)  

---


## Features in Menu

- Shortest path between airports  
- Shortest paths to a state  
- Path with stops  
- Connection counts  
- MST algorithms  
- Exit  

---

## Edge Cases Handled

- invalid airport codes  
- no path exists  
- no airports in a state  
- no path with given stops  
- disconnected graph  

---

## Design Choices

- Used adjacency list because it's efficient  
- Used distance for shortest path  
- Used cost for MST (assignment requirement)  
- Split into multiple files for organization  

