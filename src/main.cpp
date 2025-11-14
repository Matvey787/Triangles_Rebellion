#include <iostream>
#include <set>
#include <sstream>
#include <algorithm>
#include "geo_triangle.hpp"
#include "bvh.hpp"

int main()
{
    long long temp = 0;
    size_t n = 0;

    while (true)
    {
        if (!(std::cin >> temp)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Try again: ";
            continue;
        }

        if (temp < 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Number must be non-negative. Try again: ";
            continue;
        }

        n = static_cast<size_t>(temp);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        break;
    }


    std::vector<Geo::Triangle<double>> triangles(n);
    std::vector<Geo::Point<double>> points(3);

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

        triangles[i] = Geo::Triangle<double>(
            Geo::Point<double>(coords[0], coords[1], coords[2]),
            Geo::Point<double>(coords[3], coords[4], coords[5]), 
            Geo::Point<double>(coords[6], coords[7], coords[8])
        );
    }

    BVH::BVH<double> bvh(triangles);

    std::set<size_t> intersectsTriangles;

    //bvh.potentianlOverlapsPrint(1872, triangles);

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
 