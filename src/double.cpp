#include "double.h"

bool is_doubleZero(double num1, double num2, double num3)
{
    if (std::isnan(num2) && std::isnan(num3)) return std::abs(num1) < 0.001;

    if (std::isnan(num2)) return std::abs(num1)  < 0.001 && std::abs(num3) < 0.001;

    if (std::isnan(num3)) return std::abs(num1) < 0.001 && std::abs(num2) < 0.001;

    return std::abs(num1) < 0.001 && std::abs(num2) < 0.001 && std::abs(num3) < 0.001;
}