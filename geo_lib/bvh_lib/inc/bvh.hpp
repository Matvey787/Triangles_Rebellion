#pragma once

import geo_triangle;

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

namespace BVH {

namespace detail
{

template <typename T>
struct AABB {
    Geo::Point<T> min_;
    Geo::Point<T> max_;

    AABB() : min_(Geo::Point<T>(1e9, 1e9, 1e9)), max_(Geo::Point<T>(-1e9, -1e9, -1e9)) {}
    AABB(const Geo::Point<T>& pmin, const Geo::Point<T>& pmax) : min_(pmin), max_(pmax) {}

    /// @brief Расширить коробку новой точкой
    void
    expand(const Geo::Point<T>& p) {
        min_.setX(std::min(min_[0], p[0]));
        min_.setY(std::min(min_[1], p[1]));
        min_.setZ(std::min(min_[2], p[2]));
        max_.setX(std::max(max_[0], p[0]));
        max_.setY(std::max(max_[1], p[1]));
        max_.setZ(std::max(max_[2], p[2]));
    }

    /// @brief Расширить коробку другим AABB
    void
    expand(const AABB<T>& other) {
        expand(other.min_);
        expand(other.max_);
    }

    Geo::Point<T>
    center() const {
        return Geo::Point<T>(
            (min_.getX() + max_.getX()) * 0.5,
            (min_.getY() + max_.getY()) * 0.5,
            (min_.getZ() + max_.getZ()) * 0.5
        );
    }

    /// @brief Получить наибольшую ось коробки
    int longestAxis() const
    {
        Geo::Point size(max_[0] - min_[0],
                        max_[1] - min_[1],
                        max_[2] - min_[2]);

        if (size.getX() > size.getY() && size.getX() > size.getZ()) return 0;
        if (size.getY() > size.getZ()) return 1;
        return 2;
    }

    /// @brief Проверка на пересечение с другой коробкой
    bool intersects(const AABB<T>& other) const {
        return (min_.getX() <= other.max_.getX() && max_.getX() >= other.min_.getX()) &&
               (min_.getY() <= other.max_.getY() && max_.getY() >= other.min_.getY()) &&
               (min_.getZ() <= other.max_.getZ() && max_.getZ() >= other.min_.getZ());
    }
}; // AABB

/// @brief Узел BVH (Bounding Volume Hierarchy)
template <typename T>
struct BVHNode {
    AABB<T> box;
    std::unique_ptr<BVHNode<T>> left;
    std::unique_ptr<BVHNode<T>> right;
    std::vector<size_t> triangleIDs;
    bool isLeaf() const { return !left && !right; }
};

} // namespace detail

template <typename T>
class BVH {

    std::unique_ptr<detail::BVHNode<T>> build(std::vector<size_t>& indices, size_t maxLeafSize)
    {
        // наша общая коробка
        detail::AABB<T> nodeBox;
        for (auto idx : indices)
            nodeBox.expand(triBoxes[idx]);

        // создаем лист, мало треугольников
        if (indices.size() <= maxLeafSize)
        {
            auto leaf = std::make_unique<detail::BVHNode<T>>();
            leaf->box = nodeBox;
            leaf->triangleIDs = indices;
            return leaf;
        }
        size_t axis = nodeBox.longestAxis();

        // сортируем треугольники, чтобы далее разместить в две новые коробки
        auto comparatorByAxis = [&](size_t a, size_t b) {
            return triBoxes[a].center()[axis] < triBoxes[b].center()[axis];
        };
        std::sort(indices.begin(), indices.end(), comparatorByAxis);

        size_t mid = indices.size() / 2;

        std::vector<size_t> left(indices.begin(), indices.begin() + mid);
        std::vector<size_t> right(indices.begin() + mid, indices.end());

        auto node = std::make_unique<detail::BVHNode<T>>();
        node->box = nodeBox;
        node->left = build(left, maxLeafSize);
        node->right = build(right, maxLeafSize);
        return node;
    }
    void
    query(const detail::BVHNode<T>* node,
          const detail::AABB<T>& box,
          size_t triIdx,
          std::vector<size_t>& result) const
    {
        if (!node->box.intersects(box)) return;

        if (node->isLeaf())
        {
            for (auto j : node->triangleIDs) if (j != triIdx) result.push_back(j);

            return;
        }

        if (node->left) query(node->left.get(), box, triIdx, result);
        if (node->right) query(node->right.get(), box, triIdx, result);
    }
    
public:
    std::unique_ptr<detail::BVHNode<T>> root;
    std::vector<detail::AABB<T>> triBoxes;

    BVH(const std::vector<Geo::Triangle<T>>& triangles, size_t maxLeafSize = 4)
    {
        triBoxes.resize(triangles.size());
        for (size_t i = 0; i < triangles.size(); ++i) {
            detail::AABB<T> box;
            box.expand(triangles[i].getPoint(0));
            box.expand(triangles[i].getPoint(1));
            box.expand(triangles[i].getPoint(2));
            triBoxes[i] = box;
        }

        std::vector<size_t> indices(triangles.size());

        for (size_t i = 0; i < triangles.size(); ++i) indices[i] = i;

        root = build(indices, maxLeafSize);
    }

    /// @brief Получить потенциальные пересечения для треугольника
    /// @param triIdx Индекс треугольника
    /// @param result Вектор для хранения индексов треугольников, которые могут пересекаться с данным треугольником
    void
    potentialOverlaps(size_t triIdx, std::vector<size_t>& result) const
    {
        query(root.get(), triBoxes[triIdx], triIdx, result);
    }

    void
    potentianlOverlapsPrint(size_t triIdx, const std::vector<Geo::Triangle<T>>& triangles) const
    {
        std::vector<size_t> potentialOverlapsTrianglesIndexes;
        potentialOverlaps(triIdx, potentialOverlapsTrianglesIndexes);

        std::cout << "===================================================================\n"; 
        std::cout << "Potential overlaps with triangle (" << triIdx <<") " << triangles[triIdx] << ":\n";
        for (auto i : potentialOverlapsTrianglesIndexes)
        {
            std::cout << triangles[i] << "\n";
        }
        std::cout << "===================================================================\n"; 
    }
};

}; // namespace BVH