module;

import geo_point;
import geo_comparator;

#include <iostream>

export module geo_vector;

export namespace Geo
{

template<typename T>
class Vector
{
    T xProj_;
    T yProj_;
    T zProj_;

public:

    Vector(const Point<T>& p1, const Point<T>& p2)
    {
        xProj_ = p2.getX() - p1.getX();
        yProj_ = p2.getY() - p1.getY();
        zProj_ = p2.getZ() - p1.getZ();
    }

    Vector(T xProjection, T yProjection, T zProjection) : 
        xProj_(xProjection),
        yProj_(yProjection),
        zProj_(zProjection) {}
    
    Vector() : xProj_(0.0), yProj_(0.0), zProj_(0.0) {}

    T getXProj() const { return xProj_; }
    T getYProj() const { return yProj_; }
    T getZProj() const { return zProj_; }

    T operator[](int index) const
    {
        switch (index)
        {
        case 0: return xProj_;
        case 1: return yProj_;
        case 2: return zProj_;
        default: throw std::out_of_range("Invalid index Vector[]");
        }
    }

    Vector operator-() const
    {
        return Vector(-xProj_, -yProj_, -zProj_);
    }


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
};

/// @brief Vector product of a this vector and a vector
/// @tparam T floating point type
/// @param v1 First vector
/// @param v2 Second vector
/// @return Cross vector product
template<typename T>
Vector<T>
cross(const Vector<T>& v1, const Vector<T>& v2)
{
    return Vector<T>(
        v1.getYProj() * v2.getZProj() - v1.getZProj() * v2.getYProj(),
        v1.getZProj() * v2.getXProj() - v1.getXProj() * v2.getZProj(),
        v1.getXProj() * v2.getYProj() - v1.getYProj() * v2.getXProj()
    );
}

/// @brief Cross product of two vectors
/// @tparam T floating point type
/// @param v First vector
/// @param p Point like radius vector from (0, 0, 0)
/// @return Cross vector product
template<typename T>
Vector<T>
cross(const Vector<T>& v, const Point<T>& p)
{
    return Vector<T>(
        v.getYProj() * p.getZ() - v.getZProj() * p.getY(),
        v.getZProj() * p.getX() - v.getXProj() * p.getZ(),
        v.getXProj() * p.getY() - v.getYProj() * p.getX()
    );
}

/// @brief Scalar product of two vectors
/// @tparam T floating point type
/// @param v1 First vector
/// @param v2 Second vector
/// @return Scalar product
template<typename T>
T
scalar(const Vector<T>& v1, const Vector<T>& v2)
{
    return v1.getXProj() * v2.getXProj() +
           v1.getYProj() * v2.getYProj() +
           v1.getZProj() * v2.getZProj();
}

/// @brief Scalar product of two vectors
/// @tparam T floating point type
/// @param v First vector
/// @param p Point like radius vector from (0, 0, 0)
/// @return Scalar product
template<typename T>
T
scalar(const Vector<T>& v, const Point<T>& p)
{
    return v.getXProj() * p.getX() +
           v.getYProj() * p.getY() +
           v.getZProj() * p.getZ();
}


/// @brief Print Vector container in format "Vector(xProj, yProj, zProj)"
template<typename T> std::ostream&
operator<<(std::ostream& os, const Vector<T>& vec)
{
    os << "Vector(" << vec.xProj_ << ", " << vec.yProj_ << ", " << vec.zProj_ << ")";
    return os;
}

} // namespace Geo