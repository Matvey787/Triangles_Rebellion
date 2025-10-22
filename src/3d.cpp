#include "geo.h"
#include <iostream>

namespace Geo {

bool Triangle::is_line() const
{
    GeoVector side1(p1_, p2_);
    GeoVector side2(p1_, p3_);
    GeoVector normal = side1.multiply_vectorially_by(side2);
    return is_z(normal.xProj_) && is_z(normal.yProj_) && is_z(normal.zProj_);
}

// Проверка пересечения двух треугольников в 3D (без вырожденных случаев)
bool Triangle::intersect3D(Triangle& anotherTriangle)
{
    Point an_p1 = l1_.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);
    Point an_p2 = l2_.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);
    Point an_p3 = l2_.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);

    if ((an_p1.is_among(p1_, p2_) && anotherTriangle.containsPoint(an_p1)) ||
        (an_p2.is_among(p2_, p3_) && anotherTriangle.containsPoint(an_p2)) ||
        (an_p3.is_among(p1_, p3_) && anotherTriangle.containsPoint(an_p3))) {
        return true;
    }

    Point th_p1 = anotherTriangle.l1_.intersect(p1_, p2_, p3_);
    Point th_p2 = anotherTriangle.l2_.intersect(p1_, p2_, p3_);
    Point th_p3 = anotherTriangle.l2_.intersect(p1_, p2_, p3_);

    if ((th_p1.is_among(anotherTriangle.p1_, anotherTriangle.p2_) && containsPoint(th_p1)) ||
        (th_p2.is_among(anotherTriangle.p2_, anotherTriangle.p3_) && containsPoint(th_p2)) ||
        (th_p3.is_among(anotherTriangle.p1_, anotherTriangle.p3_) && containsPoint(th_p3))) {
        return true;
    }

    return false;
}
} // namespace Geo