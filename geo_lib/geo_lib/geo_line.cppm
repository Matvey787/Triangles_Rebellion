module;

import geo_point;
import geo_vector;
import geo_comparator;

#include <cmath>

#include <iostream>

export module geo_line;

export namespace Geo
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

    Line(const Point<T>& p1, const Point<T>& p2) :
        p_(p1),
        basis_(p1, p2) {}

    Line(const Vector<T>& basis, const Point<T>& point) :
        p_(point), basis_(basis) {}

    Line(const Point<T>& point, const Vector<T>& basis) :
        p_(point), basis_(basis) {}

    Line(Vector<T>&& basis, Point<T>&& point) :
        p_(point), basis_(basis) {}

    /// @brief Return basis vector of a this line
    const Vector<T>&
    getBasis() const { return basis_; }

    /// @brief Return point of a this line
    const Point<T>&
    getPoint() const { return p_; }
};




/// @brief Find intersection of a this line with plane
/// @tparam T floating point type
/// @param line Line to intersect with plane
/// @param p1 First plane point
/// @param p2 Second plane point
/// @param p3 Third plane point
/// @return Intersection point
template<typename T>
Point<T>
intersect(const Line<T>& l, const Point<T>& p1, const Point<T>& p2, const Point<T>& p3)
{
    // calculate the normal of the plane: (p2 - p1) x (p3 - p1)
    Vector<T> v1(p1, p2);
    Vector<T> v2(p1, p3);
    Vector<T> normal = cross(v1, v2);

    // checking the parallelism of lines and planes
    T denom = scalar(normal, l.getBasis());

    if (is_z(denom))
    {
        return Geo::Point<T>{NAN, NAN, NAN};
    }

    // d = -normal.dot(p1)
    T d = scalar(-normal, p1);

    // t = -(normal.dot(p_) + d) / (normal.dot(basis_))
    T t = -(scalar(normal, l.getPoint()) + d) / denom;

    const auto& lPoint = l.getPoint();
    const auto& lBasis = l.getBasis();

    // calculate the intersection point: p_ + t * basis_
    return Geo::Point<T>{
        lPoint.getX() + t * lBasis.getXProj(),
        lPoint.getY() + t * lBasis.getYProj(),
        lPoint.getZ() + t * lBasis.getZProj()
    };
}




/// @brief Find lines intersection
/// @tparam T floating point type
/// @param l1 First line
/// @param l2 Second line
/// @return Intersection point
template <typename T>
Point<T>
intersect(const Line<T>& l1, const Line<T>& l2)
{
    if (isParallel(l1, l2)) {
        return Point<T>(NAN, NAN, NAN);
    }
    const auto& l1Point = l1.getPoint();
    const auto& l1Basis = l1.getBasis();
    const auto& l2Point = l2.getPoint();
    const auto& l2Basis = l2.getBasis();

    T x1 = l1Point.getX();
    T y1 = l1Point.getY();
    T z1 = l1Point.getZ();
    T a1 = l1Basis.getXProj();
    T b1 = l1Basis.getYProj();
    T c1 = l1Basis.getZProj();
    
    T x2 = l2Point.getX();
    T y2 = l2Point.getY();
    T z2 = l2Point.getZ();
    T a2 = l2Basis.getXProj();
    T b2 = l2Basis.getYProj();
    T c2 = l2Basis.getZProj();
    
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
/// @tparam T floating point type
/// @param l1 First line
/// @param l2 Second line
/// @return True if lines are parallel, false - otherwise
template<typename T>
bool
isParallel(const Line<T>& l1, const Line<T>& l2)
{
    return l1.getBasis().is_parallel(l2.getBasis());
}




template<typename T> std::ostream&
operator<<(std::ostream& os, const Line<T>& line)
{
    os << "Line(" << line.getPoint() << ", " << line.getBasis() << ")";
    return os;
}

} // namespace Geo
