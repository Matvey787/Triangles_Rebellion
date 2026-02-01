module;

#include <cmath>

export module geo_comparator;

const double EPSILON = 1e-7;

/// @brief Checks floating point numbers for zero
/// @tparam T floating point type
/// @return True if number is zero
export template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
is_z(T num1, T num2 = NAN, T num3 = NAN)
{
    if (std::isnan(num2) && std::isnan(num3)) return std::abs(num1) < EPSILON;

    if (std::isnan(num2)) return std::abs(num1)  < EPSILON && std::abs(num3) < EPSILON;

    if (std::isnan(num3)) return std::abs(num1) < EPSILON && std::abs(num2) < EPSILON;

    return std::abs(num1) < EPSILON && std::abs(num2) < EPSILON && std::abs(num3) < EPSILON;
}

/// @brief Checks floating point numbers for above zero
/// @tparam T floating point type
/// @return True if number above zero
export template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
is_az(T num1, T num2 = NAN, T num3 = NAN)
{
    if (std::isnan(num2) && std::isnan(num3))
        return num1 > EPSILON;

    if (std::isnan(num3))
        return num1 > EPSILON && num2 > EPSILON;

    return num1 > EPSILON && num2 > EPSILON && num3 > EPSILON;
}

/// @brief Checks floating point numbers for above or equal zero
/// @tparam T floating point type
/// @return True if number above or equal zero
export template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
is_aez(T num1, T num2 = NAN, T num3 = NAN)
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

/// @brief Checks floating point numbers for below zero
/// @tparam T floating point type
/// @return  True if number below zero
export template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
is_bz(T num1, T num2 = NAN, T num3 = NAN)
{
    if (std::isnan(num2) && std::isnan(num3))
        return num1 < -EPSILON;

    if (std::isnan(num3))
        return num1 < -EPSILON && num2 < -EPSILON;

    return num1 < -EPSILON && num2 < -EPSILON && num3 < -EPSILON;
}

/// @brief Checks floating point numbers for below or equal zero
/// @tparam T floating point type
/// @return True if number below or equal zero
export template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, bool>
is_bez(T num1, T num2 = NAN, T num3 = NAN)
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
