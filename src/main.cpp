#include <iostream>
#include <set>
#include <sstream>
#include <algorithm>
#include "geo.h"

int main()
{
    size_t n;
    std::cin >> n;
    std::cin.ignore();

    std::vector<Triangle> triangles(n);
    std::vector<Point> points(3);

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

        triangles[i] = Triangle(
            Point(coords[0], coords[1], coords[2]),
            Point(coords[3], coords[4], coords[5]), 
            Point(coords[6], coords[7], coords[8])
        );
    }

    size_t intersects = 0;

    std::set<size_t> intersectsTriangles;

    for (size_t i = 0; i < n; i++)
        for (size_t j = i + 1; j < n; j++)
            if (triangles[i].is_intersect(triangles[j]))
            {
                // std::cout << "Intersection found between triangle " << triangles[i] << " and triangle " << triangles[j] << "[ " << i << ", " << j << " ]" << std::endl;
                intersectsTriangles.insert(i);
                intersectsTriangles.insert(j);
            }
    for (const auto& element : intersectsTriangles)
        std::cout << element << std::endl;
        
    return 0;
}
 