#ifndef GEO_H
#define GEO_H

#include "double.hpp"
#include "geo_point.hpp"
#include "geo_line.hpp"

#include <cmath>
#include <vector>
#include <ostream>

namespace Geo {

enum class planes_t { XY, XZ, YZ };

namespace Detail {

template<typename T> T
signDeterminant(const Geo::Point<T>& plane_p1,
                  const Geo::Point<T>& plane_p2,
                  const Geo::Point<T>& plane_p3,
                  const Geo::Point<T>& p)
{
    double v1x = plane_p1.x_ - p.x_;
    double v1y = plane_p1.y_ - p.y_;
    double v1z = plane_p1.z_ - p.z_;

    double v2x = plane_p2.x_ - p.x_;
    double v2y = plane_p2.y_ - p.y_;
    double v2z = plane_p2.z_ - p.z_;

    double v3x = plane_p3.x_ - p.x_;
    double v3y = plane_p3.y_ - p.y_;
    double v3z = plane_p3.z_ - p.z_;

    double det =
        v1x * (v2y * v3z - v2z * v3y) -
        v1y * (v2x * v3z - v2z * v3x) +
        v1z * (v2x * v3y - v2y * v3x);

    return det;
}

template<typename T> planes_t
findProjectionPlane(const Geo::Vector<T>& normal)
{
    double ax = std::abs(normal.xProj_);
    double ay = std::abs(normal.yProj_);
    double az = std::abs(normal.zProj_);

    if (ax >= ay && ax >= az)
        return planes_t::YZ;
    else if (ay >= ax && ay >= az)
        return planes_t::XZ;
    else
        return planes_t::XY;
}

template <typename T>
double getPointProj(const Point<T>& point, const Line<T>& line)
{
    const Point<T>& linePoint = line.getPoint();
    const Vector<T>& lineBasis = line.getBasis();

    Vector<T> fromLinePointToPoint = Vector<T>(linePoint, point);

    double lineBasisLengthSq = lineBasis.multiply_scalar_by(lineBasis);

    double proj = fromLinePointToPoint.multiply_scalar_by(lineBasis) / lineBasisLengthSq;

    return proj;
}

}

template<typename T>
class Triangle
{
    Point<T> p1_;
    Point<T> p2_;
    Point<T> p3_;
    Line<T> l1_;
    Line<T> l2_;
    Line<T> l3_;

    // Functions specialized to find intersection in 2D
    bool
    is_intersectAABB(const Triangle<T>& anotherTriangle, planes_t projPlane) const
    {
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
        }

        double th_min1 = std::min({p1_[plainIdx1], p2_[plainIdx1], p3_[plainIdx1]});
        double th_max1 = std::max({p1_[plainIdx1], p2_[plainIdx1], p3_[plainIdx1]});
        double th_min2 = std::min({p1_[plainIdx2], p2_[plainIdx2], p3_[plainIdx2]});
        double th_max2 = std::max({p1_[plainIdx2], p2_[plainIdx2], p3_[plainIdx2]});

        double an_min1 = std::min({anotherTriangle.p1_[plainIdx1], 
                                    anotherTriangle.p2_[plainIdx1],
                                    anotherTriangle.p3_[plainIdx1]});

        double an_max1 = std::max({anotherTriangle.p1_[plainIdx1],
                                    anotherTriangle.p2_[plainIdx1],
                                    anotherTriangle.p3_[plainIdx1]});

        double an_min2 = std::min({anotherTriangle.p1_[plainIdx2],
                                    anotherTriangle.p2_[plainIdx2],
                                    anotherTriangle.p3_[plainIdx2]});

        double an_max2 = std::max({anotherTriangle.p1_[plainIdx2],
                                    anotherTriangle.p2_[plainIdx2],
                                    anotherTriangle.p3_[plainIdx2]});

        if (is_bz(th_max1 - an_min1) || is_bz(an_max1 - th_min1) ||
            is_bz(th_max2 - an_min2) || is_bz(an_max2 - th_min2))
        {
            return false;
        }

        return true;
    }

    bool
    is_intersectSAT(const Triangle<T>& anotherTriangle, planes_t projPlane) const
    {
        // 2 треугольника по 3 линии
        const Line<T>* lines[2][3] = {
            {&l1_, &l2_, &l3_},
            {&anotherTriangle.l1_, &anotherTriangle.l2_, &anotherTriangle.l3_}
        };

        // 2 треугольника по 3 точки
        const Point<T>* points[2][3] = {
            {&p1_, &p2_, &p3_}, 
            {&anotherTriangle.p1_, &anotherTriangle.p2_, &anotherTriangle.p3_}
        };

        for (size_t tIdx = 0; tIdx < 2; ++tIdx)
        for (size_t pIdx = 0; pIdx < 3; ++pIdx)
        {
            const Line<T>& currLine = *lines[tIdx][pIdx % 3];
            const Vector<T>& currLineBasis = currLine.getBasis();
            const Point<T>& currLinePoint = currLine.getPoint();
            Line<T> normal;

            if (projPlane == planes_t::XY)
            {
                normal = Line<T>{Vector<T>(-currLineBasis.yProj_, currLineBasis.xProj_, 0.0), currLinePoint};
            }
            else if (projPlane == planes_t::XZ)
            {
                normal = Line<T>{Vector<T>(-currLineBasis.zProj_, 0.0, currLineBasis.xProj_), currLinePoint};
            }

            const Point<T>& th_p1 = *points[tIdx][pIdx];
            const Point<T>& th_p2 = *points[tIdx][(pIdx + 1) % 3];
            const Point<T>& th_p3 = *points[tIdx][(pIdx + 2) % 3];

            const Point<T>& an_p1 = *points[(tIdx + 1) % 2][pIdx % 3];
            const Point<T>& an_p2 = *points[(tIdx + 1) % 2][(pIdx + 1) % 3];
            const Point<T>& an_p3 = *points[(tIdx + 1) % 2][(pIdx + 2) % 3];

            double th_p1_proj = Detail::getPointProj(th_p1, normal);
            double th_p2_proj = Detail::getPointProj(th_p2, normal);
            double th_p3_proj = Detail::getPointProj(th_p3, normal);

            double an_p1_proj = Detail::getPointProj(an_p1, normal);
            double an_p2_proj = Detail::getPointProj(an_p2, normal);
            double an_p3_proj = Detail::getPointProj(an_p3, normal);

            double th_max_proj = std::max({th_p1_proj, th_p2_proj, th_p3_proj});
            double th_min_proj = std::min({th_p1_proj, th_p2_proj, th_p3_proj});

            double an_max_proj = std::max({an_p1_proj, an_p2_proj, an_p3_proj});
            double an_min_proj = std::min({an_p1_proj, an_p2_proj, an_p3_proj});

            if (is_bz(th_max_proj - an_min_proj) || is_bz(an_max_proj - th_min_proj))
            {
                return false;
            }
        }
        return true;
    }

    bool
    intersect2D(Triangle<T>& anotherTriangle) const
    {
        Vector<T> v1(p2_.x_ - p1_.x_, p2_.y_ - p1_.y_, p2_.z_ - p1_.z_);
        Vector<T> v2(p3_.x_ - p1_.x_, p3_.y_ - p1_.y_, p3_.z_ - p1_.z_);
        Vector<T> normal = v1.multiply_vectorially_by(v2); // нормаль к плоскости треугольников

        planes_t projectionPlane = Detail::findProjectionPlane(normal);

        if (!is_intersectAABB(anotherTriangle, projectionPlane)) return false;

        if (!is_intersectSAT(anotherTriangle, projectionPlane)) return false;

        return true;
    }

    // Functions specialized to find intersection in 2D

    bool
    containsPoint(const Point<T>& p) const
    {
        if (std::isnan(p.x_) || std::isnan(p.y_) || std::isnan(p.z_))
            return false;

        Vector<T> v1(p2_, p1_);
        Vector<T> v2(p3_, p1_);
        Vector<T> normal = v1.multiply_vectorially_by(v2);

        planes_t projPlane = Detail::findProjectionPlane(normal);

        double u1 = 0;
        double v1_ = 0;
        double u2 = 0;
        double v2_ = 0;
        double u3 = 0;
        double v3_ = 0;
        double up = 0;
        double vp = 0;

        switch (projPlane) {
            case planes_t::XY:
                u1 = p1_.x_; v1_ = p1_.y_;
                u2 = p2_.x_; v2_ = p2_.y_;
                u3 = p3_.x_; v3_ = p3_.y_;
                up = p.x_; vp = p.y_;
                break;
            case planes_t::XZ:
                u1 = p1_.x_; v1_ = p1_.z_;
                u2 = p2_.x_; v2_ = p2_.z_;
                u3 = p3_.x_; v3_ = p3_.z_;
                up = p.x_; vp = p.z_;
                break;
            case planes_t::YZ:
                u1 = p1_.y_; v1_ = p1_.z_;
                u2 = p2_.y_; v2_ = p2_.z_;
                u3 = p3_.y_; v3_ = p3_.z_;
                up = p.y_; vp = p.z_;
                break;
        }

        double area  = (u2 - u1)*(v3_ - v1_) - (v2_ - v1_)*(u3 - u1);
        double area1 = (u2 - up)*(v3_ - vp) - (v2_ - vp)*(u3 - up);
        double area2 = (u3 - up)*(v1_ - vp) - (v3_ - vp)*(u1 - up);
        double area3 = (u1 - up)*(v2_ - vp) - (v1_ - vp)*(u2 - up);

        bool sameSign = ((is_aez(area) && is_aez(area1, area2, area3)) ||
                        (is_bez(area) && is_bez(area1, area2, area3)));
        if (is_z(Detail::signDeterminant(p1_, p2_, p3_, p)))
        {
            return sameSign;
        }
        else
        {
            return false;
        }
    }

    bool
    is_line() const
    {
        Vector side1(p1_, p2_);
        Vector side2(p1_, p3_);
        Vector normal = side1.multiply_vectorially_by(side2);
        return is_z(normal.xProj_) && is_z(normal.yProj_) && is_z(normal.zProj_);
    }
    
    bool
    intersect3D(Triangle<T>& anotherTriangle) const
    {
        Point<T> an_p1 = l1_.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);
        Point<T> an_p2 = l2_.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);
        Point<T> an_p3 = l3_.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);

        if ((an_p1.is_among(p1_, p2_) && anotherTriangle.containsPoint(an_p1)) ||
            (an_p2.is_among(p2_, p3_) && anotherTriangle.containsPoint(an_p2)) ||
            (an_p3.is_among(p1_, p3_) && anotherTriangle.containsPoint(an_p3))) {
            return true;
        }

        Point<T> th_p1 = anotherTriangle.l1_.intersect(p1_, p2_, p3_);
        Point<T> th_p2 = anotherTriangle.l2_.intersect(p1_, p2_, p3_);
        Point<T> th_p3 = anotherTriangle.l3_.intersect(p1_, p2_, p3_);

        if ((th_p1.is_among(anotherTriangle.p1_, anotherTriangle.p2_) && containsPoint(th_p1)) ||
            (th_p2.is_among(anotherTriangle.p2_, anotherTriangle.p3_) && containsPoint(th_p2)) ||
            (th_p3.is_among(anotherTriangle.p1_, anotherTriangle.p3_) && containsPoint(th_p3))) {
            return true;
        }

        return false;
    }


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

    /// @brief Check if a this triangle is parallel to a triangle
    
    bool
    is_parallelTo(Triangle<T>& anotherTriangle)
    {
        const Vector<T> normal1 = l1_.getBasis().multiply_vectorially_by(l2_.getBasis());
        const Vector<T> normal2 = anotherTriangle.l1_.getBasis().multiply_vectorially_by(anotherTriangle.l2_.getBasis());
        const Vector<T> cross = normal1.multiply_vectorially_by(normal2);
        if (is_z(cross.xProj_) && is_z(cross.yProj_) && is_z(cross.zProj_)) {
            // Проверяем, лежат ли треугольники в одной плоскости
            Vector<T> vec(p1_, anotherTriangle.p1_);
            return is_z(vec.multiply_scalar_by(normal1));
        }
        return false;
    }

    /// @brief Check intersection of a this triangle with a triangle
    /// @return true if intersection detected, false - otherwise
    bool
    is_intersect(Triangle<T>& anotherTriangle)
    {
        bool is_point1 = p1_.is_equalTo(p2_) && p2_.is_equalTo(p3_);
        bool is_point2 = anotherTriangle.p1_.is_equalTo(anotherTriangle.p2_) && anotherTriangle.p2_.is_equalTo(anotherTriangle.p3_);

        if (is_point1 && is_point2)
        {
            return p1_.is_equalTo(anotherTriangle.p1_);
        }
        if (is_point1)
        {
            if (anotherTriangle.is_line())
                return p1_.is_among(anotherTriangle.p1_, anotherTriangle.p2_) ||
                    p1_.is_among(anotherTriangle.p2_, anotherTriangle.p3_) ||
                    p1_.is_among(anotherTriangle.p3_, anotherTriangle.p1_);
            return anotherTriangle.containsPoint(p1_);
        }
        if (is_point2)
        {
            if (is_line())
                return anotherTriangle.p1_.is_among(p1_, p2_) ||
                    anotherTriangle.p2_.is_among(p2_, p3_) ||
                    anotherTriangle.p3_.is_among(p3_, p1_);
            return containsPoint(anotherTriangle.p1_);
        }
        
        if (this->is_parallelTo(anotherTriangle)) return intersect2D(anotherTriangle);

        return intersect3D(anotherTriangle);
    }

    /// @brief 0 = get(first point), 1 = get(second point), 2 = get(third point)
    const Point<T>&
    getPoint(size_t index) const
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

template <typename T> std::ostream&
operator<<(std::ostream& os, const Triangle<T>& triangle)
{
    os << "Triangle(" << triangle.getPoint(0) << ", " << triangle.getPoint(1) << ", " << triangle.getPoint(2) << ")";
    return os;
}

} // namespace Geo

#endif // GEO_H