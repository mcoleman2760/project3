#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class Node {
public:
    int id;
    Node* edge1;
    Node* edge2;

    Node(int id) : id(id), edge1(nullptr), edge2(nullptr) {}
};

struct Edge {
    int fromId;
    int toId;
    double weight;

    Edge(int fromId, int toId) : fromId(fromId), toId(toId) {
        // Calculate Euclidean distance as weight
        double dx = fromId - toId;
        weight = std::abs(dx);
    }

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

class TSPSolver {
public:
    void greedyTSP(std::vector<Node>& nodes) {
        std::vector<Edge> edges;

        // Generate all possible edges
        for (Node& node1 : nodes) {
            for (Node& node2 : nodes) {
                if (&node1 != &node2) {
                    edges.emplace_back(node1.id, node2.id);
                }
            }
        }

        // Sort edges by weight
        std::sort(edges.begin(), edges.end());

        for (const Edge& edge : edges) {
            if (canAddEdge(edge,nodes)) {
                addEdge(edge, nodes);
                std::cout << "Edge from " << edge.fromId << " to " << edge.toId
                          << " of weight " << edge.weight << "\n";
            }
        }

        // Print final tour information
        printTourInfo(nodes);
    }

private:
    bool canAddEdge(const Edge& edge, const std::vector<Node>& nodes) const {
    // Check if adding the edge would form a cycle
    return edge.fromId != edge.toId &&
           (nodes[edge.fromId - 1].edge1 == nullptr || nodes[edge.fromId - 1].edge2 == nullptr) &&
           (nodes[edge.toId - 1].edge1 == nullptr || nodes[edge.toId - 1].edge2 == nullptr) &&
           ((nodes[edge.fromId - 1].edge1 != nullptr && !formsCycle(edge, nodes[edge.fromId - 1].edge1)) ||
            (nodes[edge.fromId - 1].edge2 != nullptr && !formsCycle(edge, nodes[edge.fromId - 1].edge2)) ||
            (nodes[edge.toId - 1].edge1 != nullptr && !formsCycle(edge, nodes[edge.toId - 1].edge1)) ||
            (nodes[edge.toId - 1].edge2 != nullptr && !formsCycle(edge, nodes[edge.toId - 1].edge2)));
}

bool formsCycle(const Edge& edge, const Node* current) const {
    // Check if adding the edge would form a cycle
    return current != nullptr &&
           (current->id != edge.fromId) &&
           (hasCycle(edge.fromId, current->edge1) || hasCycle(edge.fromId, current->edge2) ||
            hasCycle(edge.toId, current->edge1) || hasCycle(edge.toId, current->edge2));
}

bool hasCycle(int targetId, const Node* current) const {
    // Check if adding the edge would form a cycle
    return current != nullptr &&
           (current->id != targetId) &&
           (hasCycle(targetId, current->edge1) || hasCycle(targetId, current->edge2));
}
    void addEdge(const Edge& edge, std::vector<Node>& nodes) {
        // Add the edge to the nodes
        if (nodes[edge.fromId - 1].edge1 == nullptr) {
            nodes[edge.fromId - 1].edge1 = &nodes[edge.toId - 1];
        } else {
            nodes[edge.fromId - 1].edge2 = &nodes[edge.toId - 1];
        }

        if (nodes[edge.toId - 1].edge1 == nullptr) {
            nodes[edge.toId - 1].edge1 = &nodes[edge.fromId - 1];
        } else {
            nodes[edge.toId - 1].edge2 = &nodes[edge.fromId - 1];
        }
    }

    void printTourInfo(const std::vector<Node>& nodes) const {
        // Output the tour
        std::cout << "Tour: ";
        for (const Node& node : nodes) {
            std::cout << node.id << " ";
        }
        std::cout << "\n";

        // Calculate total distance
        double totalDistance = 0.0;
        for (const Node& node : nodes) {
            if (node.edge1 != nullptr) {
                totalDistance += std::abs(node.edge1->id - node.id);
            }
        }

        // Output total distance
        std::cout << "Total Distance: " << totalDistance << "\n";
    }
};
