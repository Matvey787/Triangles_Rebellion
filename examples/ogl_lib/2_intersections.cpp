
// OpenGL renders intersecting triangles in red and non-intersecting triangles in blue.

#include <iostream>
#include <memory>
#include "geo.hpp"

import OGLWrap;

#include <iostream>
#include <set>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "geo.hpp"
#include "bvh.hpp"

std::vector<Geo::Triangle<float>> input_stdin();
std::vector<Geo::Triangle<float>> input_fstream(std::string_view fileName);
void inputCoords(std::vector<Geo::Triangle<float>>& triangles, std::istream& iss);

int main(int argc, const char* argv[])
{
    std::vector<Geo::Triangle<float>> triangles;

    if (argc > 1)
    {
        triangles = input_fstream(argv[1]);
    }
    else
    {
        triangles = input_stdin();
    }

    BVH::BVH<float> bvh(triangles);

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

    std::unique_ptr<OGLWrap::Win3D> w1 = OGLWrap::Win3DBuilder()
                    .setWidth(1600)
                    .setHeight(1200)
                    .setTitle("First Window")
                    .setBgColor(OGLWrap::ColorFactory::fromName("black"))
                    .setStart_posX(0)
                    .setStart_posY(0)
                    .build();

    size_t triangleCount = triangles.size();
    for (size_t idx = 0; idx < triangleCount; idx++)
    {
        if (intersectsTriangles.find(idx) == intersectsTriangles.end())
            w1->addTriangle(triangles[idx], OGLWrap::RGB_color{0, 0, 255});
        else
            w1->addTriangle(triangles[idx], OGLWrap::RGB_color{255, 0, 0});
        w1->render();
    }

    while (true) {
        w1->render();
    }
        
    return 0;
}

std::vector<Geo::Triangle<float>> input_stdin()
{
    long long temp = 0;
    size_t n = 0;

    if (!(std::cin >> temp)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("Invalid input. Expected a number.");
    }

    if (temp < 0) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("Number must be non-negative.");
    }

    n = static_cast<size_t>(temp);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::vector<Geo::Triangle<float>> triangles(n);
    inputCoords(triangles, std::cin);    

    return triangles;
}
 
std::vector<Geo::Triangle<float>> input_fstream(std::string_view fileName)
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

    std::vector<Geo::Triangle<float>> triangles(n);
    inputCoords(triangles, inputFile);

    inputFile.close();

    return triangles;
}

void inputCoords(std::vector<Geo::Triangle<float>>& triangles, std::istream& iss)
{
    std::vector<float> coords;

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

        triangles[i] = Geo::Triangle<float>(
            Geo::Point<float>(coords[0], coords[1], coords[2]),
            Geo::Point<float>(coords[3], coords[4], coords[5]), 
            Geo::Point<float>(coords[6], coords[7], coords[8])
        );

        coords.clear();
    }
}
