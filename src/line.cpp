#include "geo.h"
#include "double.h"

Geo::Point Geo::Line::intersect(const Line& anotherLine) const
{
    // Проверка на параллельность
    if (this->is_parallelTo(anotherLine)) {
        return Geo::Point(NAN, NAN, NAN);
    }
    
    // Получаем параметры обеих линий
    double x1 = p_.x_;
    double y1 = p_.y_;
    double z1 = p_.z_;
    double a1 = basis_.xProj_;
    double b1 = basis_.yProj_;
    double c1 = basis_.zProj_;
    
    double x2 = anotherLine.p_.x_;
    double y2 = anotherLine.p_.y_;
    double z2 = anotherLine.p_.z_;
    double a2 = anotherLine.basis_.xProj_;
    double b2 = anotherLine.basis_.yProj_;
    double c2 = anotherLine.basis_.zProj_;
    
    // Пробуем решить через xy-плоскость
    double det_xy = a1 * (-b2) - (-a2) * b1;
    if (!is_doubleZero(det_xy)) {
        // Система уравнений:
        // a1*t - a2*s = dx, где dx = x2 - x1
        // b1*t - b2*s = dy, где dy = y2 - y1
        
        double dx = x2 - x1;
        double dy = y2 - y1;
        
        // Решаем методом Крамера
        double det_main = a1 * (-b2) - (-a2) * b1;
        double det_t = dx * (-b2) - (-a2) * dy;
        double det_s = a1 * dy - dx * b1;
        
        if (!is_doubleZero(det_main)) {
            double t = det_t / det_main;
            double s = det_s / det_main;
            
            // Проверяем z-координату
            double z_intersect1 = z1 + t * c1;
            double z_intersect2 = z2 + s * c2;
            
            if (is_doubleZero(z_intersect1 - z_intersect2)) {
                return Geo::Point(x1 + t * a1, y1 + t * b1, z_intersect1);
            }
        }
    }
    
    // Пробуем решить через xz-плоскость
    double det_xz = a1 * (-c2) - (-a2) * c1;
    if (!is_doubleZero(det_xz)) {
        // Система уравнений:
        // a1*t - a2*s = dx, где dx = x2 - x1
        // c1*t - c2*s = dz, где dz = z2 - z1
        
        double dx = x2 - x1;
        double dz = z2 - z1;
        
        double det_main = a1 * (-c2) - (-a2) * c1;
        double det_t = dx * (-c2) - (-a2) * dz;
        double det_s = a1 * dz - dx * c1;
        
        if (!is_doubleZero(det_main)) {
            double t = det_t / det_main;
            double s = det_s / det_main;
            
            // Проверяем y-координату
            double y_intersect1 = y1 + t * b1;
            double y_intersect2 = y2 + s * b2;
            
            if (is_doubleZero(y_intersect1 - y_intersect2)) {
                return Geo::Point(x1 + t * a1, y_intersect1, z1 + t * c1);
            }
        }
    }
    
    // Пробуем решить через yz-плоскость
    double det_yz = b1 * (-c2) - (-b2) * c1;
    if (!is_doubleZero(det_yz)) {
        // Система уравнений:
        // b1*t - b2*s = dy, где dy = y2 - y1
        // c1*t - c2*s = dz, где dz = z2 - z1
        
        double dy = y2 - y1;
        double dz = z2 - z1;
        
        double det_main = b1 * (-c2) - (-b2) * c1;
        double det_t = dy * (-c2) - (-b2) * dz;
        double det_s = b1 * dz - dy * c1;
        
        if (!is_doubleZero(det_main)) {
            double t = det_t / det_main;
            double s = det_s / det_main;
            
            // Проверяем x-координату
            double x_intersect1 = x1 + t * a1;
            double x_intersect2 = x2 + s * a2;
            
            if (is_doubleZero(x_intersect1 - x_intersect2)) {
                return Geo::Point(x_intersect1, y1 + t * b1, z1 + t * c1);
            }
        }
    }
    
    // Если все методы не сработали
    return Geo::Point(NAN, NAN, NAN);
}

bool Geo::Line::is_parallelTo(const Line& anotherLine) const
{
    return basis_.is_parallel(anotherLine.basis_);
}
