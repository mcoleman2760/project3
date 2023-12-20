#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <math.h>

class Node {
public:
    int id;
    double x, y;

    Node(int node_id, double x_coord, double y_coord) : id(node_id), x(x_coord), y(y_coord) {}

    double distance(const Node& other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    // Equality operator
    bool operator==(const Node& other) const {
        return id == other.id && x == other.x && y == other.y;
    }
};

void nearestNeighbor(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::vector<Node> nodes;
    int node_id;
    double x, y;

    // Read node information from the file
    while (file >> node_id >> x >> y) {
        nodes.emplace_back(node_id, x, y);
    }

    file.close();

    clock_t start_time = clock();

    // Initialize unvisited nodes with all nodes
    std::vector<Node> unvisited(nodes);
    Node current_node = nodes[0];
    std::vector<Node> visited_nodes = {current_node};
    double total_distance = 0;

    while (!unvisited.empty()) {
        // Remove the current node from unvisited
        unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), current_node), unvisited.end());

        // Find the nearest neighbor to the current node
        auto nearest_neighbor = std::min_element(unvisited.begin(), unvisited.end(),
            [&current_node](const Node& a, const Node& b) {
                return current_node.distance(a) < current_node.distance(b);
            }
        );

        // Update total distance and move to the nearest neighbor
        total_distance += current_node.distance(*nearest_neighbor);
        current_node = *nearest_neighbor;
        visited_nodes.push_back(current_node);
    }

    // Add the distance from the last node to the starting node
    total_distance += visited_nodes.back().distance(visited_nodes[0]);

    clock_t end_time = clock();
    double execution_time = double(end_time - start_time) / CLOCKS_PER_SEC * 1000;

    // Print the results
    for (const auto& node : visited_nodes) {
        std::cout << node.id << " ";
    }

    std::cout << std::endl;
    std::cout << "Total Distance: " << total_distance << std::endl;
    std::cout << "Time in ms: " << execution_time << std::endl;
}
