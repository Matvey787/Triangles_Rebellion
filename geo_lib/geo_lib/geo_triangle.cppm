module;

import geo_point;
import geo_line;
import geo_comparator;
import geo_vector;

#include <cmath>
#include <vector>
#include <ostream>
#include <algorithm>
#include <iostream>

export module geo_triangle;

export namespace Geo
{

template<typename T>
class Triangle final
{
    Point<T> p1_;
    Point<T> p2_;
    Point<T> p3_;
    Line<T> l1_;
    Line<T> l2_;
    Line<T> l3_;

public:
    Triangle() : p1_(), p2_(), p3_(), l1_(), l2_(), l3_() {}

    Triangle(Point<T>& point1, Point<T>& point2, Point<T>& point3) : 
        p1_(point1), 
        p2_(point2), 
        p3_(point3),
        l1_(p1_, p2_),
        l2_(p2_, p3_),
        l3_(p3_, p1_) {}

    Triangle(Point<T>&& point1, Point<T>&& point2, Point<T>&& point3) : 
        p1_(std::move(point1)), 
        p2_(std::move(point2)), 
        p3_(std::move(point3)),
        l1_(p1_, p2_),
        l2_(p2_, p3_),
        l3_(p3_, p1_) {}

    /// @brief 0 = get(first point), 1 = get(second point), 2 = get(third point)
    const Point<T>&
    getPoint(const size_t index) const
    {
        switch (index)
        {
        case 0: return p1_;
        case 1: return p2_;
        case 2: return p3_;
        default: throw std::out_of_range("Invalid point index");
        }
    }

    const Line<T>&
    getLine(const size_t index) const
    {
        switch (index)
        {
        case 0: return l1_;
        case 1: return l2_;
        case 2: return l3_;
        default: throw std::out_of_range("Invalid line index");
        }
    }

    // FIXME Just for old tests compatibility (gtests). Remove later.
    bool
    is_intersect(Triangle<T>& anotherTriangle)
    {
        return isIntersect(*this, anotherTriangle);
    }
};




















/// @brief Check intersection of a this triangle with a triangle
/// @tparam T floating point type
/// @param t1 First triangle
/// @param t2 Second triangle
/// @return true if intersection detected, false - otherwise
// Forward declarations for functions defined later in this file
template <typename T>
bool
isIntersect(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2);




/// @brief Print triangle in format "Triangle(p1, p2, p3)"
template <typename T>
std::ostream&
operator<<(std::ostream& os, const Triangle<T>& triangle);

} // namespace Geo



enum class planes_t { XY, XZ, YZ };


/// @brief Find a normal vector of the triangle plane
/// @tparam T floating point type
/// @param t Triangle
/// @return Normal vector
template<typename T>
Geo::Vector<T>
getNormal(const Geo::Triangle<T>& t);




/// @brief Check intersection of two triangles
/// @tparam T floating point type
/// @param t1 First triangle
/// @param t2 Second triangle
/// @return True if triangles intersect, false - otherwise
template <typename T>
bool
intersect2D(const Geo::Triangle<T> t1, const Geo::Triangle<T>& t2);




/// @brief Check intersection of two triangles in 3D
/// @tparam T floating point type
/// @param t1 First triangle
/// @param t2 Second triangle
/// @return True if triangles intersect, false - otherwise
template<typename T>
bool
intersect3D(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2);




/// @brief Check intersection of two triangles using Separating Axis Theorem (strong intersection )
/// @tparam T floating point type
/// @param t1 First triangle 
/// @param t2 Second triangle 
/// @return True if triangles intersect, false - otherwise
template <typename T>
bool
is_intersectSAT(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2);




/// @brief Check intersection of AABBs of two triangles (fast pre-check)
/// @tparam T floating point type
/// @param t1 First triangle
/// @param t2 Second triangle
/// @return True if AABBs intersect, false - otherwise
template<typename T>
bool
is_intersectAABB(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2);




/// @brief Calculate the sign of the determinant formed by vectors from point p to the triangle 
/// vertices
/// @tparam T floating point type
/// @param plane_p1 First triangle vertex
/// @param plane_p2 Second triangle vertex
/// @param plane_p3 Third triangle vertex
/// @param p Point to test
/// @return Sign of the determinant
template<typename T>
T
signDeterminant(const Geo::Point<T>& plane_p1,
                const Geo::Point<T>& plane_p2,
                const Geo::Point<T>& plane_p3,
                const Geo::Point<T>& p);




/// @brief Find the best projection plane for a triangle normal
/// @tparam T floating point type
/// @param normal Normal vector
/// @return Projection plane
template<typename T>
planes_t
findProjectionPlane(const Geo::Vector<T>& normal);





/// @brief Computes the line parameter for orthogonal point projection.
/// For a line L(t) = P₀ + t·v, returns t such that (P₀ + t·v) is the closest point on L to the
/// point (orthogonal projection).
/// @tparam T floating point type
/// @param point Point to project
/// @param line Line to project onto
/// @return Scalar t: projection parameter along the line.
template <typename T>
T
getPointProj(const Geo::Point<T>& point, const Geo::Line<T>& line);




/// @brief Checks whether a triangle is degenerate - line
/// @tparam T floating point type
/// @param t Triangle to check
/// @return True if triangle is a line, false - otherwise
template<typename T>
bool
isLine(const Geo::Triangle<T>& t);




/// @brief Check if a point belongs to a triangle
/// @tparam T floating point type
/// @param p Point to check
/// @param t Parent triangle
/// @return True if point belongs to triangle, false - otherwise
template<typename T>
bool
isPointBelongsTriangle(const Geo::Point<T>& p, const Geo::Triangle<T>& t);




/// @brief Check if triangles are parallel  
/// @tparam T floating point type
/// @param t1 First triangle
/// @param t2 Second triangle
/// @return True if triangles are parallel, false - otherwise
template<typename T>
bool
isParallel(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2);




















template<typename T>
Geo::Vector<T>
getNormal(const Geo::Triangle<T>& t)
{
    Geo::Vector<T> v1(t.getPoint(0), t.getPoint(1));
    Geo::Vector<T> v2(t.getPoint(0), t.getPoint(2));

    auto normal = cross(v1, v2);

    return normal;
}


template <typename T>
bool
intersect2D(const Geo::Triangle<T> t1, const Geo::Triangle<T>& t2)
{
    if (!is_intersectAABB(t1, t2)) return false;

    if (!is_intersectSAT(t1, t2)) return false;

    return true;
}


template<typename T>
bool
intersect3D(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2)
{
    const auto& p1_t1 = t1.getPoint(0);
    const auto& p2_t1 = t1.getPoint(1);
    const auto& p3_t1 = t1.getPoint(2);

    const auto& p1_t2 = t2.getPoint(0);
    const auto& p2_t2 = t2.getPoint(1);
    const auto& p3_t2 = t2.getPoint(2);

    auto intersec_p1_t2 = intersect(t1.getLine(0), p1_t2, p2_t2, p3_t2);
    auto intersec_p2_t2 = intersect(t1.getLine(1), p1_t2, p2_t2, p3_t2);
    auto intersec_p3_t2 = intersect(t1.getLine(2), p1_t2, p2_t2, p3_t2);

    if ((isAmong(intersec_p1_t2, p1_t1, p2_t1) && isPointBelongsTriangle(intersec_p1_t2, t2)) ||
        (isAmong(intersec_p2_t2, p2_t1, p3_t1) && isPointBelongsTriangle(intersec_p2_t2, t2)) ||
        (isAmong(intersec_p3_t2, p1_t1, p3_t1) && isPointBelongsTriangle(intersec_p3_t2, t2)))
    {
        return true;
    }

    auto intersec_p1_t1 = intersect(t2.getLine(0), p1_t1, t1.getPoint(1), t1.getPoint(2));
    auto intersec_p2_t1 = intersect(t2.getLine(1), p1_t1, t1.getPoint(1), t1.getPoint(2));
    auto intersec_p3_t1 = intersect(t2.getLine(2), p1_t1, t1.getPoint(1), t1.getPoint(2));

    if ((isAmong(intersec_p1_t1, p1_t2, p2_t2) && isPointBelongsTriangle(intersec_p1_t1, t1)) ||
        (isAmong(intersec_p2_t1, p2_t2, p3_t2) && isPointBelongsTriangle(intersec_p2_t1, t1)) ||
        (isAmong(intersec_p3_t1, p1_t2, p3_t2) && isPointBelongsTriangle(intersec_p3_t1, t1)))
    {
        return true;
    }

    return false;
}


template <typename T>
bool
is_intersectSAT(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2)
{
    auto normal = getNormal(t1);

    planes_t projPlane = findProjectionPlane(normal);

    const Geo::Line<T>* lines[2][3] = {
        {&t1.getLine(0), &t1.getLine(1), &t1.getLine(2)},
        {&t2.getLine(0), &t2.getLine(1), &t2.getLine(2)}
    };

    const Geo::Point<T>* points[2][3] = {
        {&t1.getPoint(0), &t1.getPoint(1), &t1.getPoint(2)}, 
        {&t2.getPoint(0), &t2.getPoint(1), &t2.getPoint(2)}
    };

    for (size_t tIdx = 0; tIdx < 2; ++tIdx)
    for (size_t pIdx = 0; pIdx < 3; ++pIdx)
    {
        const auto& currLine = *lines[tIdx][pIdx % 3];
        const auto& currLineBasis = currLine.getBasis();
        const auto& currLinePoint = currLine.getPoint();
        Geo::Line<T> normal;

        if (projPlane == planes_t::XY)
        {
            normal = Geo::Line<T>{Geo::Vector<T>(-currLineBasis.getYProj(), currLineBasis.getXProj(), 0.0), currLinePoint};
        }
        else if (projPlane == planes_t::XZ)
        {
            normal = Geo::Line<T>{Geo::Vector<T>(-currLineBasis.getZProj(), 0.0, currLineBasis.getXProj()), currLinePoint};
        }

        const auto& th_p1 = *points[tIdx][pIdx];
        const auto& th_p2 = *points[tIdx][(pIdx + 1) % 3];
        const auto& th_p3 = *points[tIdx][(pIdx + 2) % 3];

        const auto& intersec_p1_t2 = *points[(tIdx + 1) % 2][pIdx % 3];
        const auto& intersec_p2_t2 = *points[(tIdx + 1) % 2][(pIdx + 1) % 3];
        const auto& intersec_p3_t2 = *points[(tIdx + 1) % 2][(pIdx + 2) % 3];

        T th_p1_proj = getPointProj(th_p1, normal);
        T th_p2_proj = getPointProj(th_p2, normal);
        T th_p3_proj = getPointProj(th_p3, normal);

        T intersec_p1_t2_proj = getPointProj(intersec_p1_t2, normal);
        T intersec_p2_t2_proj = getPointProj(intersec_p2_t2, normal);
        T intersec_p3_t2_proj = getPointProj(intersec_p3_t2, normal);

        T th_max_proj = std::max({th_p1_proj, th_p2_proj, th_p3_proj});
        T th_min_proj = std::min({th_p1_proj, th_p2_proj, th_p3_proj});

        T an_max_proj = std::max({intersec_p1_t2_proj, intersec_p2_t2_proj, intersec_p3_t2_proj});
        T an_min_proj = std::min({intersec_p1_t2_proj, intersec_p2_t2_proj, intersec_p3_t2_proj});

        if (is_bz(th_max_proj - an_min_proj) || is_bz(an_max_proj - th_min_proj))
        {
            return false;
        }
    }
    return true;
}


template<typename T>
bool
is_intersectAABB(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2)
{
    Geo::Vector<T> normal = getNormal(t1);

    planes_t projPlane = findProjectionPlane(normal);

    size_t plainIdx1 = 0;
    size_t plainIdx2 = 0;


    switch (projPlane)
    {
        case planes_t::XY:
        {
            plainIdx1 = 0;
            plainIdx2 = 1;
            break;
        }
        case planes_t::XZ:
        {
            plainIdx1 = 0;
            plainIdx2 = 2;
            break;
        }
        case planes_t::YZ:
        {
            plainIdx1 = 1;
            plainIdx2 = 2;
            break;
        }
        default:
            throw std::invalid_argument("Invalid projection plane");
    };

    const auto& p1_t1 = t1.getPoint(0);
    const auto& p2_t1 = t1.getPoint(1);
    const auto& p3_t1 = t1.getPoint(2);

    const auto& p1_t2 = t2.getPoint(0);
    const auto& p2_t2 = t2.getPoint(1);
    const auto& p3_t2 = t2.getPoint(2);

    T t1_min1 = std::min({p1_t1[plainIdx1],
                          p2_t1[plainIdx1],
                          p3_t1[plainIdx1]});

    T t1_max1 = std::max({p1_t1[plainIdx1],
                          p2_t1[plainIdx1],
                          p3_t1[plainIdx1]});

    T t1_min2 = std::min({p1_t1[plainIdx2],
                          p2_t1[plainIdx2],
                          p3_t1[plainIdx2]});

    T t1_max2 = std::max({p1_t1[plainIdx2],
                          p2_t1[plainIdx2],
                          p3_t1[plainIdx2]});

    T t2_min1 = std::min({p1_t2[plainIdx1], 
                          p2_t2[plainIdx1],
                          p3_t2[plainIdx1]});

    T t2_max1 = std::max({p1_t2[plainIdx1],
                          p2_t2[plainIdx1],
                          p3_t2[plainIdx1]});

    T t2_min2 = std::min({p1_t2[plainIdx2],
                          p2_t2[plainIdx2],
                          p3_t2[plainIdx2]});

    T t2_max2 = std::max({p1_t2[plainIdx2],
                          p2_t2[plainIdx2],
                          p3_t2[plainIdx2]});

    if (is_bz(t1_max1 - t2_min1) || is_bz(t2_max1 - t1_min1) ||
        is_bz(t1_max2 - t2_min2) || is_bz(t2_max2 - t1_min2))
    {
        return false;
    }

    return true;
}


template<typename T>
T
signDeterminant(const Geo::Point<T>& plane_p1,
                const Geo::Point<T>& plane_p2,
                const Geo::Point<T>& plane_p3,
                const Geo::Point<T>& p)
{
    T v1x = plane_p1.getX() - p.getX();
    T v1y = plane_p1.getY() - p.getY();
    T v1z = plane_p1.getZ() - p.getZ();

    T v2x = plane_p2.getX() - p.getX();
    T v2y = plane_p2.getY() - p.getY();
    T v2z = plane_p2.getZ() - p.getZ();

    T v3x = plane_p3.getX() - p.getX();
    T v3y = plane_p3.getY() - p.getY();
    T v3z = plane_p3.getZ() - p.getZ();

    T det = v1x * (v2y * v3z - v2z * v3y) -
            v1y * (v2x * v3z - v2z * v3x) +
            v1z * (v2x * v3y - v2y * v3x);

    return det;
}


template<typename T>
planes_t
findProjectionPlane(const Geo::Vector<T>& normal)
{
    T ax = std::abs(normal.getXProj());
    T ay = std::abs(normal.getYProj());
    T az = std::abs(normal.getZProj());

    if (ax >= ay && ax >= az)
        return planes_t::YZ;
    else if (ay >= ax && ay >= az)
        return planes_t::XZ;
    else
        return planes_t::XY;
}


template <typename T>
T
getPointProj(const Geo::Point<T>& point, const Geo::Line<T>& line)
{
    const auto& linePoint = line.getPoint();
    const auto& lineBasis = line.getBasis();

    auto fromLinePointToPoint = Geo::Vector<T>(linePoint, point);

    T lineBasisLengthSq = scalar(lineBasis, lineBasis);

    T proj = scalar(fromLinePointToPoint, lineBasis) / lineBasisLengthSq;

    return proj;
}


template<typename T>
bool
isLine(const Geo::Triangle<T>& t)
{
    auto normal = getNormal(t);

    return normal.is_zero();
}


template<typename T>
bool
isPointBelongsTriangle(const Geo::Point<T>& p, const Geo::Triangle<T>& t)
{
    if (std::isnan(p.getX()) || std::isnan(p.getY()) || std::isnan(p.getZ()))
        return false;

    auto normal = getNormal(t);

    planes_t projPlane = findProjectionPlane(normal);

    const auto& p1 = t.getPoint(0);
    const auto& p2 = t.getPoint(1);
    const auto& p3 = t.getPoint(2);

    T u1 = 0;
    T v1_ = 0;
    T u2 = 0;
    T v2_ = 0;
    T u3 = 0;
    T v3_ = 0;
    T up = 0;
    T vp = 0;

    switch (projPlane) {
        case planes_t::XY:
            u1 = p1.getX(); v1_ = p1.getY();
            u2 = p2.getX(); v2_ = p2.getY();
            u3 = p3.getX(); v3_ = p3.getY();
            up = p.getX();   vp  = p.getY();
            break;
        case planes_t::XZ:
            u1 = p1.getX(); v1_ = p1.getZ();
            u2 = p2.getX(); v2_ = p2.getZ();
            u3 = p3.getX(); v3_ = p3.getZ();
            up = p.getX();   vp = p.getZ();
            break;
        case planes_t::YZ:
            u1 = p1.getY(); v1_ = p1.getZ();
            u2 = p2.getY(); v2_ = p2.getZ();
            u3 = p3.getY(); v3_ = p3.getZ();
            up = p.getY();   vp = p.getZ();
            break;
    };

    T area  = (u2 - u1)*(v3_ - v1_) - (v2_ - v1_)*(u3 - u1);
    T area1 = (u2 - up)*(v3_ - vp) - (v2_ - vp)*(u3 - up);
    T area2 = (u3 - up)*(v1_ - vp) - (v3_ - vp)*(u1 - up);
    T area3 = (u1 - up)*(v2_ - vp) - (v1_ - vp)*(u2 - up);

    bool sameSign = ((is_aez(area) && is_aez(area1, area2, area3)) ||
                    (is_bez(area) && is_bez(area1, area2, area3)));
    if (is_z(signDeterminant(p1, p2, p3, p)))
    {
        return sameSign;
    }
    else
    {
        return false;
    }
}


template<typename T>
bool isParallel(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2)
{
    auto n1 = getNormal(t1);
    auto n2 = getNormal(t2);
    
    if (!cross(n1, n2).is_zero())
    {
        return false;
    }
    
    const auto& p1 = t1.getPoint(0);
    const auto& p2 = t2.getPoint(0);
    
    Geo::Vector<T> vec(p1, p2);
    
    T mixed = scalar(vec, n1);
    
    bool samePlane = std::abs(mixed) < std::numeric_limits<T>::epsilon();
    
    return true;
}


template <typename T>
bool
Geo::isIntersect(const Geo::Triangle<T>& t1, const Geo::Triangle<T>& t2)
{
    const auto& p1_t1 = t1.getPoint(0);
    const auto& p2_t1 = t1.getPoint(1);
    const auto& p3_t1 = t1.getPoint(2);

    const auto& p1_t2 = t2.getPoint(0);
    const auto& p2_t2 = t2.getPoint(1);
    const auto& p3_t2 = t2.getPoint(2);


    bool is_point1 = isEqual(p1_t1, p2_t1) && isEqual(p2_t1, p3_t1);
    bool is_point2 = isEqual(p1_t2, p2_t2) && isEqual(p2_t2, p3_t2);

    if (is_point1 && is_point2)
    {
        return isEqual(p1_t1, p1_t2);
    }

    if (is_point1)
    {
        if (isLine(t2))
        {
            return isAmong(p1_t1, p1_t2, p2_t2) ||
                   isAmong(p1_t1, p2_t2, p3_t2) ||
                   isAmong(p1_t1, p3_t2, p1_t2);
        }
        return isPointBelongsTriangle(p1_t1, t2);
    }

    if (is_point2)
    {
        if (isLine(t1))
            return isAmong(p1_t2, p1_t1, p2_t1) ||
                   isAmong(p1_t2, p2_t1, p3_t1) ||
                   isAmong(p1_t2, p3_t1, p1_t1);
        return isPointBelongsTriangle(p1_t2, t1);
    }
    
    if (::isParallel(t1, t2))
    {
        return intersect2D(t1, t2);
    }

    return intersect3D(t1, t2);
}

template <typename T> std::ostream&
Geo::operator<<(std::ostream& os, const Triangle<T>& triangle)
{
    os << "Triangle(" << triangle.getPoint(0) << ", " << triangle.getPoint(1) << ", " << triangle.getPoint(2) << ")";
    return os;
}
