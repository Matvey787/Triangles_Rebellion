#pragma once

#include "double.h"

#include <cmath>
#include <iostream>

namespace Geo::detail
{

enum class planes_t { XY, XZ, YZ };
    
struct Point
{
    double x_;
    double y_;
    double z_;
    
    Point() : x_(0.0), y_(0.0), z_(0.0) {}
    Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    bool is_among(const Point& p1, const Point& p2) const
    {
        if (std::isnan(x_) || std::isnan(y_) || std::isnan(z_))
            return false;
        if (std::isnan(p1.x_) || std::isnan(p1.y_) || std::isnan(p1.z_) ||
            std::isnan(p2.x_) || std::isnan(p2.y_) || std::isnan(p2.z_))
            return false;
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

        // Если оно не равно нулю, векторы не коллинеарны.
        double crossX = vy1 * vz2 - vz1 * vy2;
        double crossY = vz1 * vx2 - vx1 * vz2;
        double crossZ = vx1 * vy2 - vy1 * vx2;
        if (!is_z(crossX) || !is_z(crossY) || !is_z(crossZ))
            return false;

        double dot = vx1 * vx2 + vy1 * vy2 + vz1 * vz2;
        if (is_bz(dot))
            return false;

        double lenSq = vx2 * vx2 + vy2 * vy2 + vz2 * vz2;
        if (is_az(dot - lenSq))
            return false;

        return true;
    }


    bool is_equalTo(const Point& anotherPoint) const
    {
        double xdiff = x_ - anotherPoint.x_;
        double ydiff = y_ - anotherPoint.y_;
        double zdiff = z_ - anotherPoint.z_;

        return is_z(xdiff, ydiff, zdiff);
    }

    friend std::ostream& operator<<(std::ostream& os, const Point& point)
    {
        os << "Point(" << point.x_ << ", " << point.y_ << ", " << point.z_ << ")";
        return os;
    }

    double operator[](int index) const
    {
        switch (index)
        {
        case 0: return x_;
        case 1: return y_;
        case 2: return z_;
        default: throw std::out_of_range("Invalid index []");
        }
    }
}; // Point


/// @brief Класс, представляющий вектор в 3D пространстве
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
        return is_z(xProj_, yProj_, zProj_);
    }

    bool is_parallel(const GeoVector& anotherVector) const
    {
        double crossX = yProj_ * anotherVector.zProj_ - zProj_ * anotherVector.yProj_;
        double crossY = zProj_ * anotherVector.xProj_ - xProj_ * anotherVector.zProj_;
        double crossZ = xProj_ * anotherVector.yProj_ - yProj_ * anotherVector.xProj_;

        return is_z(crossX, crossY, crossZ);
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


/// @brief  Класс, представляющий прямую в 3D пространстве
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

    const GeoVector& getBasis() const { return basis_; }

    const Point& getPoint() const { return p_; }

    friend std::ostream& operator<<(std::ostream& os, const Line& line)
    {
        os << "Line(" << line.p_ << ", " << line.basis_ << ")";
        return os;
    }
};

Geo::detail::planes_t findProjectionPlane(const Geo::detail::GeoVector& normal);

} // namespace detail
