#ifndef GEO_H
#define GEO_H

#include "double.h"

#include <cmath>
#include <vector>
#include <ostream>

namespace Geo {

    namespace detail {
    enum class planes_t;
}


struct Point
{
    double x_;
    double y_;
    double z_;
    
    Point() : x_(0.0), y_(0.0), z_(0.0) {}
    Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    bool is_among(const Point& p1, const Point& p2) const
    {
        // Если точка совпадает с одним из концов отрезка, она на нем лежит.
        if (this->is_equalTo(p1) || this->is_equalTo(p2))
            return true;

        if (p1.is_equalTo(p2))
            return false;

        // Вектор p1p (от начала отрезка до нашей точки)
        double vx1 = x_ - p1.x_;
        double vy1 = y_ - p1.y_;
        double vz1 = z_ - p1.z_;

        // Вектор p1p2 (весь отрезок)
        double vx2 = p2.x_ - p1.x_;
        double vy2 = p2.y_ - p1.y_;
        double vz2 = p2.z_ - p1.z_;

        // Проверяем коллинеарность через векторное произведение.
        // Если оно не равно нулю, векторы не коллинеарны.
        double crossX = vy1 * vz2 - vz1 * vy2;
        double crossY = vz1 * vx2 - vx1 * vz2;
        double crossZ = vx1 * vy2 - vy1 * vx2;
        if (!is_doubleZero(crossX) || !is_doubleZero(crossY) || !is_doubleZero(crossZ))
            return false;

        // Проверяем, что точка лежит внутри отрезка, а не на продолжении прямой.
        // Скалярное произведение векторов p1p и p1p2 должно быть в диапазоне
        // от 0 до квадрата длины p1p2.
        double dot = vx1 * vx2 + vy1 * vy2 + vz1 * vz2;
        if (dot < 0) // Точка находится "позади" p1
            return false;

        double lenSq = vx2 * vx2 + vy2 * vy2 + vz2 * vz2;
        if (dot > lenSq) // Точка находится "дальше" p2
            return false;

        return true;
    }


    bool is_equalTo(const Point& anotherPoint) const
    {
        double xdiff = x_ - anotherPoint.x_;
        double ydiff = y_ - anotherPoint.y_;
        double zdiff = z_ - anotherPoint.z_;

        return is_doubleZero(xdiff, ydiff, zdiff);
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& point)
    {
        os << "Point(" << point.x_ << ", " << point.y_ << ", " << point.z_ << ")";
        return os;
    }

};

struct GeoVector
{
    double xProj_;
    double yProj_;
    double zProj_;

    GeoVector(const Point& p1, const Point& p2)
    {
        xProj_ = p2.x_ - p1.x_;
        yProj_ = p2.y_ - p1.y_;
        zProj_ = p2.z_ - p1.z_;
    }

    GeoVector(double xProjection, double yProjection, double zProjection) : 
        xProj_(xProjection),
        yProj_(yProjection),
        zProj_(zProjection) {}
    
    GeoVector() : xProj_(0.0), yProj_(0.0), zProj_(0.0) {}

    bool is_zero()
    {
        return is_doubleZero(xProj_, yProj_, zProj_);
    }

    bool is_parallel(const GeoVector& anotherVector) const
    {
        double crossX = yProj_ * anotherVector.zProj_ - zProj_ * anotherVector.yProj_;
        double crossY = zProj_ * anotherVector.xProj_ - xProj_ * anotherVector.zProj_;
        double crossZ = xProj_ * anotherVector.yProj_ - yProj_ * anotherVector.xProj_;

        return is_doubleZero(crossX, crossY, crossZ);
    }

    GeoVector multiply_vectorially_by(const GeoVector& anotherVector) const
    {
        return GeoVector(
            yProj_ * anotherVector.zProj_ - zProj_ * anotherVector.yProj_,
            zProj_ * anotherVector.xProj_ - xProj_ * anotherVector.zProj_,
            xProj_ * anotherVector.yProj_ - yProj_ * anotherVector.xProj_
        );
    }

    // Скалярное произведение вектора на вектор
    double multiply_scalar_by(const GeoVector& anotherVector) const
    {
        return xProj_ * anotherVector.xProj_ +
               yProj_ * anotherVector.yProj_ +
               zProj_ * anotherVector.zProj_;
    }

    // Скалярное произведение вектора на радиус-вектор точки
    double multiply_scalar_by(const Point& p) const
    {
        return xProj_ * p.x_ + yProj_ * p.y_ + zProj_ * p.z_;
    }

    friend std::ostream& operator<<(std::ostream& os, const GeoVector& vec)
    {
        os << "GeoVector(" << vec.xProj_ << ", " << vec.yProj_ << ", " << vec.zProj_ << ")";
        return os;
    }
};

class Line
{
    // canonical definition
    // (x - p_x) / basis_x = (y - p_y) / basis_y = (z - p_z) / basis_z
    Point p_;
    GeoVector basis_;


public:
    Line() = default;
    Line(Point& point1, Point& point2) :
        p_(point1),
        basis_(point2.x_ - point1.x_, point2.y_ - point1.y_, point2.z_ - point1.z_) {}

    Line(const Point& point1, const Point& point2) :
        p_(point1),
        basis_(point2.x_ - point1.x_, point2.y_ - point1.y_, point2.z_ - point1.z_) {}

    Line(GeoVector& basis, Point&& point) : p_(point), basis_(basis) {}
    
    Line(GeoVector&& basis, const Point& point) : p_(point), basis_(basis) {}

    Point intersect(const Point& p1, const Point& p2, const Point& p3) const;
    
    Point intersect(const Line& anotherLine) const;

    bool is_parallelTo(const Line& anotherLine) const;

    const GeoVector& getBasis() const
    {
        return basis_;
    }

    const Point& getPoint() const
    {
        return p_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Line& line)
    {
        os << "Line(" << line.p_ << ", " << line.basis_ << ")";
        return os;
    }
};



class Triangle
{
    Point p1_;
    Point p2_;
    Point p3_;
    Line l1_;
    Line l2_;
    Line l3_;
    
    Line findIntersectLine(Triangle& anotherTriangle);

    bool intersect3D(Triangle& anotherTriangle);
    bool intersect2D(Triangle& anotherTriangle);
    bool containsPoint(const Point& p);
    bool isOnEdge(const Point& p);
    bool is_intersectAABB(const Triangle& anotherTriangle, detail::planes_t projPlane);
    bool is_intersectSAT(const Triangle& anotherTriangle, detail::planes_t projPlane);
    bool is_line() const;


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
        

    // TODO cocnvert from lines to points,  back
    // Triangle(Line& line1, Line& line2, Line& line3) : l1_(line1), l2_(line2), l3_(line3) {}

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
        case 1: return p1_;
        case 2: return p2_;
        case 3: return p3_;
        default: throw std::out_of_range("Invalid point index");
        }
    }
};

} // namespace Geo

#endif // GEO_H