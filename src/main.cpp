#include <iostream>
#include <set>
#include "geo.h"

int main()
{
    size_t n;
    std::cin >> n;

    std::vector<Triangle> triangles(n);
    std::vector<Point> points(3);

    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            std::cin >> points[j].x_ >> points[j].y_ >> points[j].z_;
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
 