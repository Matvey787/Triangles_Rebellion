#pragma once

#include "geo_point.hpp"
#include "double.hpp"

namespace Geo {

template<typename T>
class Vector
{
public:
    T xProj_;
    T yProj_;
    T zProj_;

    Vector(const Point<T>& p1, const Point<T>& p2)
    {
        xProj_ = p2.x_ - p1.x_;
        yProj_ = p2.y_ - p1.y_;
        zProj_ = p2.z_ - p1.z_;
    }

    Vector(T xProjection, T yProjection, T zProjection) : 
        xProj_(xProjection),
        yProj_(yProjection),
        zProj_(zProjection) {}
    
    Vector() : xProj_(0.0), yProj_(0.0), zProj_(0.0) {}

    /// @brief Check if all projections is 0
    bool
    is_zero() const
    {
        return is_z(xProj_, yProj_, zProj_);
    }

    /// @brief Check if a this vector is parallel to a vector
    bool
    is_parallel(const Vector<T>& anotherVector) const
    {
        T crossX = yProj_ * anotherVector.zProj_ - zProj_ * anotherVector.yProj_;
        T crossY = zProj_ * anotherVector.xProj_ - xProj_ * anotherVector.zProj_;
        T crossZ = xProj_ * anotherVector.yProj_ - yProj_ * anotherVector.xProj_;

        return is_z(crossX, crossY, crossZ);
    }

    /// @brief Vector product of a this vector and a vector
    Vector<T>
    multiply_vectorially_by(const Vector<T>& anotherVector) const
        {
        return Vector(
            yProj_ * anotherVector.zProj_ - zProj_ * anotherVector.yProj_,
            zProj_ * anotherVector.xProj_ - xProj_ * anotherVector.zProj_,
            xProj_ * anotherVector.yProj_ - yProj_ * anotherVector.xProj_
        );
    }

    /// @brief Scalar product of a this vector and a vector
    T
    multiply_scalar_by(const Vector<T>& anotherVector) const
    {
        return xProj_ * anotherVector.xProj_ +
                yProj_ * anotherVector.yProj_ +
                zProj_ * anotherVector.zProj_;
    }


    // The scalar product of a this vector and the radius vector of a point
    T
    multiply_scalar_by(const Point<T>& p) const
    {
        return xProj_ * p.x_ + yProj_ * p.y_ + zProj_ * p.z_;
    }
    
};

/// @brief Print Vector container in format "Vector(xProj, yProj, zProj)"
template<typename T> std::ostream&
operator<<(std::ostream& os, const Vector<T>& vec)
{
    os << "Vector(" << vec.xProj_ << ", " << vec.yProj_ << ", " << vec.zProj_ << ")";
    return os;
}

} // namespace Geo