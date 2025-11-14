#pragma once

#include "double.hpp"

#include <iostream>
#include <cmath>

namespace Geo {

template<typename T>
class Point
{
public:
    T x_;
    T y_;
    T z_;

    Point() : x_(0.0), y_(0.0), z_(0.0) {}
    Point(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    /// @brief Check if this point between two points
    bool
    is_among(const Point<T>& p1, const Point<T>& p2) const
    {
        if (std::isnan(x_) || std::isnan(y_) || std::isnan(z_))
            return false;
        if (std::isnan(p1.x_) || std::isnan(p1.y_) || std::isnan(p1.z_) ||
            std::isnan(p2.x_) || std::isnan(p2.y_) || std::isnan(p2.z_))
            return false;
        // If a point coincides with one of the ends of a segment, it lies on it
        if (this->is_equalTo(p1) || this->is_equalTo(p2))
            return true;

        if (p1.is_equalTo(p2))
            return false;

        // Vector p1p (from the beginning of the segment to our point)
        double vx1 = x_ - p1.x_;
        double vy1 = y_ - p1.y_;
        double vz1 = z_ - p1.z_;

        // Vector p1p2 (entire segment)
        double vx2 = p2.x_ - p1.x_;
        double vy2 = p2.y_ - p1.y_;
        double vz2 = p2.z_ - p1.z_;

        // If it is not equal to zero, the vectors are not collinear
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


    /// @brief Check if this point is equal to point
    bool
    is_equalTo(const Point<T>& p) const
    {
        double xdiff = x_ - p[0];
        double ydiff = y_ - p[1];
        double zdiff = z_ - p[2];
        return is_z(xdiff, ydiff, zdiff);
    }

    /// @brief 0 = get(x), 1 = get(y), 2 = get(z)
    double
    operator[](int index) const
    {
        switch (index)
        {
        case 0: return x_;
        case 1: return y_;
        case 2: return z_;
        default: throw std::out_of_range("Invalid index Point[]");
        }
    }
};

/// @brief Print Point container in format "Point(x, y, z)"
template<typename T> std::ostream&
operator<<(std::ostream& os, const Geo::Point<T>& point)
{
    os << "Point(" << point.x_ << ", " << point.y_ << ", " << point.z_ << ")";
    return os;
}

} // namespace Geo


