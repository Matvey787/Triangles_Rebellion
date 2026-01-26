#ifndef DOUBLE_H
#define DOUBLE_H

#include <cmath>

/// @brief Проверка, является ли число нулем
bool is_z(double num1, double num2 = NAN, double num3 = NAN);

/// @brief Проверка, является ли число положительным
bool is_az(double num1, double num2 = NAN, double num3 = NAN);

/// @brief Проверка, является ли число положительным или нулем
bool is_aez(double num1, double num2 = NAN, double num3 = NAN);

/// @brief Проверка, является ли число отрицательным
bool is_bz(double num1, double num2 = NAN, double num3 = NAN);

/// @brief Проверка, является ли число отрицательным или нулем
bool is_bez(double num1, double num2 = NAN, double num3 = NAN);

#endif // DOUBLE_H