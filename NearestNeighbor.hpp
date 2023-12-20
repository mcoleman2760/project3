#include <iostream>
#include <fstream>
#include <list>
#include <ctime>
#include <cmath>
#include <vector>

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
};

void nearestNeighbor(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::list<Node> unvisitedNodes;
    std::list<Node> visitedNodes;

    int id;
    double x, y;

    // Read nodes from the file and create a list of unvisited nodes
    while (file >> id >> x >> y) {
        unvisitedNodes.push_back(Node(id, x, y));
    }

    file.close();

    // Start the timer
    clock_t startTime = clock();

    // Start from the first node
    Node current = unvisitedNodes.front();
    visitedNodes.push_back(current);
    unvisitedNodes.pop_front();

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
        unvisitedNodes.erase(nearestNode);

        // Update total distance
        totalDistance += minDistance;
    }

    // Add the distance back to the starting node
    totalDistance += current.distance(visitedNodes.front());
    visitedNodes.push_back(visitedNodes.front()); // Add the starting node to the end

    // Stop the timer
    clock_t endTime = clock();
    double elapsedTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC * 1000.0;

    // Print the result
    for (const auto& node : visitedNodes) {
        std::cout << node.id << " ";
    }

    std::cout << "\nTotal Distance: " << totalDistance << "\nTime in ms: " << elapsedTime << std::endl;
}
