#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>

class Node {
public:
    int id;
    int x, y;

    Node(int id, int x, int y) : id(id), x(x), y(y) {}

    // Calculate Euclidean distance between two nodes
    double distanceTo(const Node& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

class TSPSolver {
public:
    std::vector<Node> nearestNeighborTSP(const std::vector<Node>& nodes) {
        int numNodes = nodes.size();
        std::vector<Node> tour;
        std::vector<bool> visited(numNodes, false);

        auto startTime = std::chrono::high_resolution_clock::now();

        // Start at node 1
        int startNodeId = 1;
        tour.push_back(getNodeById(nodes, startNodeId));
        visited[startNodeId - 1] = true;

        while (tour.size() < numNodes) {
            Node current = tour.back();
            double minDistance = std::numeric_limits<double>::max();
            int nearestNodeId = -1;

            // Find the nearest unvisited node
            for (int i = 0; i < numNodes; ++i) {
                if (!visited[i]) {
                    double distance = current.distanceTo(nodes[i]);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestNodeId = i;
                    }
                }
            }

            // Move to the nearest neighbor
            tour.push_back(nodes[nearestNodeId]);
            visited[nearestNodeId] = true;
        }

        // Return to the starting node to complete the tour
        tour.push_back(getNodeById(nodes, startNodeId));

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        // Output the tour and other information
        printTourInfo(tour, duration.count());

        return tour;
    }

private:
    Node getNodeById(const std::vector<Node>& nodes, int id) {
        for (const Node& node : nodes) {
            if (node.id == id) {
                return node;
            }
        }
        // Handle error if the node with the given id is not found
        throw std::invalid_argument("Node not found with ID: " + std::to_string(id));
    }

    void printTourInfo(const std::vector<Node>& tour, long long duration) const {
        // Output the tour
        std::cout << "Tour: ";
        for (const Node& node : tour) {
            std::cout << node.id << " ";
        }
        std::cout << "\n";

        // Calculate total distance
        double totalDistance = 0.0;
        for (size_t i = 0; i < tour.size() - 1; ++i) {
            totalDistance += tour[i].distanceTo(tour[i + 1]);
        }

        // Output total distance and time
        std::cout << "Total Distance: " << totalDistance << "\n";
        std::cout << "Time in ms: " << duration << "\n";
    }
};
