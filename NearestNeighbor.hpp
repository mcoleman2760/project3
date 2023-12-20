#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>

class Node {
public:
    int id;
    int x, y;
    Node* next;

    Node(int id, int x, int y) : id(id), x(x), y(y), next(nullptr) {}

    // Calculate Euclidean distance between two nodes
    double distanceTo(const Node& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    // Add a node to the end of the linked list
    void append(Node* newNode) {
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
};

class TSPSolver {
public:
    LinkedList nearestNeighborTSP(const std::vector<Node>& nodes) {
        int numNodes = nodes.size();
        LinkedList tour;
        std::vector<bool> visited(numNodes, false);

        auto startTime = std::chrono::high_resolution_clock::now();

        // Start at node 1
        int startNodeId = 1;
        Node* startNode = findNodeById(nodes, startNodeId);
        tour.append(startNode);
        visited[startNodeId - 1] = true;

        while (tour.head != nullptr && tour.head->next == nullptr) {
            Node* current = tour.head;
            double minDistance = std::numeric_limits<double>::max();
            int nearestNodeId = -1;

            // Find the nearest unvisited node
            for (int i = 0; i < numNodes; ++i) {
                if (!visited[i]) {
                    double distance = current->distanceTo(nodes[i]);
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestNodeId = i;
                    }
                }
            }

            // Move to the nearest neighbor
            Node* nearestNode = findNodeById(nodes, nearestNodeId + 1);
            tour.append(nearestNode);
            visited[nearestNodeId] = true;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        // Output the tour and other information
        printTourInfo(tour, duration.count());

        return tour;
    }

private:
    Node* findNodeById(const std::vector<Node>& nodes, int id) {
        for (const Node& node : nodes) {
            if (node.id == id) {
                return new Node(node.id, node.x, node.y);
            }
        }
        // Handle error if the node with the given id is not found
        throw std::invalid_argument("Node not found with ID: " + std::to_string(id));
    }

    void printTourInfo(const LinkedList& tour, long long duration) const {
        // Output the tour
        std::cout << "Tour: ";
        Node* current = tour.head;
        while (current != nullptr) {
            std::cout << current->id << " ";
            current = current->next;
        }
        std::cout << "\n";

        // Calculate total distance
        double totalDistance = 0.0;
        current = tour.head;
        while (current != nullptr && current->next != nullptr) {
            totalDistance += current->distanceTo(*(current->next));
            current = current->next;
        }

        // Output total distance and time
        std::cout << "Total Distance: " << totalDistance << "\n";
        std::cout << "Time in ms: " << duration << "\n";
    }
};
