#include "double.h"

const double EPSILON = 0.00001;

bool is_doubleZero(double num1, double num2, double num3)
{
    if (std::isnan(num2) && std::isnan(num3)) return std::abs(num1) < EPSILON;

    if (std::isnan(num2)) return std::abs(num1)  < EPSILON && std::abs(num3) < EPSILON;

    if (std::isnan(num3)) return std::abs(num1) < EPSILON && std::abs(num2) < EPSILON;

    return std::abs(num1) < EPSILON && std::abs(num2) < EPSILON && std::abs(num3) < EPSILON;
}