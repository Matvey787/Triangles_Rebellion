# Geometry Library

## Intro

The Geometry Library provides control over standard geometric containers such as points, vectors, lines, 
and triangles.  

## Documentation

1. All interfaces are located in the GEO namespace. Library header is `geo.hpp`. Below is a simple example of how to create points and triangles using the library:

```cpp
#include "geo.hpp"

int main() {
    Geo::Point<float> p1(0.0f, 0.0f, 0.0f);
    Geo::Point<float> p2(1.0f, 0.0f, 0.0f);
    Geo::Point<float> p3(0.0f, 1.0f, 0.0f);

    Geo::Triangle<float> triangle(p1, p2, p3);

    Geo::Vector<float> normal = triangle.getNormal();

    return 0;
}
```

The library also contains an algorithm for quickly finding intersections between triangles (which is essentially the purpose of this library). The algorithm is called BVH (Bounding Volume Hierarchy). 

Excerpt from the example:

```cpp
BVH::BVH<double> bvh(triangles); // creating the BVH structure from triangles

std::set<size_t> intersectsTriangles;

for (size_t i = 0, numberOfTriangles = triangles.size(); i < numberOfTriangles; i++)
{
    std::vector<size_t> candidates;     // candidates for intersection with triangle i
    bvh.potentialOverlaps(i, candidates); // looking for potential intersections 

    for (size_t j : candidates)
    {
        if (j <= i) continue;
        if (triangles[i].is_intersect(triangles[j])) // checking for real intersection
        {
            intersectsTriangles.insert(i);
            intersectsTriangles.insert(j);
        }
    }
}
```

2. To compile the project, you need to enable the add_subdirectory(path/to/geo_lib) command in cmake, then add the geo_lib library to the targets that use this library. (Same with bvh_lib)

Example you can find [here](examples/geo_lib/CMakeLists.txt)

## Examples

All examples can be found [here](examples/geo_lib/)

To run the example, do the following:

```bash
git clone https://github.com/Matvey787/Triangles_Rebellion.git

cd Triangles_Rebellion

cd examples/geo_lib

cmake -G=Ninja -S . -B build -DCMAKE_CXX_COMPILER=clang++

# If there are not enough dependencies after generation, install them.

# Compile the example
cmake --build build

# Run
./build/intersection

```

## Soureces

The source files are located [here](geo_lib/)