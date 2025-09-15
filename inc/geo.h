#ifndef GEO_H
#define GEO_H

#include <cmath>

bool is_doubleZero(double num1, double num2 = NAN, double num3 = NAN)
{
    if (std::isnan(num2) && std::isnan(num3)) return std::abs(num1) < 0.001;

    if (std::isnan(num2)) return std::abs(num1)  < 0.001 && std::abs(num3) < 0.001;;

    if (std::isnan(num3)) return std::abs(num1) < 0.001 && std::abs(num2) < 0.001;

    return std::abs(num1) < 0.001 && std::abs(num2) < 0.001 && std::abs(num3) < 0.001;
}

struct Point
{
    double x_;
    double y_;
    double z_;
    
    Point() : x_(0.0), y_(0.0), z_(0.0) {}
    Point(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    bool is_among(Point& p1, Point& p2)
    {
        double r1x = std::abs(x_ - p1.x_);
        double r1y = std::abs(y_ - p1.y_);
        double r1z = std::abs(z_ - p1.z_);

        double r2x = std::abs(x_ - p2.x_);
        double r2y = std::abs(y_ - p2.y_);
        double r2z = std::abs(z_ - p2.z_);

        double rx = std::abs(p2.x_ - p1.x_);
        double ry = std::abs(p2.y_ - p1.y_);
        double rz = std::abs(p2.z_ - p1.z_);

        return ((r1x + r2x) == rx) && ((r1y + r2y) == ry) && ((r1z + r2z )== rz);
    }

    bool is_equalTo(Point& anotherPoint)
    {
        double xdiff = x_ - anotherPoint.x_;
        double ydiff = y_ - anotherPoint.y_;
        double zdiff = z_ - anotherPoint.z_;

        return is_doubleZero(xdiff, ydiff, zdiff);
    }
};

struct GeoVector
{
    double xProj_;
    double yProj_;
    double zProj_;

    GeoVector(double xProjection, double yProjection, double zProjection) : 
        xProj_(xProjection),
        yProj_(yProjection),
        zProj_(zProjection) {}
    
    GeoVector() : xProj_(0.0), yProj_(0.0), zProj_(0.0) {}

    bool is_zero()
    {
        return is_doubleZero(xProj_, yProj_, zProj_);
    }

    bool is_parallel(GeoVector& anotherVector)
    {
        double crossX = yProj_ * anotherVector.zProj_ - zProj_ * anotherVector.yProj_;
        double crossY = zProj_ * anotherVector.xProj_ - xProj_ * anotherVector.zProj_;
        double crossZ = xProj_ * anotherVector.yProj_ - yProj_ * anotherVector.xProj_;

        return is_doubleZero(crossX, crossY, crossZ);
    }

    GeoVector multiply_vectorially_by(const GeoVector& anotherVector)
    {
        return GeoVector(
            yProj_ * anotherVector.zProj_ - zProj_ * anotherVector.yProj_,
            zProj_ * anotherVector.xProj_ - xProj_ * anotherVector.zProj_,
            xProj_ * anotherVector.yProj_ - yProj_ * anotherVector.xProj_
        );
    }
};

class Line
{
    // canonical definition
    // (x - p_x) / basis_x = (y - p_y) / basis_y = (z - p_z) / basis_z
    Point p_;
    GeoVector basis_;


public:
    Line() = default;
    Line(Point& point1, Point& point2) :
        p_(point1),
        basis_(point2.x_ - point1.x_, point2.y_ - point1.y_, point2.z_ - point1.z_) {}

    Point intersect(Line& anotherLine)
    {
        if (this->is_parallelTo(anotherLine)) return Point(NAN, NAN, NAN);

        double x1 = p_.x_;
        double y1 = p_.y_;
        double z1 = p_.z_;
        double xb1 = basis_.xProj_;
        double yb1 = basis_.yProj_;
        double zb1 = basis_.zProj_;

        double x2 = anotherLine.p_.x_;
        double y2 = anotherLine.p_.y_;
        double xb2 = anotherLine.basis_.xProj_;
        double yb2 = anotherLine.basis_.yProj_;

        double k1_nominator = (x1 - x2) / xb2 - (y1 - y2) / yb2;
        double k1_denominator = (yb1 / yb2 - xb1 / xb2);
        double k1 = k1_nominator / k1_denominator;

        // intersection point
        double ipx = x1 + k1 * xb1;
        double ipy = y1 + k1 * yb1;
        double ipz = z1 + k1 * zb1;

        return Point(ipx, ipy, ipz);
    }

    bool is_parallelTo(Line& anotherLine)
    {
        return basis_.is_parallel(anotherLine.basis_);
    }

    GeoVector get_basis()
    {
        return basis_;
    }
};



class Triangle
{
    Point p1_;
    Point p2_;
    Point p3_;
    Line l1_;
    Line l2_;
    Line l3_;
    
    Line findIntersectLine(Triangle& anotherTriangle)
    {
        // two lines basises form one plane basis 
        // try to find a, b, c, d for general equation ax + by + cz + d = 0

        GeoVector l1b = l1_.get_basis();
        GeoVector l2b = l2_.get_basis();
        GeoVector normal1 = l1b.multiply_vectorially_by(l2b);
        double a1 = normal1.xProj_;
        double b1 = normal1.yProj_;
        double c1 = normal1.zProj_;
        double d1 = - (a1 * p1_.x_ + b1 * p1_.y_ + c1 * p1_.z_);


        GeoVector anotherTriangle_l1b = anotherTriangle.l1_.get_basis();
        GeoVector anotherTriangle_l2b = anotherTriangle.l2_.get_basis();
        GeoVector normal2 = anotherTriangle_l1b.multiply_vectorially_by(anotherTriangle_l2b);
        double a2 = normal2.xProj_;
        double b2 = normal2.yProj_;
        double c2 = normal2.zProj_;
        double d2 = - (a2 * anotherTriangle.p1_.x_ + b2 * anotherTriangle.p1_.y_ + c2 * anotherTriangle.p1_.z_);

        // look for two points of intersection of planes
        // get this points from system a1*x + b1*y + c1*z + d1 = 0; 
        //                             a2*x + b2*y + c2*z + d2 = 0;

        // let the x = 0 of the first point
        Point ip1;
        ip1.x_ = 0;
        ip1.y_ = (c1 / c2 * d2 - d1) / (b1 - c1 / c2 * b2);
        ip1.z_ = -1 * (d2 + b2 * ip1.y_) / c2;

        // let the x = 1 of the second point
        Point ip2;
        ip2.x_ = 1;
        ip2.y_ = (c1 / c2 * (d2 + a2) - d1 - a1) / (b1 - c1 / c2 * b2);
        ip2.z_ = -1 * (d2 + a2 + b2 * ip1.y_) / c2;

        return Line(ip1, ip2);

    }

public:
    Triangle(Point& point1, Point& point2, Point& point3) : 
        p1_(point1), 
        p2_(point2), 
        p3_(point3),
        l1_(p1_, p2_),
        l2_(p2_, p3_),
        l3_(p3_, p1_) {}

    // TODO cocnvert from lines to points,  back
    // Triangle(Line& line1, Line& line2, Line& line3) : l1_(line1), l2_(line2), l3_(line3) {}

    bool is_parallelTo(Triangle& anotherTriangle)
    {
        if (!l1_.is_parallelTo(anotherTriangle.l1_)) return false;
        if (!l2_.is_parallelTo(anotherTriangle.l2_)) return false;
        if (!l3_.is_parallelTo(anotherTriangle.l3_)) return false;
        return true;
    }

    bool is_intersect(Triangle& anotherTriangle)
    {
        // FIXME it could be not false
        if (this->is_parallelTo(anotherTriangle)) return false;

        Line intersectLine = findIntersectLine(anotherTriangle);

        // find intersection points (triangles sides lines intersect intersectLine of triangle  
        // planes)

        // this triangle
        Point ip1 = l1_.intersect(intersectLine);
        Point ip2 = l2_.intersect(intersectLine);
        Point ip3 = l3_.intersect(intersectLine);

        // another triangle
        Point ip4 = anotherTriangle.l1_.intersect(intersectLine);
        Point ip5 = anotherTriangle.l2_.intersect(intersectLine);
        Point ip6 = anotherTriangle.l3_.intersect(intersectLine);

    }



};



#endif // GEO_H