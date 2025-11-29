#include <iostream>
#include <set>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "geo_triangle.hpp"
#include "bvh.hpp"

std::vector<Geo::Triangle<double>> input_stdin();
std::vector<Geo::Triangle<double>> input_fstream(std::string_view fileName);
void inputCoords(std::vector<Geo::Triangle<double>>& triangles, std::istream& iss);
std::set<size_t> analyzeIntersections_ON_BVH(std::vector<Geo::Triangle<double>>& triangles);
std::set<size_t> analyzeIntersections_OFF_BVH(std::vector<Geo::Triangle<double>>& triangles);

int main(int argc, const char* argv[])
{
    std::vector<Geo::Triangle<double>> triangles;

    if (argc > 1)
    {
        triangles = input_fstream(argv[1]);
    }
    else
    {
        triangles = input_stdin();
    }

    #ifdef BVH_OFF
        std::set<size_t> intersectsTriangles = analyzeIntersections_OFF_BVH(triangles);
    #else
        std::set<size_t> intersectsTriangles = analyzeIntersections_ON_BVH(triangles);
    #endif

    for (auto idx : intersectsTriangles)
        std::cout << idx << std::endl;
        
    return 0;
}

std::set<size_t> analyzeIntersections_ON_BVH(std::vector<Geo::Triangle<double>>& triangles)
{
    BVH::BVH<double> bvh(triangles);

    std::set<size_t> intersectsTriangles;

    for (size_t i = 0, numberOfTriangles = triangles.size(); i < numberOfTriangles; i++)
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

    return intersectsTriangles;
}

std::set<size_t> analyzeIntersections_OFF_BVH(std::vector<Geo::Triangle<double>>& triangles)
{
    std::set<size_t> intersectsTriangles;

    for (size_t i = 0, numberOfTriangles = triangles.size(); i < numberOfTriangles; i++)
        for (size_t j = i + 1; j < numberOfTriangles; j++)
        {
            if (triangles[i].is_intersect(triangles[j]))
            {
                intersectsTriangles.insert(i);
                intersectsTriangles.insert(j);
            }
        }

    return intersectsTriangles;
}

std::vector<Geo::Triangle<double>> input_stdin()
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
    inputCoords(triangles, std::cin);    

    return triangles;
}
 
std::vector<Geo::Triangle<double>> input_fstream(std::string_view fileName)
{
    std::ifstream inputFile(fileName.data());

    if (!inputFile.is_open())
        throw std::runtime_error("Failed to open file" + std::string(fileName));

    long temp = 0;
    size_t n = 0;

    inputFile >> temp;
    if (temp < 0)
        throw std::runtime_error("Number of triangles must be non-negative");
    
    n = static_cast<size_t>(temp);

    std::vector<Geo::Triangle<double>> triangles(n);
    inputCoords(triangles, inputFile);

    inputFile.close();

    return triangles;
}

void inputCoords(std::vector<Geo::Triangle<double>>& triangles, std::istream& iss)
{
    std::vector<double> coords;

    for (size_t i = 0, numberOfTriangles = triangles.size(); i < numberOfTriangles; i++)
    {
        std::string line;
        
        while (coords.size() < 9) {
            std::getline(iss, line);
            
            if (line.empty() || line.find('#') != std::string::npos) {
                continue;
            }
            
            std::replace(line.begin(), line.end(), ',', ' ');

            for (char& c : line) if (!std::isdigit(c) && c != '-' && c != '.' && c != ' ') c = ' ';
            
            std::istringstream iss(line);

            double coord = 0.0;

            while (iss >> coord)
            {
                coords.push_back(coord);
            }
            
        }
        if (coords.size() != 9)
        {
            throw std::runtime_error("Invalid input: expected 9 coordinates for triangle, got "
                                        + std::to_string(coords.size()));
        }

        triangles[i] = Geo::Triangle<double>(
            Geo::Point<double>(coords[0], coords[1], coords[2]),
            Geo::Point<double>(coords[3], coords[4], coords[5]), 
            Geo::Point<double>(coords[6], coords[7], coords[8])
        );

        coords.clear();
    }
}