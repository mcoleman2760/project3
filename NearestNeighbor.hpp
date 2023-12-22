//#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cmath>
#include <limits>
#include <algorithm>
#include <sstream>



class NODE
{
    private:
        int id_;
        double x_;
        double y_;

    public:
        NODE(int id, double x, double y) : id_(id), x_(x), y_(y) {}

        int getId() const
        {
            return this->id_;
        }
        double getX() const
        {
            return this->x_;
        }
        double getY() const
        {
            return this->y_;
        }

        //the crucial distance method
        static double distance(const NODE& a, const NODE& b){

            double inside1 = a.x_ - b.x_;
            double inside2 = a.y_ - b.y_;
            inside1 = inside1 * inside1;
            inside2 = inside2 * inside2;
            return std::sqrt(inside1 + inside2);
        }
};

//the function to read the txt file 
std::vector<NODE> read(const std::string& filename){
    std::ifstream file(filename);

    std::vector<NODE> nodes;
    std::string line;
    int id;
    double x, y;

    while (std::getline(file, line)){

        std::istringstream inputs(line);

        if (inputs >> id >> x >> y){
            nodes.emplace_back(id, x, y);
        }
    }
    return nodes;
}


//Nearest Neighbor algorithm
void nearestNeighbor(const std::string& filename)
{
    auto nodes = read(filename);
    std::vector<bool> visited(nodes.size(), false);
    std::vector<int> trip;
    double total = 0;


    // checks if the file is empty and has no nodes
    if (nodes.size() == 0){
        return;
    }

    auto current = nodes.begin();
    trip.push_back(current->getId());
    visited[current - nodes.begin()] = true;
    // starts the clock to find the duration
    auto start = std::chrono::steady_clock::now();

    for (size_t i = 1; i < nodes.size(); ++i){
        auto nearest = nodes.end();
        double minDistance = std::numeric_limits<double>::max();

        for (auto it = nodes.begin(); it != nodes.end(); ++it){
            if (!visited[it - nodes.begin()]){

                double distance = NODE::distance(*current, *it);
                if (distance < minDistance){
                    nearest = it;
                    minDistance = distance;
                }
            }
        }
       
        visited[nearest - nodes.begin()] = true;
        total = total + minDistance;
        trip.push_back(nearest->getId());
        current = nearest;
    }
    // gives the distance of the salesman
    total += NODE::distance(*current, nodes.front());
    trip.push_back(nodes.front().getId());

    // ends the clock and give the duration
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // prints the IDs of the nodes visited in order.
    for (int id : trip){
        std::cout << id << " ";
    }
   
    std::cout << "\nTotal Distance: " << total << "\nTime: " << duration.count() << "\n\n\n";
}
