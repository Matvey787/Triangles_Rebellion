#include "double.h"

const double EPSILON = 1e-7;

// double is zero
bool is_z(double num1, double num2, double num3)
{
    if (std::isnan(num2) && std::isnan(num3)) return std::abs(num1) < EPSILON;

    if (std::isnan(num2)) return std::abs(num1)  < EPSILON && std::abs(num3) < EPSILON;

    if (std::isnan(num3)) return std::abs(num1) < EPSILON && std::abs(num2) < EPSILON;

    return std::abs(num1) < EPSILON && std::abs(num2) < EPSILON && std::abs(num3) < EPSILON;
}


// above zero
bool is_az(double num1, double num2, double num3)
{
    if (std::isnan(num2) && std::isnan(num3))
        return num1 > EPSILON;

    if (std::isnan(num3))
        return num1 > EPSILON && num2 > EPSILON;

    return num1 > EPSILON && num2 > EPSILON && num3 > EPSILON;
}

// above or equal zero
bool is_aez(double num1, double num2, double num3)
{
    if (std::isnan(num2) && std::isnan(num3))
        return (num1 > EPSILON || std::abs(num1) < EPSILON);

    if (std::isnan(num3))
        return (num1 > EPSILON || std::abs(num1) < EPSILON) &&
               (num2 > EPSILON || std::abs(num2) < EPSILON);

    return (num1 > EPSILON || std::abs(num1) < EPSILON) &&
           (num2 > EPSILON || std::abs(num2) < EPSILON) &&
           (num3 > EPSILON || std::abs(num3) < EPSILON);
}


// below zero
bool is_bz(double num1, double num2, double num3)
{
    if (std::isnan(num2) && std::isnan(num3))
        return num1 < -EPSILON;

    if (std::isnan(num3))
        return num1 < -EPSILON && num2 < -EPSILON;

    return num1 < -EPSILON && num2 < -EPSILON && num3 < -EPSILON;
}
// below or equal zero
bool is_bez(double num1, double num2, double num3)
{
    if (std::isnan(num2) && std::isnan(num3))
        return (num1 < -EPSILON || std::abs(num1) < EPSILON);

    if (std::isnan(num3))
        return (num1 < -EPSILON || std::abs(num1) < EPSILON) &&
               (num2 < -EPSILON || std::abs(num2) < EPSILON);

    return (num1 < -EPSILON || std::abs(num1) < EPSILON) &&
           (num2 < -EPSILON || std::abs(num2) < EPSILON) &&
           (num3 < -EPSILON || std::abs(num3) < EPSILON);
}