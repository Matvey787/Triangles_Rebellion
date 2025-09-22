#include <gtest/gtest.h>
#include "geo.h"

TEST(PointTest, BasicCreation) {
    Point p(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(p.x_, 1.0);
    EXPECT_DOUBLE_EQ(p.y_, 2.0);
    EXPECT_DOUBLE_EQ(p.z_, 3.0);
}

TEST(GeoVectorTest, BasicCreation) {
    Point p1(0.0, 0.0, 0.0);
    Point p2(1.0, 2.0, 3.0);
    GeoVector vec(p1, p2);
    
    EXPECT_DOUBLE_EQ(vec.xProj_, 1.0);
    EXPECT_DOUBLE_EQ(vec.yProj_, 2.0);
    EXPECT_DOUBLE_EQ(vec.zProj_, 3.0);
}

TEST(LineTest, ParallelCheck) {
    Point p1(0.0, 0.0, 0.0);
    Point p2(1.0, 0.0, 0.0);
    Point p3(0.0, 1.0, 0.0);
    Point p4(2.0, 0.0, 0.0);
    
    Line line1(p1, p2);
    Line line2(p1, p4);
    
    EXPECT_TRUE(line1.is_parallelTo(line2));
}

TEST(LineTest, IntersectionTest) {
    Point p1(0.0, 0.0, 0.0);
    Point p2(1.0, 0.0, 0.0);
    Point p3(0.0, 1.0, 0.0);
    Point p4(0.0, 0.0, 1.0);
    
    Line line1(p1, p2);
    Line line2(p1, p3);
    
    Point intersection = line1.intersect(line2);
    
    EXPECT_NEAR(intersection.x_, 0.0, 0.001);
    EXPECT_NEAR(intersection.y_, 0.0, 0.001);
    EXPECT_NEAR(intersection.z_, 0.0, 0.001);
}

TEST(TriangleTest, 3dIntersectionTest) {
    Triangle t1(Point(0, 0, 0), Point(123, -123, 123), Point(1, 2, 3));
    Triangle t2(Point(0, 0, 0), Point(123, -123, 123), Point(1, 2, 3));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dNoIntersectionTest) {
    // Треугольники в разных плоскостях без пересечения
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(0, 0, 1), Point(1, 0, 1), Point(0, 1, 1));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dEdgeIntersectionTest) {
    // Пересечение по ребру в 3D
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
    Triangle t2(Point(1, 0, 0), Point(1, 2, 0), Point(1, 0, 2));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dEdgeIntersectionFullOverlap) {
    // Полное пересечение по стороне (стороны совпадают)
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
    Triangle t2(Point(0, 0, 0), Point(2, 0, 0), Point(1, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dEdgeIntersectionPartialOverlap) {
    // Частичное пересечение по стороне (одна сторона внутри другой)
    Triangle t1(Point(0, 0, 0), Point(4, 0, 0), Point(0, 4, 0));
    Triangle t2(Point(1, 0, 0), Point(3, 0, 0), Point(2, 2, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dEdgeIntersectionOnePointInside) {
    // Одна точка пересечения внутри стороны, другая - вне
    Triangle t1(Point(0, 0, 0), Point(3, 0, 0), Point(0, 3, 0));
    Triangle t2(Point(1, 0, 0), Point(4, 0, 0), Point(2, 2, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Точка (1,0,0) внутри, (4,0,0) вне
}

TEST(TriangleTest, 3dEdgeIntersectionBothPointsOutside) {
    // Обе точки пересечения вне стороны (пересечение только сегментом)
    Triangle t1(Point(1, 0, 0), Point(2, 0, 0), Point(1.5, 1, 0));
    Triangle t2(Point(0, 0, 0), Point(3, 0, 0), Point(1.5, -1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Сегмент (1,0,0)-(2,0,0) внутри (0,0,0)-(3,0,0)
}

TEST(TriangleTest, 3dEdgeIntersectionShortInsideLong) {
    // Короткая сторона полностью внутри длинной
    Triangle t1(Point(0, 0, 0), Point(5, 0, 0), Point(2.5, 3, 0));
    Triangle t2(Point(2, 0, 0), Point(3, 0, 0), Point(2.5, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dEdgeIntersectionLongOutsideShort) {
    // Длинная сторона выходит за пределы короткой с двух сторон
    Triangle t1(Point(2, 0, 0), Point(3, 0, 0), Point(2.5, 1, 0));
    Triangle t2(Point(0, 0, 0), Point(5, 0, 0), Point(2.5, -1, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dEdgeIntersectionTouchingAtEndpoint) {
    // Касание только конечной точкой
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(1, 1, 0));
    Triangle t2(Point(2, 0, 0), Point(4, 0, 0), Point(3, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Точка (2,0,0) общая
}

TEST(TriangleTest, 3dEdgeIntersectionAlmostTouching) {
    // Почти касание, но без пересечения
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(1, 1, 0));
    Triangle t2(Point(2.5, 0, 0), Point(4, 0, 0), Point(3, 1, 0));
    EXPECT_FALSE(t1.is_intersect(t2)); // Нет пересечения
}

TEST(TriangleTest, 3dEdgeIntersectionCollinearButSeparate) {
    // Коллинеарные стороны, без пересечения
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0.5, 1, 0));
    Triangle t2(Point(2, 0, 0), Point(3, 0, 0), Point(2.5, 1, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dEdgeIntersectionPerpendicular) {
    // Пересечение перпендикулярных сторон
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(1, 1, 0));
    Triangle t2(Point(1, -1, 0), Point(1, 1, 0), Point(0, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Пересекаются в точке (1,0,0)
}

TEST(TriangleTest, 3dEdgeIntersectionSkewed) {
    // Пересечение наклонных сторон в 3D
    Triangle t1(Point(0, 0, 0), Point(2, 2, 2), Point(0, 2, 0));
    Triangle t2(Point(1, 1, 1), Point(3, 3, 3), Point(1, 3, 1));
    EXPECT_TRUE(t1.is_intersect(t2)); // Стороны лежат на одной линии в 3D
}

TEST(TriangleTest, 3dEdgeIntersectionSinglePointOnly) {
    // Пересечение только в одной точке стороны
    Triangle t1(Point(0, 0, 0), Point(3, 0, 0), Point(0, 3, 0));
    Triangle t2(Point(2, 0, 0), Point(4, 0, 0), Point(3, 2, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Только точка (2,0,0) на границе
}

TEST(TriangleTest, 3dEdgeIntersectionWithGap) {
    // Стороны на одной линии, но с зазором
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0.5, 1, 0));
    Triangle t2(Point(1.1, 0, 0), Point(2, 0, 0), Point(1.5, 1, 0));
    EXPECT_FALSE(t1.is_intersect(t2)); // Зазор 0.1 между сторонами
}

TEST(TriangleTest, 3dPointIntersectionTest) {
    // Пересечение одной точкой в 3D
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
    Triangle t2(Point(1, 1, 0), Point(3, 1, 0), Point(1, 3, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Точка (1,1,0) общая
}

TEST(TriangleTest, 2dXYPlaneIntersectionTest) {
    // Пересечение в плоскости XY
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
    Triangle t2(Point(1, 1, 0), Point(3, 1, 0), Point(1, 3, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 2dXZPlaneIntersectionTest) {
    // Пересечение в плоскости XZ
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 0, 2));
    Triangle t2(Point(1, 0, 1), Point(3, 0, 1), Point(1, 0, 3));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 2dYZPlaneIntersectionTest) {
    // Пересечение в плоскости YZ
    Triangle t1(Point(0, 0, 0), Point(0, 2, 0), Point(0, 0, 2));
    Triangle t2(Point(0, 1, 1), Point(0, 3, 1), Point(0, 1, 3));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 2dArbitraryPlaneIntersectionTest) {
    // Пересечение в произвольной плоскости
    Triangle t1(Point(0, 0, 0), Point(2, 1, 1), Point(0, 2, 2));
    Triangle t2(Point(1, 0.5, 0.5), Point(3, 1.5, 1.5), Point(1, 2.5, 2.5));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 2dXYPlaneNoIntersectionTest) {
    // Нет пересечения в плоскости XY
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(2, 2, 0), Point(3, 2, 0), Point(2, 3, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 2dXZPlaneNoIntersectionTest) {
    // Нет пересечения в плоскости XZ
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 0, 1));
    Triangle t2(Point(2, 0, 2), Point(3, 0, 2), Point(2, 0, 3));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 2dYZPlaneNoIntersectionTest) {
    // Нет пересечения в плоскости YZ
    Triangle t1(Point(0, 0, 0), Point(0, 1, 0), Point(0, 0, 1));
    Triangle t2(Point(0, 2, 2), Point(0, 3, 2), Point(0, 2, 3));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 2dEdgeIntersectionTest) {
    // Пересечение по стороне в 2D
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
    Triangle t2(Point(0, 0, 0), Point(0, 2, 0), Point(-1, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общее ребро (0,0,0)-(0,2,0)
}

TEST(TriangleTest, 2dPointIntersectionTest) {
    // Пересечение одной точкой в 2D
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
    Triangle t2(Point(1, 1, 0), Point(3, 1, 0), Point(1, 3, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Только точка (1,1,0)
}

TEST(TriangleTest, 3dCompleteOverlapTest) {
    // Полное совпадение треугольников
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dParallelPlanesNoIntersectionTest) {
    // Параллельные плоскости без пересечения
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(0, 0, 1), Point(1, 0, 1), Point(0, 1, 1));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dTouchingAtPointTest) {
    // Касание в одной точке в 3D
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(0, 0, 1), Point(0, 1, 1), Point(1, 0, 1));
    // Точка (0,0,0) близка, но не пересекает
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(LineTest, SkewLines) {
    // Перекрестные линии (не пересекаются и не параллельны)
    Point p1(0.0, 0.0, 0.0);
    Point p2(1.0, 0.0, 0.0); // Линия вдоль оси X
    Point p3(0.0, 0.0, 1.0);
    Point p4(0.0, 1.0, 1.0); // Линия вдоль оси Y на высоте z=1
    
    Line line1(p1, p2); // Направляющий вектор: (1,0,0)
    Line line2(p3, p4); // Направляющий вектор: (0,1,0)
    
    EXPECT_FALSE(line1.is_parallelTo(line2)); // Векторы (1,0,0) и (0,1,0) не параллельны
    Point intersection = line1.intersect(line2);
    EXPECT_TRUE(std::isnan(intersection.x_) && std::isnan(intersection.y_) && std::isnan(intersection.z_)); // Нет пересечения
}

TEST(TriangleTest, 3dVertexInsideTriangle) {
    // Вершина одного треугольника внутри другого
    Triangle t1(Point(0, 0, 0), Point(4, 0, 0), Point(0, 4, 0));
    Triangle t2(Point(1, 1, 0), Point(5, 0, 0), Point(0, 5, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Точка (1,1,0) внутри t1
}

TEST(TriangleTest, 3dOneTriangleInsideAnother) {
    // Один треугольник полностью внутри другого
    Triangle t2(Point(1, 1, 0), Point(2, 1, 0), Point(1, 2, 0));
    Triangle t1(Point(0, 0, 0), Point(5, 0, 0), Point(0, 5, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // t2 полностью внутри t1
}

TEST(TriangleTest, 3dCoplanarNoIntersection) {
    // Треугольники в одной плоскости, но не пересекаются
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(2, 2, 0), Point(3, 2, 0), Point(2, 3, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dCoplanarEdgeTouching) {
    // Треугольники в одной плоскости, касаются по ребру
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 2, 0));
    Triangle t2(Point(2, 0, 0), Point(4, 0, 0), Point(2, 2, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общее ребро (2,0,0)-(2,2,0)
}

TEST(TriangleTest, 3dNonCoplanarEdgeIntersection) {
    // Пересечение ребер в 3D, треугольники не в одной плоскости
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 0, 2));
    Triangle t2(Point(1, -1, 1), Point(1, 1, 1), Point(3, 0, 1));
    EXPECT_TRUE(t1.is_intersect(t2)); // Пересечение в точке (1,0,1)
}

TEST(TriangleTest, 3dParallelPlanesTouchingEdge) {
    // Параллельные плоскости, касаются по ребру
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(0, 0, 1), Point(2, 0, 1), Point(0, 1, 1));
    EXPECT_FALSE(t1.is_intersect(t2)); // Параллельные плоскости без пересечения
}

TEST(TriangleTest, 2dDegenerateTriangleIntersection) {
    // Вырожденный треугольник (линия) пересекает треугольник
    Triangle t1(Point(0, 0, 0), Point(2, 0, 0), Point(2, 0, 0)); // Вырожденный в линию
    Triangle t2(Point(1, -1, 0), Point(1, 1, 0), Point(3, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Линия пересекает треугольник
}

TEST(TriangleTest, 3dDegenerateTriangleNoIntersection) {
    // Вырожденный треугольник (линия) не пересекает треугольник
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(1, 0, 0)); // Вырожденный в линию
    Triangle t2(Point(2, 2, 0), Point(3, 2, 0), Point(2, 3, 0));
    EXPECT_FALSE(t1.is_intersect(t2)); // Нет пересечения
}

TEST(TriangleTest, VertexCoincidenceOnly) {
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(0, 0, 0), Point(-1, 0, 0), Point(0, -1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общая вершина (0,0,0)
}

TEST(TriangleTest, TwoVerticesCoincide) {
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(0, 0, 0), Point(1, 0, 0), Point(0, -1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общая сторона (0,0,0)-(1,0,0)
}

TEST(TriangleTest, SameTriangleDifferentOrder) {
    Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(0, 1, 0));
    Triangle t2(Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, SideCompletelyInsideOtherSide) {
    Triangle t1(Point(0, 0, 0), Point(6, 0, 0), Point(0, 6, 0));
    Triangle t2(Point(2, 0, 0), Point(4, 0, 0), Point(3, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, VertexOnOtherSide) {
    Triangle t1(Point(0, 0, 0), Point(5, 0, 0), Point(0, 5, 0));
    Triangle t2(Point(2, 0, 0), Point(3, 1, 0), Point(4, 2, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Точка (2,0,0) лежит на ребре t1
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
