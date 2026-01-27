#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "geo.h"

namespace BVH {

namespace detail
{


/// @brief Коробка для треугольника в 3D пространстве
struct AABB {
    Geo::Point min_;
    Geo::Point max_;

    AABB() : min_(Geo::Point(1e9, 1e9, 1e9)), max_(Geo::Point(-1e9, -1e9, -1e9)) {}
    AABB(const Geo::Point& pmin, const Geo::Point& pmax) : min_(pmin), max_(pmax) {}

    /// @brief Расширить коробку новой точкой
    void expand(const Geo::Point& p) {
        min_.x_ = std::min(min_.x_, p.x_);
        min_.y_ = std::min(min_.y_, p.y_);
        min_.z_ = std::min(min_.z_, p.z_);
        max_.x_ = std::max(max_.x_, p.x_);
        max_.y_ = std::max(max_.y_, p.y_);
        max_.z_ = std::max(max_.z_, p.z_);
    }

    /// @brief Расширить коробку другим AABB
    void expand(const AABB& other) {
        expand(other.min_);
        expand(other.max_);
    }

    Geo::Point center() const {
        return Geo::Point(
            (min_.x_ + max_.x_) * 0.5,
            (min_.y_ + max_.y_) * 0.5,
            (min_.z_ + max_.z_) * 0.5
        );
    }

    /// @brief Получить наибольшую ось коробки
    int longestAxis() const {
        Geo::Point size(max_.x_ - min_.x_, max_.y_ - min_.y_, max_.z_ - min_.z_);
        if (size.x_ > size.y_ && size.x_ > size.z_) return 0;
        if (size.y_ > size.z_) return 1;
        return 2;
    }

    /// @brief Проверка на пересечение с другой коробкой
    bool intersects(const AABB& other) const {
        return (min_.x_ <= other.max_.x_ && max_.x_ >= other.min_.x_) &&
               (min_.y_ <= other.max_.y_ && max_.y_ >= other.min_.y_) &&
               (min_.z_ <= other.max_.z_ && max_.z_ >= other.min_.z_);
    }
}; // AABB

/// @brief Узел BVH (Bounding Volume Hierarchy)
struct BVHNode {
    AABB box;
    std::unique_ptr<BVHNode> left;
    std::unique_ptr<BVHNode> right;
    std::vector<size_t> triangleIDs;
    bool isLeaf() const { return !left && !right; }
};

} // namespace detail


/// @brief Класс для построения и управления BVH
class BVH {

    std::unique_ptr<detail::BVHNode> build(std::vector<size_t>& indices, size_t maxLeafSize);
    void query(const detail::BVHNode* node, const detail::AABB& box, size_t triIdx, std::vector<size_t>& result) const;
    
public:
    std::unique_ptr<detail::BVHNode> root;
    std::vector<detail::AABB> triBoxes;

    BVH(const std::vector<Geo::Triangle>& triangles, size_t maxLeafSize = 4);

    /// @brief Получить потенциальные пересечения для треугольника
    /// @param triIdx Индекс треугольника
    /// @param result Вектор для хранения индексов треугольников, которые могут пересекаться с данным треугольником
    void potentialOverlaps(size_t triIdx, std::vector<size_t>& result) const;
    
};

}; // namespace BVH