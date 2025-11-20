#pragma once

#include "geo_point.hpp"
#include "geo_vector.hpp"

#include <iostream>

namespace Geo
{

template<typename T>
class Line
{
    // canonical definition
    // (x - p_x) / basis_x = (y - p_y) / basis_y = (z - p_z) / basis_z
    Point<T> p_;
    Vector<T> basis_;

public:
    Line() = default;

    Line(Point<T>& point1, Point<T>& point2) :
        p_(point1),
        basis_(point2.x_ - point1.x_, point2.y_ - point1.y_, point2.z_ - point1.z_) {}

    Line(const Point<T>& point1, const Point<T>& point2) :
        p_(point1),
        basis_(point2.x_ - point1.x_, point2.y_ - point1.y_, point2.z_ - point1.z_) {}

    Line(Vector<T>& basis, Point<T>&& point) : p_(point), basis_(basis) {}
    
    Line(Vector<T>&& basis, const Point<T>& point) : p_(point), basis_(basis) {}

    /// @brief Find intersection of a this line with plane
    /// @param p1 First plane point
    /// @param p2 Second plane point
    /// @param p3 Third plane point
    /// @return Intersection point
    Point<T>
    intersect(const Point<T>& p1, const Point<T>& p2, const Point<T>& p3) const
    {
        // calculate the normal of the plane: (p2 - p1) x (p3 - p1)
        Vector<T> v1{p2.x_ - p1.x_, p2.y_ - p1.y_, p2.z_ - p1.z_};
        Vector<T> v2{p3.x_ - p1.x_, p3.y_ - p1.y_, p3.z_ - p1.z_};
        Vector<T> normal = v1.multiply_vectorially_by(v2);

        // checking the parallelism of lines and planes
        T denom = normal.multiply_scalar_by(basis_);
        if (is_z(denom)) {
            return Point<T>{NAN, NAN, NAN};
        }

        // d = -normal.dot(p1)
        Vector<T> p1_vec{p1.x_, p1.y_, p1.z_};
        T d = -normal.multiply_scalar_by(p1_vec);

        // t = -(normal.dot(p_) + d) / (normal.dot(basis_))
        Vector<T> p_vec{p_.x_, p_.y_, p_.z_};
        T t = -(normal.multiply_scalar_by(p_vec) + d) / denom;

        // calculate the intersection point: p_ + t * basis_
        return Point<T>{
            p_.x_ + t * basis_.xProj_,
            p_.y_ + t * basis_.yProj_,
            p_.z_ + t * basis_.zProj_
        };
    }
    
    /// @brief Find intersection of a this line with a line
    /// @return Intersection point
    Point<T>
    intersect(const Line<T>& anotherLine) const
    {
        if (this->is_parallelTo(anotherLine)) {
            return Point<T>(NAN, NAN, NAN);
        }
        
        T x1 = p_.x_;
        T y1 = p_.y_;
        T z1 = p_.z_;
        T a1 = basis_.xProj_;
        T b1 = basis_.yProj_;
        T c1 = basis_.zProj_;
        
        T x2 = anotherLine.p_.x_;
        T y2 = anotherLine.p_.y_;
        T z2 = anotherLine.p_.z_;
        T a2 = anotherLine.basis_.xProj_;
        T b2 = anotherLine.basis_.yProj_;
        T c2 = anotherLine.basis_.zProj_;
        
        // try xy-plane
        T det_xy = a1 * (-b2) - (-a2) * b1;
        if (!is_z(det_xy)) {
            // a1*t - a2*s = dx, dx = x2 - x1
            // b1*t - b2*s = dy, dy = y2 - y1
            
            T dx = x2 - x1;
            T dy = y2 - y1;
            
            T det_main = a1 * (-b2) - (-a2) * b1;
            T det_t = dx * (-b2) - (-a2) * dy;
            T det_s = a1 * dy - dx * b1;
            
            if (!is_z(det_main)) {
                T t = det_t / det_main;
                T s = det_s / det_main;
                
                // check z-coordinate
                T z_intersect1 = z1 + t * c1;
                T z_intersect2 = z2 + s * c2;
                
                if (is_z(z_intersect1 - z_intersect2)) {
                    return Geo::Point<T>(x1 + t * a1, y1 + t * b1, z_intersect1);
                }
            }
        }
        
        // try xz-plane
        T det_xz = a1 * (-c2) - (-a2) * c1;
        if (!is_z(det_xz)) {
            // a1*t - a2*s = dx, dx = x2 - x1
            // c1*t - c2*s = dz, dz = z2 - z1
            
            T dx = x2 - x1;
            T dz = z2 - z1;
            
            T det_main = a1 * (-c2) - (-a2) * c1;
            T det_t = dx * (-c2) - (-a2) * dz;
            T det_s = a1 * dz - dx * c1;
            
            if (!is_z(det_main)) {
                T t = det_t / det_main;
                T s = det_s / det_main;
                
                // check y-coordinate
                T y_intersect1 = y1 + t * b1;
                T y_intersect2 = y2 + s * b2;
                
                if (is_z(y_intersect1 - y_intersect2)) {
                    return Geo::Point<T>(x1 + t * a1, y_intersect1, z1 + t * c1);
                }
            }
        }
        
        // try yz-plane
        T det_yz = b1 * (-c2) - (-b2) * c1;
        if (!is_z(det_yz)) {
            // b1*t - b2*s = dy dy = y2 - y1
            // c1*t - c2*s = dz dz = z2 - z1
            
            T dy = y2 - y1;
            T dz = z2 - z1;
            
            T det_main = b1 * (-c2) - (-b2) * c1;
            T det_t = dy * (-c2) - (-b2) * dz;
            T det_s = b1 * dz - dy * c1;
            
            if (!is_z(det_main)) {
                T t = det_t / det_main;
                T s = det_s / det_main;
                
                // check x-coordinate
                T x_intersect1 = x1 + t * a1;
                T x_intersect2 = x2 + s * a2;
                
                if (is_z(x_intersect1 - x_intersect2)) {
                    return Geo::Point<T>(x_intersect1, y1 + t * b1, z1 + t * c1);
                }
            }
        }
        
        return Geo::Point<T>(NAN, NAN, NAN);
    }

    /// @brief Check if a this line is parallel to another line
    bool
    is_parallelTo(const Line<T>& anotherLine) const
    {
        return basis_.is_parallel(anotherLine.basis_);
    }

    /// @brief Return basis vector of a this line
    const Vector<T>&
    getBasis() const { return basis_; }

    /// @brief Return point of a this line
    const Point<T>&
    getPoint() const { return p_; }
};

template<typename T> std::ostream&
operator<<(std::ostream& os, const Line<T>& line)
{
    os << "Line(" << line.getPoint() << ", " << line.getBasis() << ")";
    return os;
}

} // namespace Geo
