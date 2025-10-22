#include "geo.h"
#include "double.h"
#include <iostream>
#include <algorithm>

using namespace Geo;

Line Triangle::findIntersectLine(Triangle& anotherTriangle)
{
    // считаем нормали к плоскостям треугольников
    GeoVector l1b = l1_.getBasis();
    GeoVector l2b = l2_.getBasis();
    GeoVector normal1 = l1b.multiply_vectorially_by(l2b);

    GeoVector l1b_anotherT = anotherTriangle.l1_.getBasis();
    GeoVector l2b_anotherT = anotherTriangle.l2_.getBasis();
    GeoVector normal2 = l1b_anotherT.multiply_vectorially_by(l2b_anotherT);

    // направление линии пересечения — векторное произведение нормалей
    GeoVector intersectionBasis = normal1.multiply_vectorially_by(normal2);

    // коэффициенты уравнений плоскостей
    double a1 = normal1.xProj_;
    double b1 = normal1.yProj_;
    double c1 = normal1.zProj_;
    double d1 = - (a1 * p1_.x_ + b1 * p1_.y_ + c1 * p1_.z_);

    double a2 = normal2.xProj_;
    double b2 = normal2.yProj_;
    double c2 = normal2.zProj_;
    double d2 = - (a2 * anotherTriangle.p1_.x_ + b2 * anotherTriangle.p1_.y_ + c2 * anotherTriangle.p1_.z_);

    // пробуем найти точку на линии пересечения
    double x = 0, y = 0, z = 0;

    double mainDet1 = b1 * c2 - b2 * c1; // x = 0
    double mainDet2 = a1 * c2 - a2 * c1; // y = 0
    double mainDet3 = a1 * b2 - a2 * b1; // z = 0

    if (!is_z(mainDet1)) // x = 0
    {
        y = (-d1 * c2 + d2 * c1) / mainDet1;
        z = (-b1 * d2 + b2 * d1) / mainDet1;
        return Line(intersectionBasis, Point(0, y, z));
    }
    else if (!is_z(mainDet2)) // y = 0
    {
        x = (-d1 * c2 + d2 * c1) / mainDet2;
        z = (-a1 * d2 + a2 * d1) / mainDet2;
        return Line(intersectionBasis, Point(x, 0, z));
    }
    else if (!is_z(mainDet3)) // z = 0
    {
        x = (-d1 * b2 + d2 * b1) / mainDet3;
        y = (-a1 * d2 + a2 * d1) / mainDet3;
        return Line(intersectionBasis, Point(x, y, 0));
    }

    // если все определители нулевые то это вообще не к нам :)
    return Line(intersectionBasis, Point(NAN, NAN, NAN));
}

bool Triangle::containsPoint(const Point& P) {
    if (std::isnan(P.x_) || std::isnan(P.y_) || std::isnan(P.z_)) return false;
    GeoVector vector1(p2_, p1_);
    GeoVector vector2(p3_, p1_);
    GeoVector normal = vector1.multiply_vectorially_by(vector2);

    enum class Plane { XY, XZ, YZ } projPlane;

    double ax = std::abs(normal.xProj_);
    double ay = std::abs(normal.yProj_);
    double az = std::abs(normal.zProj_);

    if (ax >= ay && ax >= az)
        projPlane = Plane::YZ;
    else if (ay >= ax && ay >= az)
        projPlane = Plane::XZ;
    else
        projPlane = Plane::XY;

    double u1, v1, u2, v2, u3, v3, up, vp;

    switch (projPlane) {
        case Plane::XY:
            u1 = p1_.x_; v1 = p1_.y_;
            u2 = p2_.x_; v2 = p2_.y_;
            u3 = p3_.x_; v3 = p3_.y_;
            up = P.x_; vp = P.y_;
            break;
        case Plane::XZ:
            u1 = p1_.x_; v1 = p1_.z_;
            u2 = p2_.x_; v2 = p2_.z_;
            u3 = p3_.x_; v3 = p3_.z_;
            up = P.x_; vp = P.z_;
            break;
        case Plane::YZ:
            u1 = p1_.y_; v1 = p1_.z_;
            u2 = p2_.y_; v2 = p2_.z_;
            u3 = p3_.y_; v3 = p3_.z_;
            up = P.y_; vp = P.z_;
            break;
    }

    // ориентированный детерминант для 2D треугольника
    auto orient = [](double ax, double ay, double bx, double by, double cx, double cy) {
        return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
    };

    double d1 = orient(u1, v1, u2, v2, up, vp);
    double d2 = orient(u2, v2, u3, v3, up, vp);
    double d3 = orient(u3, v3, u1, v1, up, vp);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
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
    if (this->is_parallelTo(anotherTriangle)) return intersect2D(anotherTriangle);

    return intersect3D(anotherTriangle);
}
