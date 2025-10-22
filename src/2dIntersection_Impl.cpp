#include "geo.h"
#include <algorithm>
#include <iostream>

namespace Geo {

namespace detail {

    enum class planes_t { XY, XZ };

    // FIXME
    inline planes_t findProjectionPlane(const GeoVector& normal)
    {
        if (!is_z(normal.zProj_))
            return planes_t::XY; // плоскость треугольников не перп. пл-ти XY

        return planes_t::XZ;
    }

    double getPointProj(const Point& point, const Line& line)
    {
        const Point& linePoint = line.getPoint();
        const GeoVector& lineBasis = line.getBasis();

        GeoVector fromLinePointToPoint = GeoVector(linePoint, point);


        double lineBasisLengthSq = lineBasis.multiply_scalar_by(lineBasis);

        double proj = fromLinePointToPoint.multiply_scalar_by(lineBasis) / lineBasisLengthSq;

        return proj;
    }

}

bool Triangle::is_intersectAABB(const Triangle& anotherTriangle, detail::planes_t projPlane)
{
    switch (projPlane)
    {
        case detail::planes_t::XY:
        {
            double th_minX = std::min({p1_.x_, p2_.x_, p3_.x_});
            double th_maxX = std::max({p1_.x_, p2_.x_, p3_.x_});
            double th_minY = std::min({p1_.y_, p2_.y_, p3_.y_});
            double th_maxY = std::max({p1_.y_, p2_.y_, p3_.y_});

            double an_minX = std::min({anotherTriangle.p1_.x_, 
                                       anotherTriangle.p2_.x_,
                                       anotherTriangle.p3_.x_});

            double an_maxX = std::max({anotherTriangle.p1_.x_,
                                       anotherTriangle.p2_.x_,
                                       anotherTriangle.p3_.x_});

            double an_minY = std::min({anotherTriangle.p1_.y_,
                                       anotherTriangle.p2_.y_,
                                       anotherTriangle.p3_.y_});

            double an_maxY = std::max({anotherTriangle.p1_.y_,
                                       anotherTriangle.p2_.y_,
                                       anotherTriangle.p3_.y_});

            if (is_bz(th_maxX - an_minX) || is_bz(an_maxX - th_minX) ||
                is_bz(th_maxY - an_minY) || is_bz(an_maxY - th_minY))
            {
                return false;
            }

            break;
        }
        case detail::planes_t::XZ:
        {
            double th_minX = std::min({p1_.x_, p2_.x_, p3_.x_});
            double th_maxX = std::max({p1_.x_, p2_.x_, p3_.x_});
            double th_minZ = std::min({p1_.z_, p2_.z_, p3_.z_});
            double th_maxZ = std::max({p1_.z_, p2_.z_, p3_.z_});

            double an_minX = std::min({anotherTriangle.p1_.x_,
                                       anotherTriangle.p2_.x_,
                                       anotherTriangle.p3_.x_});

            double an_maxX = std::max({anotherTriangle.p1_.x_,
                                       anotherTriangle.p2_.x_,
                                       anotherTriangle.p3_.x_});

            double an_minZ = std::min({anotherTriangle.p1_.z_,
                                       anotherTriangle.p2_.z_,
                                       anotherTriangle.p3_.z_});

            double an_maxZ = std::max({anotherTriangle.p1_.z_,
                                       anotherTriangle.p2_.z_,
                                       anotherTriangle.p3_.z_});

            if (is_bz(th_maxX - an_minX) || is_bz(an_maxX - th_minX) ||
                is_bz(th_maxZ - an_minZ) || is_bz(an_maxZ - th_minZ))
            {
                return false;
            }
            break;
        }
    }

    return true;
}

bool Triangle::is_intersectSAT(const Triangle& anotherTriangle, detail::planes_t projPlane)
{
    // 2 треугольника по 3 линии
    const Line* lines[2][3] = {
        {&l1_, &l2_, &l3_},
        {&anotherTriangle.l1_, &anotherTriangle.l2_, &anotherTriangle.l3_}
    };

    // 2 треугольника по 3 точки
    const Point* points[2][3] = {
        {&p1_, &p2_, &p3_}, 
        {&anotherTriangle.p1_, &anotherTriangle.p2_, &anotherTriangle.p3_}
    };

    for (size_t tIdx = 0; tIdx < 2; ++tIdx)
    for (size_t pIdx = 0; pIdx < 3; ++pIdx)
    {
        const Line& currLine = *lines[tIdx][pIdx % 3];
        const GeoVector& currLineBasis = currLine.getBasis();
        const Point& currLinePoint = currLine.getPoint();
        Line normal;

        if (projPlane == detail::planes_t::XY)
        {
            normal = Line{GeoVector(-currLineBasis.yProj_, currLineBasis.xProj_, 0.0), currLinePoint};
        }
        else if (projPlane == detail::planes_t::XZ)
        {
            normal = Line{GeoVector(-currLineBasis.zProj_, 0.0, currLineBasis.xProj_), currLinePoint};
        }

        const Point& th_p1 = *points[tIdx][pIdx];
        const Point& th_p2 = *points[tIdx][(pIdx + 1) % 3];
        const Point& th_p3 = *points[tIdx][(pIdx + 2) % 3];

        const Point& an_p1 = *points[(tIdx + 1) % 2][pIdx % 3];
        const Point& an_p2 = *points[(tIdx + 1) % 2][(pIdx + 1) % 3];
        const Point& an_p3 = *points[(tIdx + 1) % 2][(pIdx + 2) % 3];

        double th_p1_proj = detail::getPointProj(th_p1, normal);
        double th_p2_proj = detail::getPointProj(th_p2, normal);
        double th_p3_proj = detail::getPointProj(th_p3, normal);

        double an_p1_proj = detail::getPointProj(an_p1, normal);
        double an_p2_proj = detail::getPointProj(an_p2, normal);
        double an_p3_proj = detail::getPointProj(an_p3, normal);

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

bool Triangle::intersect2D(Triangle& anotherTriangle) {

    // std::cout << "intersect2d\n";

    GeoVector v1(p2_.x_ - p1_.x_, p2_.y_ - p1_.y_, p2_.z_ - p1_.z_);
    GeoVector v2(p3_.x_ - p1_.x_, p3_.y_ - p1_.y_, p3_.z_ - p1_.z_);
    GeoVector normal = v1.multiply_vectorially_by(v2); // нормаль к плоскости треугольников

    detail::planes_t projectionPlane = detail::findProjectionPlane(normal);

    if (!is_intersectAABB(anotherTriangle, projectionPlane)) return false;

    if (!is_intersectSAT(anotherTriangle, projectionPlane)) return false;

    return true;
}

}