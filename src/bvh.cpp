#include "bvh.hpp"
#include <iostream>

namespace BVH {

BVH::BVH(const std::vector<Geo::Triangle>& triangles, size_t maxLeafSize)
{
    triBoxes.resize(triangles.size());
    for (size_t i = 0; i < triangles.size(); ++i) {
        detail::AABB box;
        box.expand(triangles[i].getPoint(0));
        box.expand(triangles[i].getPoint(1));
        box.expand(triangles[i].getPoint(2));
        triBoxes[i] = box;
    }

    std::vector<size_t> indices(triangles.size());

    for (size_t i = 0; i < triangles.size(); ++i) indices[i] = i;

    root = build(indices, maxLeafSize);
}

std::unique_ptr<detail::BVHNode> BVH::build(std::vector<size_t>& indices, size_t maxLeafSize)
{
    // наша общая коробка
    detail::AABB nodeBox;
    for (auto idx : indices)
        nodeBox.expand(triBoxes[idx]);

    // создаем лист, мало треугольников
    if (indices.size() <= maxLeafSize)
    {
        auto leaf = std::make_unique<detail::BVHNode>();
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

    auto node = std::make_unique<detail::BVHNode>();
    node->box = nodeBox;
    node->left = build(left, maxLeafSize);
    node->right = build(right, maxLeafSize);
    return node;
}

void BVH::potentialOverlaps(size_t triIdx, std::vector<size_t>& result) const {
    query(root.get(), triBoxes[triIdx], triIdx, result);
}

void BVH::query(const detail::BVHNode* node,
                const detail::AABB& box,
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

}; // namespace BVH