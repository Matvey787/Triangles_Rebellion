#include <iostream>
#include <set>
#include <sstream>
#include <algorithm>
#include "geo.h"
#include "bvh.hpp"

int main()
{
    size_t n;
    std::cin >> n;
    std::cin.ignore();

    std::vector<Geo::Triangle> triangles(n);
    std::vector<Geo::Point> points(3);

    for (size_t i = 0; i < n; i++)
    {
        std::string line;
        std::vector<double> coords;
        
        while (coords.size() < 9) {
            std::getline(std::cin, line);
            
            if (line.empty() || line.find('#') != std::string::npos) {
                continue;
            }
            
            std::replace(line.begin(), line.end(), ',', ' ');
            for (char& c : line) {
                if (!std::isdigit(c) && c != '-' && c != '.' && c != ' ') {
                    c = ' ';
                }
            }
            
            std::istringstream iss(line);
            double coord;
            while (iss >> coord) {
                coords.push_back(coord);
            }
        }

        if (coords.size() != 9) {
            throw std::runtime_error("Invalid input: expected 9 coordinates for triangle, got " + 
                                std::to_string(coords.size()));
        }

        triangles[i] = Geo::Triangle(
            Geo::Point(coords[0], coords[1], coords[2]),
            Geo::Point(coords[3], coords[4], coords[5]), 
            Geo::Point(coords[6], coords[7], coords[8])
        );
    }

    BVH::BVH bvh(triangles);

    std::set<size_t> intersectsTriangles;

    for (size_t i = 0; i < n; i++)
    {
        std::vector<size_t> candidates;
        bvh.potentialOverlaps(i, candidates);

        for (size_t j : candidates)
        {
            if (j <= i) continue;
            if (triangles[i].is_intersect(triangles[j]))
            {
                intersectsTriangles.insert(i);
                intersectsTriangles.insert(j);
            }
        }
    }

    for (auto idx : intersectsTriangles)
        std::cout << idx << std::endl;
        
    return 0;
}
 