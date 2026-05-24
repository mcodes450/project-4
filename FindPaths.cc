#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

#include "graph.h"

using namespace std;

int pathfindDriver(int argc, char **argv) {
    if (argc != 3) {
        return 0;
    }
    string graph_file = argv[1];
    int start_vertex = stoi(argv[2]);

    Graph graph;
    if (!graph.loadGraphFromFile(graph_file)) {
        return 0;
    }

    vector<double> distances;
    vector<int> path_prefixes;

    graph.dijkstra(start_vertex, distances, path_prefixes);

    for (int i = 1; i <= graph.getNumVertices(); ++i) {
        if (distances[i] == numeric_limits<double>::infinity()) {
            cout << i << ": not_possible" << endl;
        } else {
            vector<int> path;
            for (int curr = i; curr != -1; curr = path_prefixes[curr]) {
                path.push_back(curr);
            }
            cout << i << ":";
            for (auto it = path.rbegin(); it != path.rend(); ++it) {
                cout << " " << *it;
            }
            cout << " cost: " << distances[i] << endl;
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
		cout << "Usage: " << argv[0] << " <GRAPH_FILE>" << "<STARTING_VERTEX>" << endl;
		return 0;
    }

    pathfindDriver(argc, argv);

    return 0;
}
