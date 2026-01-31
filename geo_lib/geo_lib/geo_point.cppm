module;


import geo_comparator;

#include <iostream>
#include <cmath>

export module geo_point;

export namespace Geo
{

template<typename T>
class Point final
{
    T x_;
    T y_;
    T z_;

public:

    Point() : x_(0.0), y_(0.0), z_(0.0) {}
    explicit Point(T x, T y, T z) : x_(x), y_(y), z_(z) {}

    void setX(T x){ x_ = x; }
    void setY(T y) { y_ = y; }
    void setZ(T z) { z_ = z; }

    T getX() const { return x_; }
    T getY() const { return y_; }
    T getZ() const { return z_; }

    /// @brief 0 = get(x), 1 = get(y), 2 = get(z)
    T operator[](int index) const
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


/// @brief Check if this point between two points
/// @param verificationPoint Point to check
/// @param p1 First segment point
/// @param p2 Second segment point
/// @return True if point is among, false - otherwise
template<typename T>
bool
isAmong(const Point<T>& verificationPoint, const Point<T>& p1, const Point<T>& p2)
{
    if (std::isnan(verificationPoint[0]) ||
        std::isnan(verificationPoint[1]) ||
        std::isnan(verificationPoint[2]))
    {
        return false;
    }

    if (std::isnan(p1.getX()) || std::isnan(p1.getY()) || std::isnan(p1.getZ()) ||
        std::isnan(p2.getX()) || std::isnan(p2.getY()) || std::isnan(p2.getZ()))
    {
        return false;
    }

    // If a point coincides with one of the ends of a segment, it lies on it
    if (isEqual(verificationPoint, p1) || isEqual(verificationPoint, p2))
    {
        return true;
    }

    if (isEqual(p1, p2))
    {
        return false;
    }

    // Vector p1p (from the beginning of the segment to our point)
    T vx1 = verificationPoint[0] - p1[0];
    T vy1 = verificationPoint[1] - p1[1];
    T vz1 = verificationPoint[2] - p1[2];

    // Vector p1p2 (entire segment)
    T vx2 = p2[0] - p1[0];
    T vy2 = p2[1] - p1[1];
    T vz2 = p2[2] - p1[2];

    // If it is not equal to zero, the vectors are not collinear
    T crossX = vy1 * vz2 - vz1 * vy2;
    T crossY = vz1 * vx2 - vx1 * vz2;
    T crossZ = vx1 * vy2 - vy1 * vx2;

    if (!is_z(crossX) || !is_z(crossY) || !is_z(crossZ))
    {
        return false;
    }

    T dot = vx1 * vx2 + vy1 * vy2 + vz1 * vz2;
    if (is_bz(dot))
    {
        return false;
    }

    T lenSq = vx2 * vx2 + vy2 * vy2 + vz2 * vz2;
    if (is_az(dot - lenSq))
    {
        return false;
    }

    return true;
}


/// @brief Check if this point is equal to point
/// @param p1 First point
/// @param p2 Second point
/// @return True if points are equal, false - otherwise
template<typename T>
bool
isEqual(const Point<T>& p1, const Point<T>& p2)
{
    T xdiff = p1[0] - p2[0];
    T ydiff = p1[1] - p2[1];
    T zdiff = p1[2] - p2[2];
    return is_z(xdiff, ydiff, zdiff);
}


/// @brief Print Point container in format "Point(x, y, z)"
template<typename T> std::ostream&
operator<<(std::ostream& os, const Geo::Point<T>& point)
{
    os << "Point(" << point.x_ << ", " << point.y_ << ", " << point.z_ << ")";
    return os;
}

} // namespace Geo


