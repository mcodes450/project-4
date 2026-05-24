#ifndef STUDENT_GRAPH
#define STUDENT_GRAPH

#include "binary_heap.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

class Graph {
public:
    struct Edge {
        int to;
        double weight;
    };

    struct DijkstraNode {
        int vertex;
        double cost;

        // Default constructor needed by BinaryHeap internal array allocations
        DijkstraNode() : vertex(0), cost(0.0) {}
        DijkstraNode(int v, double c) : vertex(v), cost(c) {}

        // Min-heap ordering property: lowest cost is prioritized
        bool operator<(const DijkstraNode& other) const {
            return cost < other.cost;
        }
    };

    Graph() : num_vertices_(0) {}

    // Initialize adjacency list size based on vertex count
    void init(int num_vertices) {
        num_vertices_ = num_vertices;
        adj_.assign(num_vertices_ + 1, std::vector<Edge>());
    }

    // Safely insert directed edge details
    void addEdge(int from, int to, double weight) {
        if (from >= 1 && from <= num_vertices_ && to >= 1 && to <= num_vertices_) {
            adj_[from].push_back({to, weight});
        }
    }

    // Parse the graph configuration file line-by-line [cite: 20]
    bool loadGraphFromFile(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            return false;
        }
        std::string line;
        if (std::getline(infile, line)) {
            std::stringstream ss(line);
            int num_vertices;
            if (ss >> num_vertices) {
                init(num_vertices);
            }
        }
        while (std::getline(infile, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            int from;
            if (!(ss >> from)) continue;
            int to;
            double weight;
            while (ss >> to >> weight) {
                addEdge(from, to, weight);
            }
        }
        return true;
    }

    // Query exact direct weights for Q1 adjacency validation [cite: 28, 29]
    bool getEdgeWeight(int from, int to, double& weight) const {
        if (from < 1 || from > num_vertices_ || to < 1 || to > num_vertices_) {
            return false;
        }
        for (const auto& edge : adj_[from]) {
            if (edge.to == to) {
                weight = edge.weight;
                return true;
            }
        }
        return false;
    }

    int getNumVertices() const {
        return num_vertices_;
    }

    // Priority-queue-based Dijkstra implementation [cite: 50]
    void dijkstra(int start_vertex, std::vector<double>& distances, std::vector<int>& path_prefixes) const {
        distances.assign(num_vertices_ + 1, std::numeric_limits<double>::infinity());
        path_prefixes.assign(num_vertices_ + 1, -1);

        if (start_vertex < 1 || start_vertex > num_vertices_) {
            return;
        }

        distances[start_vertex] = 0.0;
        BinaryHeap<DijkstraNode> pq;
        pq.insert(DijkstraNode(start_vertex, 0.0));

        while (!pq.isEmpty()) {
            DijkstraNode current;
            pq.deleteMin(current);

            int u = current.vertex;
            double d = current.cost;

            if (d > distances[u]) {
                continue;
            }

            for (const auto& edge : adj_[u]) {
                int v = edge.to;
                double weight = edge.weight;
                if (distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    path_prefixes[v] = u;
                    pq.insert(DijkstraNode(v, distances[v]));
                }
            }
        }
    }

private:
    int num_vertices_;
    std::vector<std::vector<Edge>> adj_;
};

#endif
