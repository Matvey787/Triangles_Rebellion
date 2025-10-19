#include "geo.h"
#include <iostream>

namespace Geo {

namespace detail {
    /// @brief Определяет положение точки p относительно плоскости, заданной точками plane_p1, plane_p2, plane_p3
    double signDeterminant(const Point& plane_p1, const Point& plane_p2, const Point& plane_p3, const Point& p)
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

}

bool Triangle::is_line() const
{
    GeoVector side1(p1_, p2_);
    GeoVector side2(p1_, p3_);
    GeoVector normal = side1.multiply_vectorially_by(side2);
    return is_doubleZero(normal.xProj_) && is_doubleZero(normal.yProj_) && is_doubleZero(normal.zProj_);
}

// Проверка пересечения двух треугольников в 3D
bool Triangle::intersect3D(Triangle& anotherTriangle)
{
    // вырожденные треугольники
    if (is_line())
    {
        Line line{p1_, p1_.is_equalTo(p2_) ? p3_ : p2_};
        Point intersection = line.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);
        if (anotherTriangle.containsPoint(intersection)) return true;

        return false;
    }
    if (anotherTriangle.is_line())
    {
        Line line{anotherTriangle.p1_, anotherTriangle.p1_.is_equalTo(anotherTriangle.p2_) ? anotherTriangle.p3_ : anotherTriangle.p2_};
        Point intersection = line.intersect(p1_, p2_, p3_);
        if (containsPoint(intersection)) return true;

        return false;
    }

    const Point& th_p1 = p1_;
    const Point& th_p2 = p2_;
    const Point& th_p3 = p3_;

    const Point& an_p1 = anotherTriangle.p1_;
    const Point& an_p2 = anotherTriangle.p2_;
    const Point& an_p3 = anotherTriangle.p3_;

    double det1 = detail::signDeterminant(an_p1, an_p2, an_p3, th_p1);
    double det2 = detail::signDeterminant(an_p1, an_p2, an_p3, th_p2);
    double det3 = detail::signDeterminant(an_p1, an_p2, an_p3, th_p3);

    if (is_aboveZero(det1, det2, det3) || is_belowZero(det1, det2, det3)) return false;

    double det4 = detail::signDeterminant(th_p1, th_p2, th_p3, an_p1);
    double det5 = detail::signDeterminant(th_p1, th_p2, th_p3, an_p2);
    double det6 = detail::signDeterminant(th_p1, th_p2, th_p3, an_p3);

    if (is_aboveZero(det4, det5, det6) || is_belowZero(det4, det5, det6)) return false;

    // Проверка компланарности
    std::vector<Point*> th_complanarVertices;
    if (is_doubleZero(det1)) th_complanarVertices.push_back(&p1_);
    if (is_doubleZero(det2)) th_complanarVertices.push_back(&p2_);
    if (is_doubleZero(det3)) th_complanarVertices.push_back(&p3_);

    std::vector<Point*> an_complanarVertices;
    if (is_doubleZero(det4)) an_complanarVertices.push_back(&anotherTriangle.p1_);
    if (is_doubleZero(det5)) an_complanarVertices.push_back(&anotherTriangle.p2_);
    if (is_doubleZero(det6)) an_complanarVertices.push_back(&anotherTriangle.p3_);

    for (Point* p : th_complanarVertices) if (anotherTriangle.containsPoint(*p)) return true;

    for (Point* p : an_complanarVertices) if (containsPoint(*p)) return true;

    // перестановка вершин
    Point* t1_v1 = &p1_;
    Point* t1_v2 = &p2_;
    Point* t1_v3 = &p3_;
    if ((det1 > 1e-10 && det2 < -1e-10 && det3 < -1e-10) || 
        (det1 < -1e-10 && det2 > 1e-10 && det3 > 1e-10)) {
        // p1_ — изолированная вершина
    } else if ((is_aboveZero(det2) && is_belowZero(det1) && is_belowZero(det3)) || 
               (is_belowZero(det2) && is_aboveZero(det1) && is_aboveZero(det3))) {
        t1_v1 = &p2_;
        t1_v2 = &p1_;
        t1_v3 = &p3_;
    } else if ((is_aboveZero(det3) && is_belowZero(det1) && is_belowZero(det2)) || 
               (is_belowZero(det3) && is_aboveZero(det1) && is_aboveZero(det2))) {
        t1_v1 = &p3_;
        t1_v2 = &p1_;
        t1_v3 = &p2_;
    }

    Point* t2_v1 = &anotherTriangle.p1_;
    Point* t2_v2 = &anotherTriangle.p2_;
    Point* t2_v3 = &anotherTriangle.p3_;
    if ((is_aboveZero(det4) && is_belowZero(det5) && is_belowZero(det6)) || 
        (is_belowZero(det4) && is_aboveZero(det5) && is_aboveZero(det6))) {
    } else if ((is_aboveZero(det5) && is_belowZero(det4) && is_belowZero(det6)) || 
               (is_belowZero(det5) && is_aboveZero(det4) && is_aboveZero(det6))) {
        t2_v1 = &anotherTriangle.p2_;
        t2_v2 = &anotherTriangle.p1_;
        t2_v3 = &anotherTriangle.p3_;
    } else if ((is_aboveZero(det6) && is_belowZero(det4) && is_belowZero(det5)) || 
               (is_belowZero(det6) && is_aboveZero(det4) && is_aboveZero(det5))) {
        t2_v1 = &anotherTriangle.p3_;
        t2_v2 = &anotherTriangle.p1_;
        t2_v3 = &anotherTriangle.p2_;
    }

    // проверка пересечения интервалов
    double det7 = detail::signDeterminant(*t1_v1, *t1_v2, *t2_v1, *t2_v2);
    double det8 = detail::signDeterminant(*t1_v1, *t1_v3, *t2_v3, *t2_v1);
    if(is_belowZero(det7) && is_belowZero(det8)) return true;

    // пересечения по граням
    const Line th_lines[3] = {{th_p1, th_p2}, {th_p2, th_p3}, {th_p3, th_p1}};
    const Line an_lines[3] = {{an_p1, an_p2}, {an_p2, an_p3}, {an_p3, an_p1}};

    for (auto& line : th_lines)
    {
        if (anotherTriangle.containsPoint(line.intersect(an_lines[0]))) return true;

        if (anotherTriangle.containsPoint(line.intersect(an_lines[1]))) return true;

        if (anotherTriangle.containsPoint(line.intersect(an_lines[2]))) return true;
    }

    return false;
}
}