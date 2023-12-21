#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <list>
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

class LinkedList {
public:
    std::list<Node> nodes;

    void addNode(const Node& node) {
        nodes.push_back(node);
    }

    void removeNode(const Node& node) {
        nodes.remove(node);
    }

    bool empty() const {
        return nodes.empty();
    }

    Node front() const {
        return nodes.front();
    }

    void popFront() {
        nodes.pop_front();
    }
};

void nearestNeighbor(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    LinkedList unvisited;
    int node_id;
    int x, y;

    // Read node information from the file
    while (file >> node_id ) {
        unvisited.addNode(Node(node_id, x, y));
    }

   file.close();

    clock_t start_time = clock();

    Node start_node = unvisited.front();
    Node current_node = start_node;
    LinkedList visited_nodes;
    int total_distance = 0;

    while (!unvisited.empty()) {
        unvisited.removeNode(current_node);
        visited_nodes.addNode(current_node);

        auto nearest_neighbor = std::min_element(unvisited.nodes.begin(), unvisited.nodes.end(),
            [&current_node](const Node& a, const Node& b) {
                return current_node.distance(a) < current_node.distance(b);
            }
        );

        total_distance = current_node.distance(*nearest_neighbor) + total_distance;
        current_node = *nearest_neighbor;
    }

    // Add the distance from the last node to the starting node
    total_distance += current_node.distance(start_node);

    // Ensure the first node is visited in the end
    visited_nodes.addNode(start_node);

    clock_t end_time = clock();
    int execution_time = double(end_time - start_time) / CLOCKS_PER_SEC * 1000;

    // Print the results
    for (const auto& node : visited_nodes.nodes) {
        std::cout << node.id << " ";
    }

    std::cout << std::endl;
    std::cout << "Total Distance: " << total_distance << std::endl;
    std::cout << "Time in ms: " << execution_time << std::endl;
}
