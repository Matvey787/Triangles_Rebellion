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
    return is_z(normal.xProj_) && is_z(normal.yProj_) && is_z(normal.zProj_);
}

// Проверка пересечения двух треугольников в 3D
bool Triangle::intersect3D(Triangle& anotherTriangle)
{
    // вырожденные треугольники
    if (is_line())
    {
        Line line{p1_, p1_.is_equalTo(p2_) ? p3_ : p2_};
        Point intersection = line.intersect(anotherTriangle.p1_, anotherTriangle.p2_, anotherTriangle.p3_);
        if (anotherTriangle.containsPoint(intersection) && containsPoint(intersection)) return true;

        return false;
    }
    if (anotherTriangle.is_line())
    {
        Line line{anotherTriangle.p1_, anotherTriangle.p1_.is_equalTo(anotherTriangle.p2_) ? anotherTriangle.p3_ : anotherTriangle.p2_};
        Point intersection = line.intersect(p1_, p2_, p3_);
        if (containsPoint(intersection) && anotherTriangle.containsPoint(intersection)) return true;

        return false;
    }

    // std::cout << "Here 1\n";

    const Point& th_p1 = p1_;
    const Point& th_p2 = p2_;
    const Point& th_p3 = p3_;

    const Point& an_p1 = anotherTriangle.p1_;
    const Point& an_p2 = anotherTriangle.p2_;
    const Point& an_p3 = anotherTriangle.p3_;

    double det1 = detail::signDeterminant(an_p1, an_p2, an_p3, th_p1);
    double det2 = detail::signDeterminant(an_p1, an_p2, an_p3, th_p2);
    double det3 = detail::signDeterminant(an_p1, an_p2, an_p3, th_p3);

    std::cout << "det1: " << det1 << ", det2: " << det2 << ", det3: " << det3 << "\n";

    if (is_az(det1, det2, det3) || is_bz(det1, det2, det3)) return false;

    double det4 = detail::signDeterminant(th_p1, th_p2, th_p3, an_p1);
    double det5 = detail::signDeterminant(th_p1, th_p2, th_p3, an_p2);
    double det6 = detail::signDeterminant(th_p1, th_p2, th_p3, an_p3);

    std::cout << "det4: " << det4 << ", det5: " << det5 << ", det6: " << det6 << "\n";

    if (is_az(det4, det5, det6) || is_bz(det4, det5, det6)) return false;

    // Проверка компланарности
    std::vector<Point*> th_complanarVertices;
    if (is_z(det1)) th_complanarVertices.push_back(&p1_);
    if (is_z(det2)) th_complanarVertices.push_back(&p2_);
    if (is_z(det3)) th_complanarVertices.push_back(&p3_);

    std::vector<Point*> an_complanarVertices;
    if (is_z(det4)) an_complanarVertices.push_back(&anotherTriangle.p1_);
    if (is_z(det5)) an_complanarVertices.push_back(&anotherTriangle.p2_);
    if (is_z(det6)) an_complanarVertices.push_back(&anotherTriangle.p3_);

    for (Point* p : th_complanarVertices) if (anotherTriangle.containsPoint(*p)) return true;

    for (Point* p : an_complanarVertices) if (containsPoint(*p)) return true;

    // // // std::cout << "Here 2\n";

    // перестановка вершин
    // --- ПЕРЕУПОРЯДОЧИВАНИЕ ВЕРШИН С УЧЕТОМ ПОЛОЖИТЕЛЬНОЙ ПОЛУПЛОСКОСТИ ---

    Point* t1_v1 = &p1_;
    Point* t1_v2 = &p2_;
    Point* t1_v3 = &p3_;
    if ((is_az(det1) && is_bez(det2) && is_bez(det3)) || 
        (is_bz(det1) && is_aez(det2) && is_aez(det3))) {
        // std::cout << "p1 - изолированная вершина\n";
        if (is_bz(det1))
        {
            std::swap(t1_v2, t1_v3);
        }
    } else if ((is_az(det2) && is_bez(det1) && is_bez(det3)) || 
            (is_bz(det2) && is_aez(det1) && is_aez(det3))) {
        // std::cout << "p2 - изолированная вершина\n";
        t1_v1 = &p2_;
        t1_v2 = &p1_;
        t1_v3 = &p3_;
        if (is_bz(det2))
        {
            std::swap(t1_v2, t1_v3);
        }
        
    } else if ((is_az(det3) && is_bez(det1) && is_bez(det2)) || 
            (is_bz(det3) && is_aez(det1) && is_aez(det2))) {
        // std::cout << "p3 - изолированная вершина\n";
        t1_v1 = &p3_;
        t1_v2 = &p1_;
        t1_v3 = &p2_;
        if (is_bz(det3))
        {
            std::swap(t1_v2, t1_v3);
        }
    }

    Point* t2_v1 = &anotherTriangle.p1_;
    Point* t2_v2 = &anotherTriangle.p2_;
    Point* t2_v3 = &anotherTriangle.p3_;

    // Аналогично для второго треугольника
    if ((is_az(det4) && is_bez(det5) && is_bez(det6)) || 
        (is_bz(det4) && is_aez(det5) && is_aez(det6))) {
        // std::cout << "p1 - изолированная вершина (второй треугольник)\n";
        if (is_bz(det4))
        {
            std::swap(t2_v2, t2_v3);
        }

    } else if ((is_az(det5) && is_bez(det4) && is_bez(det6)) || 
            (is_bz(det5) && is_aez(det4) && is_aez(det6))) {
        t2_v1 = &anotherTriangle.p2_;
        t2_v2 = &anotherTriangle.p1_;
        t2_v3 = &anotherTriangle.p3_;
        if (is_bz(det5))
        {
            std::swap(t2_v2, t2_v3);
        }
    } else if ((is_az(det6) && is_bez(det4) && is_bez(det5)) || 
            (is_bz(det6) && is_aez(det4) && is_aez(det5))) {
        // std::cout << "p3 - изолированная вершина (второй треугольник)\n";
        t2_v1 = &anotherTriangle.p3_;
        t2_v2 = &anotherTriangle.p1_;
        t2_v3 = &anotherTriangle.p2_;
        if (is_bz(det6))
        {
            std::swap(t2_v2, t2_v3);
        }
    }


    // проверка пересечения интервалов
    double det7 = detail::signDeterminant(*t1_v1, *t1_v2, *t2_v1, *t2_v2);
    std::cout << "det7: " << det7 << std::endl;
    double det8 = detail::signDeterminant(*t1_v1, *t1_v3, *t2_v3, *t2_v1);
    std::cout << "det8: " << det8 << std::endl;
    if(is_bez(det7) && is_bez(det8)) return true;

    // // // std::cout << "Here 3\n";

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