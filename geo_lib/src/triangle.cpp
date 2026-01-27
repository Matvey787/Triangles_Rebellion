#include "geo.h"
#include "double.h"
#include <iostream>
#include <algorithm>

/// @brief Определяет положение точки p относительно плоскости, заданной точками plane_p1, plane_p2, plane_p3
/// @param plane_p1 Первая точка плоскости
/// @param plane_p2 Вторая точка плоскости
/// @param plane_p3 Третья точка плоскости
/// @param p Точка, положение которой нужно определить
static double signDeterminant(const Geo::Point& plane_p1,
                       const Geo::Point& plane_p2,
                       const Geo::Point& plane_p3,
                       const Geo::Point& p)
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

namespace Geo {

bool Triangle::containsPoint(const Point& P)
{
    if (std::isnan(P.x_) || std::isnan(P.y_) || std::isnan(P.z_))
        return false;

    GeoVector v1(p2_, p1_);
    GeoVector v2(p3_, p1_);
    GeoVector normal = v1.multiply_vectorially_by(v2);

    detail::planes_t projPlane = detail::findProjectionPlane(normal);

    double u1 = 0;
    double v1_ = 0;
    double u2 = 0;
    double v2_ = 0;
    double u3 = 0;
    double v3_ = 0;
    double up = 0;
    double vp = 0;

    switch (projPlane) {
        case detail::planes_t::XY:
            u1 = p1_.x_; v1_ = p1_.y_;
            u2 = p2_.x_; v2_ = p2_.y_;
            u3 = p3_.x_; v3_ = p3_.y_;
            up = P.x_; vp = P.y_;
            break;
        case detail::planes_t::XZ:
            u1 = p1_.x_; v1_ = p1_.z_;
            u2 = p2_.x_; v2_ = p2_.z_;
            u3 = p3_.x_; v3_ = p3_.z_;
            up = P.x_; vp = P.z_;
            break;
        case detail::planes_t::YZ:
            u1 = p1_.y_; v1_ = p1_.z_;
            u2 = p2_.y_; v2_ = p2_.z_;
            u3 = p3_.y_; v3_ = p3_.z_;
            up = P.y_; vp = P.z_;
            break;
    }

    double area  = (u2 - u1)*(v3_ - v1_) - (v2_ - v1_)*(u3 - u1);
    double area1 = (u2 - up)*(v3_ - vp) - (v2_ - vp)*(u3 - up);
    double area2 = (u3 - up)*(v1_ - vp) - (v3_ - vp)*(u1 - up);
    double area3 = (u1 - up)*(v2_ - vp) - (v1_ - vp)*(u2 - up);

    bool sameSign = ((is_aez(area) && is_aez(area1, area2, area3)) ||
                     (is_bez(area) && is_bez(area1, area2, area3)));
    if (is_z(signDeterminant(p1_, p2_, p3_, P)))
    {
        return sameSign;
    }
    else
    {
        return false;
    }
}



bool Triangle::is_parallelTo(Triangle& anotherTriangle) {
    const GeoVector normal1 = l1_.getBasis().multiply_vectorially_by(l2_.getBasis());
    const GeoVector normal2 = anotherTriangle.l1_.getBasis().multiply_vectorially_by(anotherTriangle.l2_.getBasis());
    const GeoVector cross = normal1.multiply_vectorially_by(normal2);
    if (is_z(cross.xProj_) && is_z(cross.yProj_) && is_z(cross.zProj_)) {
        // Проверяем, лежат ли треугольники в одной плоскости
        GeoVector vec(p1_, anotherTriangle.p1_);
        return is_z(vec.multiply_scalar_by(normal1));
    }
    return false;
}

bool Triangle::is_intersect(Triangle& anotherTriangle)
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
}; // namespace Geo