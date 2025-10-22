#include "geo.h"
#include <algorithm>
#include <iostream>


/// @brief Проецирует точку на прямую
/// @param point Точка, которую нужно спроецировать
/// @param line Прямая, на которую нужно спроецировать точку
/// @return Координаты проекции точки на прямую
static double getPointProj(const Geo::detail::Point& point, const Geo::detail::Line& line)
{
    const Geo::detail::Point& linePoint = line.getPoint();
    const Geo::detail::GeoVector& lineBasis = line.getBasis();

    Geo::detail::GeoVector fromLinePointToPoint = Geo::detail::GeoVector(linePoint, point);


    double lineBasisLengthSq = lineBasis.multiply_scalar_by(lineBasis);

    double proj = fromLinePointToPoint.multiply_scalar_by(lineBasis) / lineBasisLengthSq;

    return proj;
}

namespace Geo {

namespace detail
{

/// @brief Определяет плоскость проекции, исходя из нормали
/// @param normal Нормаль к плоскости
/// @return Плоскость проекции
Geo::detail::planes_t findProjectionPlane(const Geo::detail::GeoVector& normal)
{
    double ax = std::abs(normal.xProj_);
    double ay = std::abs(normal.yProj_);
    double az = std::abs(normal.zProj_);

    if (ax >= ay && ax >= az)
        return Geo::detail::planes_t::YZ;
    else if (ay >= ax && ay >= az)
        return Geo::detail::planes_t::XZ;
    else
        return Geo::detail::planes_t::XY;
}

} // namespace detail

bool Triangle::is_intersectAABB(const Triangle& anotherTriangle, detail::planes_t projPlane)
{
    size_t plainIdx1 = 0;
    size_t plainIdx2 = 0;

    switch (projPlane)
    {
        case detail::planes_t::XY:
        {
            plainIdx1 = 0;
            plainIdx2 = 0;
            break;
        }
        case detail::planes_t::XZ:
        {
            plainIdx1 = 0;
            plainIdx2 = 2;
            break;
        }
        case detail::planes_t::YZ:
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

        double th_p1_proj = getPointProj(th_p1, normal);
        double th_p2_proj = getPointProj(th_p2, normal);
        double th_p3_proj = getPointProj(th_p3, normal);

        double an_p1_proj = getPointProj(an_p1, normal);
        double an_p2_proj = getPointProj(an_p2, normal);
        double an_p3_proj = getPointProj(an_p3, normal);

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

/// @brief Проверяет пересечение треугольников в 2D
bool Triangle::intersect2D(Triangle& anotherTriangle) {

    GeoVector v1(p2_.x_ - p1_.x_, p2_.y_ - p1_.y_, p2_.z_ - p1_.z_);
    GeoVector v2(p3_.x_ - p1_.x_, p3_.y_ - p1_.y_, p3_.z_ - p1_.z_);
    GeoVector normal = v1.multiply_vectorially_by(v2); // нормаль к плоскости треугольников

    detail::planes_t projectionPlane = detail::findProjectionPlane(normal);

    if (!is_intersectAABB(anotherTriangle, projectionPlane)) return false;

    if (!is_intersectSAT(anotherTriangle, projectionPlane)) return false;

    return true;
}

} // namespace Geo