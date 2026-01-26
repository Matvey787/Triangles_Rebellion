#ifndef GEO_H
#define GEO_H

#include "double.h"
#include "geo_detail.hpp"

#include <cmath>
#include <vector>
#include <ostream>

namespace Geo
{

using namespace detail;


/// @brief Класс, представляющий треугольник в 3D пространстве
class Triangle
{
    Point p1_;
    Point p2_;
    Point p3_;
    Line l1_;
    Line l2_;
    Line l3_;

    // Функции для определения пересечения в 2D
    bool is_intersectAABB(const Triangle& anotherTriangle, planes_t projPlane);
    bool is_intersectSAT(const Triangle& anotherTriangle, planes_t projPlane);
    bool intersect2D(Triangle& anotherTriangle);

    // Функции для определения принадлежности
    bool containsPoint(const Point& p);
    bool is_line() const;
    bool intersect3D(Triangle& anotherTriangle);


public:
    Triangle() : p1_(), p2_(), p3_(), l1_(), l2_(), l3_() {}

    Triangle(Point& point1, Point& point2, Point& point3) : 
        p1_(point1), 
        p2_(point2), 
        p3_(point3),
        l1_(p1_, p2_),
        l2_(p2_, p3_),
        l3_(p3_, p1_) {}

    Triangle(Point&& point1, Point&& point2, Point&& point3) : 
        p1_(std::move(point1)), 
        p2_(std::move(point2)), 
        p3_(std::move(point3)),
        l1_(p1_, p2_),
        l2_(p2_, p3_),
        l3_(p3_, p1_) {}

    bool is_parallelTo(Triangle& anotherTriangle);

    bool is_intersect(Triangle& anotherTriangle);

    friend std::ostream& operator<<(std::ostream& os, const Triangle& triangle)
    {
        os << "Triangle(" << triangle.p1_ << ", " << triangle.p2_ << ", " << triangle.p3_ << ")";
        return os;
    }

    const Point& getPoint(size_t index) const
    {
        switch (index)
        {
        case 0: return p1_;
        case 1: return p2_;
        case 2: return p3_;
        default: throw std::out_of_range("Invalid point index");
        }
    }
};

} // namespace Geo

#endif // GEO_H