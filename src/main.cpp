#include <iostream>
#include <set>
#include <sstream>
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
        for (size_t j = 0; j < 3; j++)
        {
            // чисто моя преблуда для парсинга файла теста
            std::string line;
            std::getline(std::cin, line);

            while (line.empty() || line.find('#') != std::string::npos) std::getline(std::cin, line);

            for (char& c : line) {
                if (!std::isdigit(c) && c != '-' && c != '.' && c != ' ')
                    c = ' ';
            }
            
            std::istringstream iss(line);
            iss >> points[j].x_ >> points[j].y_ >> points[j].z_;
        }
        triangles[i] = Triangle(points[0], points[1], points[2]);
    }

    size_t intersects = 0;

    std::set<size_t> intersectsTriangles;

    for (size_t i = 0; i < n; i++)
        for (size_t j = i + 1; j < n; j++)
            if (triangles[i].is_intersect(triangles[j]))
            {
                intersectsTriangles.insert(i);
                intersectsTriangles.insert(j);
            }
    for (const auto& element : intersectsTriangles)
        std::cout << element << std::endl;
        
    return 0;
}
 