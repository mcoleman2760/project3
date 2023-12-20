#include <iostream>
#include <fstream>
#include <list>
#include <cmath>
#include <vector>
#include <chrono>

class Node {
public:
    int id;
    double x, y;

    Node(int _id, double _x, double _y) : id(_id), x(_x), y(_y) {}

    // Calculate distance between this node and another node
    double distance(const Node& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

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

    std::list<Node>::iterator begin() {
        return nodes.begin();
    }

    std::list<Node>::iterator end() {
        return nodes.end();
    }
};

void nearestNeighbor(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    LinkedList unvisitedNodes;
    std::list<Node> visitedNodes;

    int id;
    double x, y;

    // Read nodes from the file and create a list of unvisited nodes
    while (file >> id >> x >> y) {
        unvisitedNodes.addNode(Node(id, x, y));
    }

    file.close();

    // Start the timer
    auto startTime = std::chrono::high_resolution_clock::now();

    // Start from the first node
    Node current = unvisitedNodes.front();
    visitedNodes.push_back(current);
    unvisitedNodes.popFront();

    double totalDistance = 0.0;

    // Visit nodes in order until all nodes are visited
    while (!unvisitedNodes.empty()) {
        double minDistance = std::numeric_limits<double>::infinity();
        auto nearestNode = unvisitedNodes.begin();

        // Find the nearest unvisited node
        for (auto it = unvisitedNodes.begin(); it != unvisitedNodes.end(); ++it) {
            double dist = current.distance(*it);
            if (dist < minDistance) {
                minDistance = dist;
                nearestNode = it;
            }
        }

        // Move to the nearest node
        current = *nearestNode;
        visitedNodes.push_back(current);
        unvisitedNodes.removeNode(*nearestNode);

        // Update total distance
        totalDistance += minDistance;
    }

    // Add the distance back to the starting node
    totalDistance += current.distance(visitedNodes.front());
    visitedNodes.push_back(visitedNodes.front()); // Add the starting node to the end

    // Stop the timer
    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Print the result
    for (const auto& node : visitedNodes) {
        std::cout << node.id << " ";
    }
    std::cout << std::endl;
    std::cout << "Total Distance: " << totalDistance << std::endl;
    std::cout << "Time in ms: " << elapsedTime << std::endl;
}
