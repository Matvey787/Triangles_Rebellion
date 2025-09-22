#include "geo.h"
#include "double.h"
#include <iostream>

Line Triangle::findIntersectLine(Triangle& anotherTriangle)
{
    // считаем нормали к плоскостям треугольников
    GeoVector l1b = l1_.get_basis();
    GeoVector l2b = l2_.get_basis();
    GeoVector normal1 = l1b.multiply_vectorially_by(l2b);

    GeoVector l1b_anotherT = anotherTriangle.l1_.get_basis();
    GeoVector l2b_anotherT = anotherTriangle.l2_.get_basis();
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

    if (!is_doubleZero(mainDet1)) // x = 0
    {
        y = (-d1 * c2 + d2 * c1) / mainDet1;
        z = (-b1 * d2 + b2 * d1) / mainDet1;
        return Line(intersectionBasis, Point(0, y, z));
    }
    else if (!is_doubleZero(mainDet2)) // y = 0
    {
        x = (-d1 * c2 + d2 * c1) / mainDet2;
        z = (-a1 * d2 + a2 * d1) / mainDet2;
        return Line(intersectionBasis, Point(x, 0, z));
    }
    else if (!is_doubleZero(mainDet3)) // z = 0
    {
        x = (-d1 * b2 + d2 * b1) / mainDet3;
        y = (-a1 * d2 + a2 * d1) / mainDet3;
        return Line(intersectionBasis, Point(x, y, 0));
    }

    // если все определители нулевые то это вообще не к нам :)
    return Line(intersectionBasis, Point(NAN, NAN, NAN));
}

bool Triangle::intersect3D(Triangle& anotherTriangle)
{
    Line intersectLine = findIntersectLine(anotherTriangle);

    // std::cout << "Intersection Line: " << intersectLine << std::endl;

    GeoVector intersectionBasis = intersectLine.get_basis();

    // find intersection points (triangles sides lines intersect intersectLine of triangle  
    // planes)
    
    // this triangle
    // точки, находящиеся на пересечеии каждой линии треуголька с линией пересечения плоскостей 
    // треугольников
    Point ip1 = l1_.intersect(intersectLine);
    Point ip2 = l2_.intersect(intersectLine);
    Point ip3 = l3_.intersect(intersectLine);

    // Проверяем, что точки также лежат на соответствующих им сторонам треугольника
    std::vector<Point*> riptt; // real intersection points of this triangle
    if (ip1.is_among(p1_, p2_)) riptt.push_back(&ip1);
    if (ip2.is_among(p2_, p3_)) riptt.push_back(&ip2);
    if (ip3.is_among(p3_, p1_)) riptt.push_back(&ip3);

    // another triangle
    // Поступаем аналогично и со вторым треугольником
    Point ip4 = anotherTriangle.l1_.intersect(intersectLine);
    Point ip5 = anotherTriangle.l2_.intersect(intersectLine);
    Point ip6 = anotherTriangle.l3_.intersect(intersectLine);
    std::vector<Point*> ripat; // real intersection points of another triangle
    if (ip4.is_among(anotherTriangle.p1_, anotherTriangle.p2_)) ripat.push_back(&ip4);
    if (ip5.is_among(anotherTriangle.p2_, anotherTriangle.p3_)) ripat.push_back(&ip5);
    if (ip6.is_among(anotherTriangle.p3_, anotherTriangle.p1_)) ripat.push_back(&ip6);

    // Если таких точек не оказалось, то есть плоскости треугольников пересекаются, но сами 
    // треугольники оказались далеки от линии пересечения
    if (riptt.empty() || ripat.empty()) return false;

    // Если по одной точке у треугольников на линии пересечения плоскостей треугольников,
    // то для их пересечения необходимо совпадения двух этих точек.
    if (riptt.size() == 1 && ripat.size() == 1)
    {
        if (riptt.at(0)->is_equalTo(*ripat.at(0))) return true;

        return false;
    }
    
    // Нашлись две такие точки хотя бы у одного треугольника
    if (riptt.size() >= 2)
    {
        Point* ip1_riptt = riptt.at(0);
        Point* ip2_riptt;

        if (riptt.size() == 3)
            ip2_riptt = riptt.at(1)->is_equalTo(*ip1_riptt) ? riptt.at(2) : riptt.at(1);
        else
            ip2_riptt = riptt.at(1);

        // смотрим, какая то точка второго треугольника лежит в отрезке первого треугольника
        for (Point* ip : ripat)
        {
            if (ip->is_among(*ip1_riptt, *ip2_riptt)) return true;
        }

        // и наоборот
        if (ripat.size() >= 2)
        {
            Point* ip1_ripat = ripat.at(0);
            Point* ip2_ripat;
            if (ripat.size() == 3)
                ip2_ripat = ripat.at(1)->is_equalTo(*ip1_ripat) ? ripat.at(2) : ripat.at(1);
            else
                ip2_ripat = ripat.at(1);

            for (Point* ip : riptt)
            {
                if (ip->is_among(*ip1_ripat, *ip2_ripat)) return true;
            }
        }
    }

    return false;
}

bool Triangle::intersect2D(Triangle& anotherTriangle)
{
    // std::cout << "It's 2D intersection" << std::endl;
    // вектор от одной точке одного треугольника до точки другого треугольника
    GeoVector helpVec(p1_, anotherTriangle.p1_);

    // нормальк плоскости треугольника
    GeoVector n = l1_.get_basis().multiply_vectorially_by(l2_.get_basis());

    // параллельны, но не в одной плоскости
    if (!is_doubleZero(helpVec.multiply_scalar_by(n))) return false;

    Point ip11 = l1_.intersect(anotherTriangle.l1_);
    Point ip12 = l1_.intersect(anotherTriangle.l2_);
    Point ip13 = l1_.intersect(anotherTriangle.l3_);

    bool ip11_belong_t1_and_t2 = ip11.is_among(p1_, p2_) &&
                                 ip11.is_among(anotherTriangle.p1_, anotherTriangle.p2_);

    bool ip12_belong_t1_and_t2 = ip12.is_among(p1_, p2_) &&
                                 ip12.is_among(anotherTriangle.p1_, anotherTriangle.p2_);

    bool ip13_belong_t1_and_t2 = ip13.is_among(p1_, p2_) &&
                                 ip13.is_among(anotherTriangle.p1_, anotherTriangle.p2_);

    if (ip11_belong_t1_and_t2 || ip12_belong_t1_and_t2 || ip13_belong_t1_and_t2) return true;

    Point ip21 = l2_.intersect(anotherTriangle.l1_);

    // std::cout << l2_ << std::endl;
    // std::cout << anotherTriangle.l1_ << std::endl;

    Point ip22 = l2_.intersect(anotherTriangle.l2_);
    Point ip23 = l2_.intersect(anotherTriangle.l3_);

    bool ip21_belong_t1_and_t2 = ip21.is_among(p2_, p3_) && 
                          ip21.is_among(anotherTriangle.p2_, anotherTriangle.p3_);

    bool ip22_belong_t1_and_t2 = ip22.is_among(p2_, p3_) &&
                          ip22.is_among(anotherTriangle.p2_, anotherTriangle.p3_);

    bool ip23_belong_t1_and_t2 = ip23.is_among(p2_, p3_) &&
                          ip23.is_among(anotherTriangle.p2_, anotherTriangle.p3_);

    if (ip21_belong_t1_and_t2 || ip22_belong_t1_and_t2 || ip23_belong_t1_and_t2) return true;

    Point ip31 = l3_.intersect(anotherTriangle.l1_);
    Point ip32 = l3_.intersect(anotherTriangle.l2_);
    Point ip33 = l3_.intersect(anotherTriangle.l3_);


    bool ip31_belong_t1_and_t2 = ip31.is_among(p3_, p1_) &&
                          ip31.is_among(anotherTriangle.p3_, anotherTriangle.p1_);

    bool ip32_belong_t1_and_t2 = ip32.is_among(p3_, p1_) &&
                          ip32.is_among(anotherTriangle.p3_, anotherTriangle.p1_);

    bool ip33_belong_t1_and_t2 = ip33.is_among(p3_, p1_) &&
                          ip33.is_among(anotherTriangle.p3_, anotherTriangle.p1_);

    if (ip31_belong_t1_and_t2 || ip32_belong_t1_and_t2 || ip33_belong_t1_and_t2) return true;

    // остался последний случай если один треугольник лежит внутри другого
    
    auto isAmongAnyPair = [](const Point& pt, const Point& a, const Point& b, const Point& c) -> bool {
        return pt.is_among(a, b) || pt.is_among(a, c) || pt.is_among(b, c);
    };

    // вершины this внутри отрезков пересечений со сторонами another
    bool thisInside =
        isAmongAnyPair(p1_, ip11, ip12, ip13) &&
        isAmongAnyPair(p2_, ip21, ip22, ip23) &&
        isAmongAnyPair(p3_, ip31, ip32, ip33);

    // вершины another внутри отрезков пересечений со сторонами this
    bool anotherInside =
        isAmongAnyPair(anotherTriangle.p1_, ip11, ip21, ip31) &&
        isAmongAnyPair(anotherTriangle.p2_, ip12, ip22, ip32) &&
        isAmongAnyPair(anotherTriangle.p3_, ip13, ip23, ip33);

    if (thisInside || anotherInside)
    {
        return true;
    }

    return false;
}

bool Triangle::is_parallelTo(Triangle& anotherTriangle)
{
    GeoVector normal1 = l1_.get_basis().multiply_vectorially_by(l2_.get_basis());
    GeoVector normal2 = anotherTriangle.l1_.get_basis().multiply_vectorially_by(anotherTriangle.l2_.get_basis());
    if (normal1.is_parallel(normal2)) return true;
    return false;
}

bool Triangle::is_intersect(Triangle& anotherTriangle)
{
    // std::cout << p1_ << std::endl;
    // std::cout << p2_ << std::endl;
    // std::cout << p3_ << std::endl;

    // std::cout << anotherTriangle.p1_ << std::endl;
    // std::cout << anotherTriangle.p2_ << std::endl;
    // std::cout << anotherTriangle.p3_ << std::endl;
    if (this->is_parallelTo(anotherTriangle)) return intersect2D(anotherTriangle);

    // std::cout << "It's 3d intersection" << std::endl;
    return intersect3D(anotherTriangle);
}
