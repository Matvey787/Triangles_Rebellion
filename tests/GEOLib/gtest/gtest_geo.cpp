#include <gtest/gtest.h>

import geo_lib;

using namespace Geo;

// TEST(PointTest, BasicCreation) {
//     Point<double> p(1.0, 2.0, 3.0);
//     EXPECT_DOUBLE_EQ(p.x_, 1.0);
//     EXPECT_DOUBLE_EQ(p.y_, 2.0);
//     EXPECT_DOUBLE_EQ(p.z_, 3.0);
// }

// TEST(VectorTest, BasicCreation) {
//     Point<double> p1(0.0, 0.0, 0.0);
//     Point<double> p2(1.0, 2.0, 3.0);
//     Vector<double> vec(p1, p2);
    
//     EXPECT_DOUBLE_EQ(vec.xProj_, 1.0);
//     EXPECT_DOUBLE_EQ(vec.yProj_, 2.0);
//     EXPECT_DOUBLE_EQ(vec.zProj_, 3.0);
// }

// TEST(LineTest, ParallelCheck) {
//     Point<double> p1(0.0, 0.0, 0.0);
//     Point<double> p2(1.0, 0.0, 0.0);
//     Point<double> p3(0.0, 1.0, 0.0);
//     Point<double> p4(2.0, 0.0, 0.0);
    
//     Line line1(p1, p2);
//     Line line2(p1, p4);
    
//     EXPECT_TRUE(line1.is_parallelTo(line2));
// }

// TEST(LineTest, IntersectionTest) {
//     Point<double> p1(0.0, 0.0, 0.0);
//     Point<double> p2(1.0, 0.0, 0.0);
//     Point<double> p3(0.0, 1.0, 0.0);
//     Point<double> p4(0.0, 0.0, 1.0);
    
//     Line line1(p1, p2);
//     Line line2(p1, p3);
    
//     Point<double> intersection = line1.intersect(line2);
    
//     EXPECT_NEAR(intersection.x_, 0.0, 0.001);
//     EXPECT_NEAR(intersection.y_, 0.0, 0.001);
//     EXPECT_NEAR(intersection.z_, 0.0, 0.001);
// }

// TEST(TriangleTest, 3dIntersectionTest) {
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(123, -123, 123), Point<double>(1, 2, 3));
//     Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(123, -123, 123), Point<double>(1, 2, 3));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dNoIntersectionTest) {
//     // Треугольники в разных плоскостях без пересечения
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
//     Triangle<double> t2(Point<double>(0, 0, 1), Point<double>(1, 0, 1), Point<double>(0, 1, 1));
//     EXPECT_FALSE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dEdgeIntersectionTest) {
//     // Пересечение по ребру в 3D
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
//     Triangle<double> t2(Point<double>(1, 0, 0), Point<double>(1, 2, 0), Point<double>(1, 0, 2));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dEdgeIntersectionFullOverlap) {
//     // Полное пересечение по стороне (стороны совпадают)
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
//     Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(1, 1, 0));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dEdgeIntersectionPartialOverlap) {
//     // Частичное пересечение по стороне (одна сторона внутри другой)
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(4, 0, 0), Point<double>(0, 4, 0));
//     Triangle<double> t2(Point<double>(1, 0, 0), Point<double>(3, 0, 0), Point<double>(2, 2, 0));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dEdgeIntersectionOnePointInside) {
//     // Одна точка пересечения внутри стороны, другая - вне
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
//     Triangle<double> t2(Point<double>(1, 0, 0), Point<double>(4, 0, 0), Point<double>(2, 2, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Точка (1,0,0) внутри, (4,0,0) вне
// }

// TEST(TriangleTest, 3dEdgeIntersectionBothPointsOutside) {
//     // Обе точки пересечения вне стороны (пересечение только сегментом)
//     Triangle<double> t1(Point<double>(1, 0, 0), Point<double>(2, 0, 0), Point<double>(1.5, 1, 0));
//     Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(1.5, -1, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Сегмент (1,0,0)-(2,0,0) внутри (0,0,0)-(3,0,0)
// }

// TEST(TriangleTest, 3dEdgeIntersectionShortInsideLong) {
//     // Короткая сторона полностью внутри длинной
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(5, 0, 0), Point<double>(2.5, 3, 0));
//     Triangle<double> t2(Point<double>(2, 0, 0), Point<double>(3, 0, 0), Point<double>(2.5, 1, 0));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dEdgeIntersectionLongOutsideShort) {
//     // Длинная сторона выходит за пределы короткой с двух сторон
//     Triangle<double> t1(Point<double>(2, 0, 0), Point<double>(3, 0, 0), Point<double>(2.5, 1, 0));
//     Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(5, 0, 0), Point<double>(2.5, -1, 0));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dEdgeIntersectionTouchingAtEndPoint) {
//     // Касание только конечной точкой
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(1, 1, 0));
//     Triangle<double> t2(Point<double>(2, 0, 0), Point<double>(4, 0, 0), Point<double>(3, 1, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Точка (2,0,0) общая
// }

// TEST(TriangleTest, 3dEdgeIntersectionAlmostTouching) {
//     // Почти касание, но без пересечения
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(1, 1, 0));
//     Triangle<double> t2(Point<double>(2.5, 0, 0), Point<double>(4, 0, 0), Point<double>(3, 1, 0));
//     EXPECT_FALSE(t1.is_intersect(t2)); // Нет пересечения
// }

// TEST(TriangleTest, 3dEdgeIntersectionCollinearButSeparate) {
//     // Коллинеарные стороны, без пересечения
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0.5, 1, 0));
//     Triangle<double> t2(Point<double>(2, 0, 0), Point<double>(3, 0, 0), Point<double>(2.5, 1, 0));
//     EXPECT_FALSE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dEdgeIntersectionPerpendicular) {
//     // Пересечение перпендикулярных сторон
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(1, 1, 0));
//     Triangle<double> t2(Point<double>(1, -1, 0), Point<double>(1, 1, 0), Point<double>(0, 0, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Пересекаются в точке (1,0,0)
// }

// TEST(TriangleTest, 3dEdgeIntersectionSkewed) {
//     // Пересечение наклонных сторон в 3D
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 2, 2), Point<double>(0, 2, 0));
//     Triangle<double> t2(Point<double>(1, 1, 1), Point<double>(3, 3, 3), Point<double>(1, 3, 1));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Стороны лежат на одной линии в 3D
// }

// TEST(TriangleTest, 3dEdgeIntersectionSinglePointOnly) {
//     // Пересечение только в одной точке стороны
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
//     Triangle<double> t2(Point<double>(2, 0, 0), Point<double>(4, 0, 0), Point<double>(3, 2, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Только точка (2,0,0) на границе
// }

// TEST(TriangleTest, 3dEdgeIntersectionWithGap) {
//     // Стороны на одной линии, но с зазором
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0.5, 1, 0));
//     Triangle<double> t2(Point<double>(1.1, 0, 0), Point<double>(2, 0, 0), Point<double>(1.5, 1, 0));
//     EXPECT_FALSE(t1.is_intersect(t2)); // Зазор 0.1 между сторонами
// }

// TEST(TriangleTest, 3dPointIntersectionTest) {
//     // Пересечение одной точкой в 3D
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
//     Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(3, 1, 0), Point<double>(1, 3, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Точка (1,1,0) общая
// }

// TEST(TriangleTest, 2dXYPlaneIntersectionTest1) {
//     // Пересечение в плоскости XY
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(81.8857, 0, 0), Point<double>(0, 78.4631, 0));
//     Triangle<double> t2(Point<double>(0.2058, 12.201, 0), Point<double>(155.092, 12.201, 0), Point<double>(73.2058, 90.6641, 0));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dXYPlaneIntersectionTest2) {
//     // Пересечение в плоскости XY
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
//     Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(3, 1, 0), Point<double>(1, 3, 0));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dXZPlaneIntersectionTest) {
//     // Пересечение в плоскости XZ
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 0, 2));
//     Triangle<double> t2(Point<double>(1, 0, 1), Point<double>(3, 0, 1), Point<double>(1, 0, 3));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dYZPlaneIntersectionTest) {
//     // Пересечение в плоскости YZ
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 2, 0), Point<double>(0, 0, 2));
//     Triangle<double> t2(Point<double>(0, 1, 1), Point<double>(0, 3, 1), Point<double>(0, 1, 3));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dArbitraryPlaneIntersectionTest) {
//     // Пересечение в произвольной плоскости
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 1, 1), Point<double>(0, 2, 2));
//     Triangle<double> t2(Point<double>(1, 0.5, 0.5), Point<double>(3, 1.5, 1.5), Point<double>(1, 2.5, 2.5));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dXYPlaneNoIntersectionTest) {
//     // Нет пересечения в плоскости XY
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
//     Triangle<double> t2(Point<double>(2, 2, 0), Point<double>(3, 2, 0), Point<double>(2, 3, 0));
//     EXPECT_FALSE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dXZPlaneNoIntersectionTest) {
//     // Нет пересечения в плоскости XZ
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 0, 1));
//     Triangle<double> t2(Point<double>(2, 0, 2), Point<double>(3, 0, 2), Point<double>(2, 0, 3));
//     EXPECT_FALSE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dYZPlaneNoIntersectionTest) {
//     // Нет пересечения в плоскости YZ
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 1, 0), Point<double>(0, 0, 1));
//     Triangle<double> t2(Point<double>(0, 2, 2), Point<double>(0, 3, 2), Point<double>(0, 2, 3));
//     EXPECT_FALSE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 2dEdgeIntersectionTest) {
//     // Пересечение по стороне в 2D
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
//     Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(0, 2, 0), Point<double>(-1, 1, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Общее ребро (0,0,0)-(0,2,0)
// }

// TEST(TriangleTest, 2dPointIntersectionTest) {
//     // Пересечение одной точкой в 2D
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
//     Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(3, 1, 0), Point<double>(1, 3, 0));
//     EXPECT_TRUE(t1.is_intersect(t2)); // Только точка (1,1,0)
// }

// TEST(TriangleTest, 3dCompleteOverlapTest) {
//     // Полное совпадение треугольников
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
//     Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
//     EXPECT_TRUE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dParallelPlanesNoIntersectionTest) {
//     // Параллельные плоскости без пересечения
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
//     Triangle<double> t2(Point<double>(0, 0, 1), Point<double>(1, 0, 1), Point<double>(0, 1, 1));
//     EXPECT_FALSE(t1.is_intersect(t2));
// }

// TEST(TriangleTest, 3dTouchingAtPointTest) {
//     // Касание в одной точке в 3D
//     Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
//     Triangle<double> t2(Point<double>(0, 0, 1), Point<double>(0, 1, 1), Point<double>(1, 0, 1));
//     // Точка (0,0,0) близка, но не пересекает
//     EXPECT_FALSE(t1.is_intersect(t2));
// }

// TEST(LineTest, SkewLines) {
//     // Перекрестные линии (не пересекаются и не параллельны)
//     Point<double> p1(0.0, 0.0, 0.0);
//     Point<double> p2(1.0, 0.0, 0.0); // Линия вдоль оси X
//     Point<double> p3(0.0, 0.0, 1.0);
//     Point<double> p4(0.0, 1.0, 1.0); // Линия вдоль оси Y на высоте z=1
    
//     Line line1(p1, p2); // Направляющий вектор: (1,0,0)
//     Line line2(p3, p4); // Направляющий вектор: (0,1,0)
    
//     EXPECT_FALSE(line1.is_parallelTo(line2)); // Векторы (1,0,0) и (0,1,0) не параллельны
//     Point<double> intersection = line1.intersect(line2);
//     EXPECT_TRUE(std::isnan(intersection.x_) && std::isnan(intersection.y_) && std::isnan(intersection.z_)); // Нет пересечения
// }

TEST(TriangleTest, 3dVertexInsideTriangle) {
    // Вершина одного треугольника внутри другого
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(4, 0, 0), Point<double>(0, 4, 0));
    Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(5, 0, 0), Point<double>(0, 5, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Точка (1,1,0) внутри t1
}

TEST(TriangleTest, 3dOneTriangleInsideAnother) {
    // Один треугольник полностью внутри другого
    Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(2, 1, 0), Point<double>(1, 2, 0));
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(5, 0, 0), Point<double>(0, 5, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // t2 полностью внутри t1
}

TEST(TriangleTest, 3dCoplanarNoIntersection) {
    // Треугольники в одной плоскости, но не пересекаются
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
    Triangle<double> t2(Point<double>(2, 2, 0), Point<double>(3, 2, 0), Point<double>(2, 3, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(TriangleTest, 3dCoplanarEdgeTouching) {
    // Треугольники в одной плоскости, касаются по ребру
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
    Triangle<double> t2(Point<double>(2, 0, 0), Point<double>(4, 0, 0), Point<double>(2, 2, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общее ребро (2,0,0)-(2,2,0)
}

TEST(TriangleTest, 3dNonCoplanarEdgeIntersection) {
    // Пересечение ребер в 3D, треугольники не в одной плоскости
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 0, 2));
    Triangle<double> t2(Point<double>(1, -1, 1), Point<double>(1, 1, 1), Point<double>(3, 0, 1));
    EXPECT_TRUE(t1.is_intersect(t2)); // Пересечение в точке (1,0,1)
}

TEST(TriangleTest, 3dParallelPlanesTouchingEdge) {
    // Параллельные плоскости, касаются по ребру
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 1, 0));
    Triangle<double> t2(Point<double>(0, 0, 1), Point<double>(2, 0, 1), Point<double>(0, 1, 1));
    EXPECT_FALSE(t1.is_intersect(t2)); // Параллельные плоскости без пересечения
}

TEST(TriangleTest, 2dDegenerateTriangleIntersection) {
    // Вырожденный треугольник (линия) пересекает треугольник
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(2, 0, 0)); // Вырожденный в линию
    Triangle<double> t2(Point<double>(1, -1, 0), Point<double>(1, 1, 0), Point<double>(3, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Линия пересекает треугольник
}

TEST(TriangleTest, 3dDegenerateTriangleNoIntersection) {
    // Вырожденный треугольник (линия) не пересекает треугольник
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(1, 0, 0)); // Вырожденный в линию
    Triangle<double> t2(Point<double>(2, 2, 0), Point<double>(3, 2, 0), Point<double>(2, 3, 0));
    EXPECT_FALSE(t1.is_intersect(t2)); // Нет пересечения
}

TEST(TriangleTest, VertexCoincidenceOnly) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
    Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(-1, 0, 0), Point<double>(0, -1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общая вершина (0,0,0)
}

TEST(TriangleTest, TwoVerticesCoincide) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
    Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, -1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общая сторона (0,0,0)-(1,0,0)
}

TEST(TriangleTest, SameTriangleDifferentOrder) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
    Triangle<double> t2(Point<double>(1, 0, 0), Point<double>(0, 1, 0), Point<double>(0, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, SideCompletelyInsideOtherSide) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(6, 0, 0), Point<double>(0, 6, 0));
    Triangle<double> t2(Point<double>(2, 0, 0), Point<double>(4, 0, 0), Point<double>(3, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(TriangleTest, VertexOnOtherSide) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(5, 0, 0), Point<double>(0, 5, 0));
    Triangle<double> t2(Point<double>(2, 0, 0), Point<double>(3, 1, 0), Point<double>(4, 2, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Точка (2,0,0) лежит на ребре t1
}

TEST(TriangleTest, 2dXYPlaneIntersectionTest) {
    // Пересечение в плоскости XY
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
    Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(3, 1, 0), Point<double>(1, 3, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dXZPlaneIntersectionTest) {
    // Пересечение в плоскости XZ
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 0, 2));
    Triangle<double> t2(Point<double>(1, 0, 1), Point<double>(3, 0, 1), Point<double>(1, 0, 3));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dYZPlaneIntersectionTest) {
    // Пересечение в плоскости YZ
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 2, 0), Point<double>(0, 0, 2));
    Triangle<double> t2(Point<double>(0, 1, 1), Point<double>(0, 3, 1), Point<double>(0, 1, 3));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dArbitraryPlaneIntersectionTest) {
    // Пересечение в произвольной плоскости
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 1, 1), Point<double>(0, 2, 2));
    Triangle<double> t2(Point<double>(1, 0.5, 0.5), Point<double>(3, 1.5, 1.5), Point<double>(1, 2.5, 2.5));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dXYPlaneNoIntersectionTest) {
    // Нет пересечения в плоскости XY
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 1, 0));
    Triangle<double> t2(Point<double>(2, 2, 0), Point<double>(3, 2, 0), Point<double>(2, 3, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dXZPlaneNoIntersectionTest) {
    // Нет пересечения в плоскости XZ
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(1, 0, 0), Point<double>(0, 0, 1));
    Triangle<double> t2(Point<double>(2, 0, 2), Point<double>(3, 0, 2), Point<double>(2, 0, 3));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dYZPlaneNoIntersectionTest) {
    // Нет пересечения в плоскости YZ
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 1, 0), Point<double>(0, 0, 1));
    Triangle<double> t2(Point<double>(0, 2, 2), Point<double>(0, 3, 2), Point<double>(0, 2, 3));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dEdgeIntersectionTest) {
    // Пересечение по стороне в 2D
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
    Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(0, 2, 0), Point<double>(-1, 1, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общее ребро (0,0,0)-(0,2,0)
}

TEST(CHATGPTTriangleTest, 2dPointIntersectionTest) {
    // Пересечение одной точкой в 2D
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(2, 0, 0), Point<double>(0, 2, 0));
    Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(3, 1, 0), Point<double>(1, 3, 0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Только точка (1,1,0)
}

// ===== Дополнительные тесты =====

TEST(CHATGPTTriangleTest, 2dXYPlaneFractionalIntersectionTest) {
    Triangle<double> t1(Point<double>(0.1234, 0.5678, 0), Point<double>(5.4321, 0.5678, 0), Point<double>(0.1234, 4.8765, 0));
    Triangle<double> t2(Point<double>(2.0000, 1.0000, 0), Point<double>(6.0000, 1.5000, 0), Point<double>(2.5000, 5.0000, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dXZPlaneLargeCoordinatesIntersectionTest) {
    Triangle<double> t1(Point<double>(100.0000, 0, 100.0000), Point<double>(500.0000, 0, 100.0000), Point<double>(100.0000, 0, 500.0000));
    Triangle<double> t2(Point<double>(300.0000, 0, 200.0000), Point<double>(700.0000, 0, 200.0000), Point<double>(300.0000, 0, 600.0000));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dYZPlaneFractionalNoIntersectionTest) {
    Triangle<float> t1(Point<float>(0, 0.1111, 0.2222), Point<float>(0, 2.3333, 0.2222), Point<float>(0, 0.1111, 2.4444));
    Triangle<float> t2(Point<float>(0, 5.5555, 5.6666), Point<float>(0, 6.7777, 5.6666), Point<float>(0, 5.5555, 6.8888));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dXYPlaneCloseButNoIntersectionTest) {
    Triangle<float> t1(Point<float>(10.1234, 10.1234, 0), Point<float>(15.5678, 10.1234, 0), Point<float>(10.1234, 15.5678, 0));
    Triangle<float> t2(Point<float>(15.6789, 15.6789, 0), Point<float>(20.1111, 15.6789, 0), Point<float>(15.6789, 20.1111, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dXZPlaneDiagonalIntersectionTest) {
    Triangle<float> t1(Point<float>(0, 0, 0), Point<float>(1000.0000, 0, 0), Point<float>(0, 0, 1000.0000));
    Triangle<float> t2(Point<float>(500.0000, 0, -100.0000), Point<float>(600.0000, 0, 1100.0000), Point<float>(-100.0000, 0, 600.0000));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dYZPlaneEdgeTouchIntersectionTest) {
    Triangle<float> t1(Point<float>(0, 0, 0), Point<float>(0, 5.0000, 0), Point<float>(0, 0, 5.0000));
    Triangle<float> t2(Point<float>(0, 0, 0), Point<float>(0, 5.0000, 0), Point<float>(0, 2.5000, -5.0000));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(CHATGPTTriangleTest, 2dArbitraryPlaneFractionalIntersectionTest) {
    Triangle<float> t1(Point<float>(0.0000, 0.0000, 0.0000), Point<float>(2.2222, 1.1111, 1.1111), Point<float>(0.0000, 2.3333, 2.3333));
    Triangle<float> t2(Point<float>(1.0000, 0.5555, 0.5555), Point<float>(3.3333, 1.4444, 1.4444), Point<float>(1.1111, 2.7777, 2.7777));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, CoplanarOverlappingPartial) {
    // Частичное перекрытие в одной плоскости (XY)
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(3.5, 0.0, 0.0), Point<float>(1.5, 2.8, 0.0));
    Triangle<float> t2(Point<float>(1.0, 0.5, 0.0), Point<float>(4.0, 0.5, 0.0), Point<float>(2.0, 3.0, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, CoplanarNoOverlap) {
    // Треугольники в одной плоскости без пересечения
    Triangle<float> t1(Point<float>(-1.2, -1.2, 0.0), Point<float>(1.2, -1.2, 0.0), Point<float>(0.0, 1.2, 0.0));
    Triangle<float> t2(Point<float>(2.5, 2.5, 0.0), Point<float>(4.5, 2.5, 0.0), Point<float>(3.5, 4.5, 0.0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, NonCoplanarIntersectingEdges) {
    // Пересечение ребер в 3D (не в одной плоскости)
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(4.0, 0.0, 0.0), Point<float>(0.0, 0.0, 4.0));
    Triangle<float> t2(Point<float>(2.0, -1.0, 2.0), Point<float>(2.0, 2.0, 2.0), Point<float>(5.0, 0.5, 2.0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Пересечение в точке (2,0,2)
}

TEST(GROKTriangleTest, NonCoplanarNoIntersection) {
    // Не пересекаются, плоскости пересекаются, но треугольники нет
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(2.0, 0.0, 0.0), Point<float>(0.0, 2.0, 1.0));
    Triangle<float> t2(Point<float>(3.0, 3.0, 2.0), Point<float>(5.0, 3.0, 2.0), Point<float>(3.0, 5.0, 3.0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, ParallelPlanesNoTouch) {
    // Параллельные плоскости без касания
    Triangle<float> t1(Point<float>(0.5, 0.5, 0.0), Point<float>(2.5, 0.5, 0.0), Point<float>(0.5, 2.5, 0.0));
    Triangle<float> t2(Point<float>(0.5, 0.5, 1.5), Point<float>(2.5, 0.5, 1.5), Point<float>(0.5, 2.5, 1.5));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, VertexInsideOtherTriangle) {
    // Вершина одного внутри другого
    Triangle<float> t1(Point<float>(-2.0, -2.0, 0.0), Point<float>(3.0, -2.0, 0.0), Point<float>(-2.0, 3.0, 0.0));
    Triangle<float> t2(Point<float>(0.0, 0.0, 0.0), Point<float>(4.0, 1.0, 0.0), Point<float>(1.0, 4.0, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2)); // (0,0,0) внутри t1
}

TEST(GROKTriangleTest, OneInsideAnotherCoplanar) {
    // Один полностью внутри другого в плоскости
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(5.5, 0.0, 0.0), Point<float>(0.0, 5.5, 0.0));
    Triangle<float> t2(Point<float>(1.2, 1.2, 0.0), Point<float>(2.8, 1.2, 0.0), Point<float>(1.2, 2.8, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, EdgeTouchingVertex) {
    // Ребро касается вершины
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(3.0, 0.0, 0.0), Point<float>(0.0, 3.0, 0.0));
    Triangle<float> t2(Point<float>(3.0, 0.0, 0.0), Point<float>(4.5, 1.5, 0.0), Point<float>(3.0, -1.5, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общая вершина (3,0,0)
}

TEST(GROKTriangleTest, SkewedEdgesNoIntersection) {
    // Наклонные ребра без пересечения
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(2.5, 2.5, 2.5), Point<float>(0.0, 3.0, 0.0));
    Triangle<float> t2(Point<float>(3.0, 3.0, 3.0), Point<float>(5.5, 5.5, 5.5), Point<float>(3.0, 6.0, 3.0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, DegenerateTriangleIntersecting) {
    // Вырожденный треугольник (линия) пересекает
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(4.0, 0.0, 0.0), Point<float>(4.0, 0.0, 0.0)); // Линия
    Triangle<float> t2(Point<float>(2.0, -1.0, 0.0), Point<float>(2.0, 1.0, 0.0), Point<float>(5.0, 0.0, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, FractionalCoordinatesIntersect) {
    // Пересечение с дробными координатами в XZ
    Triangle<float> t1(Point<float>(0.25, 0.0, 0.25), Point<float>(3.75, 0.0, 0.25), Point<float>(0.25, 0.0, 3.75));
    Triangle<float> t2(Point<float>(1.5, 0.0, 1.0), Point<float>(4.5, 0.0, 1.0), Point<float>(1.5, 0.0, 4.0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, ArbitraryPlaneNoIntersect) {
    // Нет пересечения в произвольной плоскости
    Triangle<float> t1(Point<float>(1.1, 1.1, 1.1), Point<float>(3.3, 2.2, 2.2), Point<float>(1.1, 3.3, 3.3));
    Triangle<float> t2(Point<float>(4.4, 4.4, 4.4), Point<float>(6.6, 5.5, 5.5), Point<float>(4.4, 6.6, 6.6));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, CloseButNoTouch) {
    // Близко, но без касания
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(2.0, 0.0, 0.0), Point<float>(1.0, 1.5, 0.0));
    Triangle<float> t2(Point<float>(2.01, 0.0, 0.0), Point<float>(4.0, 0.0, 0.0), Point<float>(3.0, 1.5, 0.0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, FullOverlapDifferentOrder) {
    // Полное совпадение с другим порядком вершин
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(2.5, 0.0, 0.0), Point<float>(0.0, 2.5, 0.0));
    Triangle<float> t2(Point<float>(2.5, 0.0, 0.0), Point<float>(0.0, 2.5, 0.0), Point<float>(0.0, 0.0, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, EdgePartialOverlapCoplanar) {
    // Частичное перекрытие ребер в плоскости
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(5.0, 0.0, 0.0), Point<float>(2.5, 3.0, 0.0));
    Triangle<float> t2(Point<float>(1.5, 0.0, 0.0), Point<float>(3.5, 0.0, 0.0), Point<float>(2.5, 2.0, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, VertexOnEdge) {
    // Вершина лежит на ребре другого
    Triangle<float> t1(Point<float>(0.0, 0.0, 0.0), Point<float>(4.0, 0.0, 0.0), Point<float>(0.0, 4.0, 0.0));
    Triangle<float> t2(Point<float>(2.0, 0.0, 0.0), Point<float>(3.0, 1.0, 0.0), Point<float>(1.0, 1.0, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2)); // (2,0,0) на ребре t1
}

TEST(GROKTriangleTest, LargeCoordinatesIntersect) {
    // Большие координаты, пересечение в YZ
    Triangle<float> t1(Point<float>(0.0, 100.0, 100.0), Point<float>(0.0, 500.0, 100.0), Point<float>(0.0, 100.0, 500.0));
    Triangle<float> t2(Point<float>(0.0, 200.0, 150.0), Point<float>(0.0, 600.0, 150.0), Point<float>(0.0, 300.0, 550.0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, NegativeCoordinatesNoIntersect) {
    // Отрицательные координаты, нет пересечения
    Triangle<float> t1(Point<float>(-5.0, -5.0, -5.0), Point<float>(-1.0, -5.0, -5.0), Point<float>(-5.0, -1.0, -5.0));
    Triangle<float> t2(Point<float>(-0.5, -0.5, 0.0), Point<float>(3.5, -0.5, 0.0), Point<float>(-0.5, 3.5, 0.0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(GROKTriangleTest, MixedCoordinatesTouchPoint) {
    // Касание в точке с смешанными координатами
    Triangle<float> t1(Point<float>(-1.0, 0.0, 1.0), Point<float>(2.0, 0.0, 1.0), Point<float>(0.5, 2.0, 1.0));
    Triangle<float> t2(Point<float>(0.5, 0.0, 1.0), Point<float>(3.0, 1.0, 2.0), Point<float>(0.5, -1.0, 0.0));
    EXPECT_TRUE(t1.is_intersect(t2)); // Общая точка (0.5, 0.0, 1.0)
}

TEST(MYTriangleest, 2dTest1_noIntersection) {
    Triangle<float> t1(Point<float>(0, 0, 0), Point<float>(3, 0, 0), Point<float>(0, 3, 0));
    Triangle<float> t2(Point<float>(2, 3, 0), Point<float>(5, 3, 0), Point<float>(2, 6, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 2dTest2_noIntersection) {
    Triangle<float> t1(Point<float>(0, 0, 0), Point<float>(3, 0, 0), Point<float>(0, 3, 0));
    Triangle<float> t2(Point<float>(3, 1, 0), Point<float>(3, 2, 0), Point<float>(6, 4, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 2dTest3_noIntersection) {
    Triangle<float> t1(Point<float>(0, 0, 0), Point<float>(3, 0, 0), Point<float>(0, 3, 0));
    Triangle<float> t2(Point<float>(-0.5, -0.5, 0), Point<float>(-0.5, 100, 0), Point<float>(-100, -100, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 2dTest4_noIntersection) {
    Triangle<float> t1(Point<float>(0, 0, 0), Point<float>(3, 0, 0), Point<float>(0, 3, 0));
    Triangle<float> t2(Point<float>(3, -2, 0), Point<float>(5, -4, 0), Point<float>(4, 3, 0));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest5_noIntersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(0, 0, 0.2), Point<double>(3, 0, 0.2), Point<double>(0, 3, 0.2));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest6_noIntersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(10.912, 10.912, 10000000.0001), Point<double>(3, 0, 0.0123), Point<double>(0, 3, 0.0123));
    EXPECT_FALSE(t1.is_intersect(t2));
}

// пересечение по точке
TEST(MYTriangleest, 3dTest7_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(3, -2, -1234), Point<double>(5, -4, 100000), Point<double>(3, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

// кусок одого заходит на другой
TEST(MYTriangleest, 2dTest8_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(3,5,0), Point<double>(4, 4, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

// один внутри другого (стороны параллельны)
TEST(MYTriangleest, 2dTest9_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(0.5, 2, 0), Point<double>(2, 0.5,0), Point<double>(0.5, 0.5, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

// один внутри другого (стороны не параллельны)
TEST(MYTriangleest, 2dTest10_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(0.5, 2, 0), Point<double>(2, 0.1, 0), Point<double>(0.5, 0.5, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest11_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(1, 1, 0), Point<double>(2, 2, 2), Point<double>(2, -2, 2));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest12_noIntersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(3, 0, 0), Point<double>(0, 3, 0));
    Triangle<double> t2(Point<double>(1, 1, 0.1), Point<double>(2, 2, 2), Point<double>(2, -2, 2));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest13_intersection) {
    Triangle<double> t1(Point<double>(10, 0, 0), Point<double>(0, 10, 0), Point<double>(0, 0, 0));
    Triangle<double> t2(Point<double>(5, 1, 1), Point<double>(5, -1, -1), Point<double>(5, -3, 10));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest14_noIntersection) {
    Triangle<double> t1(Point<double>(10, 0, 0), Point<double>(0, 10, 0), Point<double>(0, 0, 0));
    Triangle<double> t2(Point<double>(5, 1, 1.5), Point<double>(5, -1, -1), Point<double>(5, -3, 10));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest15_intersection) {
    Triangle<double> t1(Point<double>(2.0, 1.0, 5.0), Point<double>(8.0, 3.0, 2.0), Point<double>(4.0, 9.0, 6.0));
    Triangle<double> t2(Point<double>(3.3, 4.3, 4.7), Point<double>(6.7, 5.0, 3.8), Point<double>(4.7, 7.0, 5.5));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest16_noIntersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 3, 3), Point<double>(0, 0, 3));
    Triangle<double> t2(Point<double>(8, 8, 8), Point<double>(8, 8, 8), Point<double>(-10, 8, 8));
    EXPECT_FALSE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest17_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 3, 3), Point<double>(0, 0, 3));
    Triangle<double> t2(Point<double>(3, 3, 3), Point<double>(3, 3, 3), Point<double>(-4, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest18_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 3, 3), Point<double>(0, 0, 3));
    Triangle<double> t2(Point<double>(3, 3, 3), Point<double>(3, 3, 3), Point<double>(-4, 0, 2));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest19_intersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 3, 3), Point<double>(0, 0, 3));
    Triangle<double> t2(Point<double>(0, 0, 0), Point<double>(0, 0, 0), Point<double>(0, 0, 0));
    EXPECT_TRUE(t1.is_intersect(t2));
}

TEST(MYTriangleest, 3dTest20_noIntersection) {
    Triangle<double> t1(Point<double>(0, 0, 0), Point<double>(0, 3, 3), Point<double>(0, 0, 3));
    Triangle<double> t2(Point<double>(1000, 1000, 1000), Point<double>(1000, 1000, 1000), Point<double>(1000, 1000, 1000));
    EXPECT_FALSE(t1.is_intersect(t2));
}


TEST(intersect_3d, Triangle) {
    // arrange
    Triangle<double> tr1(Point<double>(0, 0, 2), Point<double>(0, 1, 0), Point<double>(1, 0, 0));
    Triangle<double> tr2(Point<double>(0, 0, 1), Point<double>(0, 2, 0), Point<double>(2, 0, 0));
    Triangle<double> tr3(Point<double>(0, 0, 3), Point<double>(0, 2, 0), Point<double>(2, 0, 0));

    // act, assert
    EXPECT_TRUE(tr1.is_intersect(tr2));
    EXPECT_FALSE(tr1.is_intersect(tr3));
}

TEST(intersect_3d, PiercesInteriorAtPoint) {
    // arrange
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0.5,0.5,-1), Point<double>(0.5,0.5,1), Point<double>(2,2,2));

    // act, assert
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, TouchesInsideAtVertex) {
    // arrange
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0.3,0.3,0), Point<double>(2,0,1), Point<double>(0,2,1));

    // act, assert
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, EdgeInPlaneOverlapsTriangle) {
    // arrange
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0.2,0.2,0), Point<double>(1.2,0.2,0), Point<double>(0.5,0.5,1));

    // act, assert
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, PointInPlaneOverlapsTriangleIntersection) {
    // arrange
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(1,0,0), Point<double>(0,1,0));
    Triangle<double> t2(Point<double>(0,0.5,-0.5), Point<double>(0,0.5,0.5), Point<double>(-1,0,0));

    // act, assert
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, CrossesPlaneOutside_NoIntersection) {
    // arrange
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(3,3,-1), Point<double>(3,3,1), Point<double>(4,4,0));

    // act, assert
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, ParallelPlanes_NoIntersection) {
    // arrange
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0,0,1), Point<double>(2,0,1), Point<double>(0,2,1));

    // act, assert
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, TouchesAtT1Vertex) {
    // arrange
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0,0,-1), Point<double>(0,0,1), Point<double>(1,1,1));

    // act, assert
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_PiercesTriangle) {
    // t1 in the plane z=0; tSeg are collinear Point<double>s: a segment through the interior of t1
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> tSeg(Point<double>(0.5,0.5,-1), Point<double>(0.5,0.5,0), Point<double>(0.5,0.5,1));
    EXPECT_TRUE(t1.is_intersect(tSeg));
    EXPECT_TRUE(tSeg.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_Coplanar_OverlapsEdge) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> tSeg(Point<double>(0,0,0), Point<double>(1,0,0), Point<double>(2,0,0)); // segment on edge t1
    EXPECT_TRUE(t1.is_intersect(tSeg));
    EXPECT_TRUE(tSeg.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_CollinearSegment_ParallelOffset_NoIntersection) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> tSeg(Point<double>(3,3,1), Point<double>(4,4,1), Point<double>(5,5,1));
    EXPECT_FALSE(t1.is_intersect(tSeg));
    EXPECT_FALSE(tSeg.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Segment_Triangle_Intersection) {
    Triangle<double> tSeg(Point<double>(0,0,0), Point<double>(0,0,0), Point<double>(4,4,4));
    Triangle<double> t1(Point<double>(0,0,1), Point<double>(0,1,0), Point<double>(1,0,0));
    EXPECT_TRUE(t1.is_intersect(tSeg));
    EXPECT_TRUE(tSeg.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_InsideTriangle) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> tPt(Point<double>(0.3,0.3,0), Point<double>(0.3,0.3,0), Point<double>(0.3,0.3,0));
    EXPECT_TRUE(t1.is_intersect(tPt));
    EXPECT_TRUE(tPt.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_AtVertex) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> tPt(Point<double>(0,0,0), Point<double>(0,0,0), Point<double>(0,0,0));
    EXPECT_TRUE(t1.is_intersect(tPt));
    EXPECT_TRUE(tPt.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_OffPlane_NoIntersection) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> tPt(Point<double>(0.3,0.3,0.1), Point<double>(0.3,0.3,0.1), Point<double>(0.3,0.3,0.1));
    EXPECT_FALSE(t1.is_intersect(tPt));
    EXPECT_FALSE(tPt.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Segment_Segment_NoIntersection) {
    Triangle<double> t1(Point<double>(2,0,0), Point<double>(1,0,0), Point<double>(0,0,0));
    Triangle<double> t2(Point<double>(0,0,1), Point<double>(0,0,2), Point<double>(0,0,3));
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Segment_Segment_Intersection) {
    Triangle<double> t1(Point<double>(2,0,0), Point<double>(1,0,0), Point<double>(0,0,0));
    Triangle<double> t2(Point<double>(1,0,-1), Point<double>(1,0,0), Point<double>(1,0,1));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_NoIntersection) {
    Triangle<double> t1(Point<double>(2,0,0), Point<double>(1,0,0), Point<double>(0,0,0));
    Triangle<double> t2(Point<double>(0,0,1), Point<double>(0,0,1), Point<double>(0,0,1));
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Point_Segment_Intersection) {
    Triangle<double> t1(Point<double>(0,0,2), Point<double>(0,0,3), Point<double>(0,0,-1));
    Triangle<double> t2(Point<double>(0,0,1), Point<double>(0,0,1), Point<double>(0,0,1));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Two_Points_NoIntersection) {
    Triangle<double> t1(Point<double>(2,0,0), Point<double>(2,0,0), Point<double>(2,0,0));
    Triangle<double> t2(Point<double>(0,0,1), Point<double>(0,0,1), Point<double>(0,0,1));
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, Degenerate_Two_Points_Intersection) {
    Triangle<double> t1(Point<double>(2,0,0), Point<double>(2,0,0), Point<double>(2,0,0));
    Triangle<double> t2(Point<double>(2,0,0), Point<double>(2,0,0), Point<double>(2,0,0));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Coplanar_Disjoint_NoIntersection) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(3,0,0), Point<double>(5,0,0), Point<double>(3,2,0));
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, Coplanar_ShareFullEdge_Touching) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(2,0,0), Point<double>(0,2,0), Point<double>(2,2,0)); // common edge
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Coplanar_Containment_T2InsideT1) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(3,0,0), Point<double>(0,3,0));
    Triangle<double> t2(Point<double>(0.5,0.5,0), Point<double>(1.5,0.5,0), Point<double>(0.5,1.5,0));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Coplanar_TouchAtSingleVertexOnly) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(2,0,0), Point<double>(3,0,0), Point<double>(2,1,0)); // touching at the top (2,0,0)
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Coplanar_IdenticalTriangles) {
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, SkewPlanes_IntersectInSegment) {
    // the planes are not parallel; the intersection line gives a segment inside both
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0.2,0.2,-1), Point<double>(1.5,0.2,1), Point<double>(0.2,1.5,1));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, SkewPlanes_LineCutsOutside_NoIntersection) {
    // t2 intersects plane t1, but the section is outside t1
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(3,3,-1), Point<double>(3,3,1), Point<double>(4,2,0.5));
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_Diagonal) {
    // Two non-parallel planes, the intersection is a segment inside both Triangle<double>s
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(3,0,0), Point<double>(0,3,0));
    Triangle<double> t2(Point<double>(0.3,0.3,-1), Point<double>(2.4,0.3,1), Point<double>(0.3,2.4,1));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, SkewPlanes_InteriorSegment_TouchesT1Edges) {
    // The intersection segment lies from (1,0,0) to (0,1,0) - touches the edges t1
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0,0,-1), Point<double>(2,0,1), Point<double>(0,2,1));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, EdgeFace_Pierce_InteriorPoint) {
    // Edge t2 pierces the interior of t1 at one Point
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0.8,0.6,-1), Point<double>(0.8,0.6,1), Point<double>(3,-1,0.5));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, VertexOnT1Edge_PointContact) {
    // Vertex t2 lies on edge t1, the remaining Point<double>s are outside the plane t1
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(1,0,0), Point<double>(1,0,1), Point<double>(2,1,1));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, NonCoplanar_ShareFullEdge) {
    // Common full edge for non-parallel planes (intersection line = edge)
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(1,0,1)); // плоскость y=0
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, SkewPlanes_LineHitsOutside_NoIntersection_2) {
    // The intersection line of the planes passes outside t1
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(0,2,0));
    Triangle<double> t2(Point<double>(3,3,-1), Point<double>(5,3,1), Point<double>(3,5,1));
    EXPECT_FALSE(t1.is_intersect(t2));
    EXPECT_FALSE(t2.is_intersect(t1));
}

TEST(intersect_3d, Coplanar_PartialAreaOverlap) {
    // Coplanar Triangle<double>s with non-empty intersection area
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(3,0,0), Point<double>(0,3,0));
    Triangle<double> t2(Point<double>(0.5,0,0), Point<double>(2.5,0,0), Point<double>(0.5,2,0));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

TEST(intersect_3d, Coplanar_ProperEdgeCross) {
    // Coplanar: edges intersect, no vertex lies inside another
    Triangle<double> t1(Point<double>(0,0,0), Point<double>(2,0,0), Point<double>(1,2,0));
    Triangle<double> t2(Point<double>(-0.5,0.5,0), Point<double>(2.5,0.5,0), Point<double>(1.5,-1,0));
    EXPECT_TRUE(t1.is_intersect(t2));
    EXPECT_TRUE(t2.is_intersect(t1));
}

static Point<double> P(float x, float y, float z = 0.0) { return Point<double>{x, y, z}; }

TEST(intersect_2d, Disjoint_NoOverlap) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<double> B{ P(3,3), P(4,3), P(3,4) };

    // act, assert
    EXPECT_FALSE(A.is_intersect(B));
    EXPECT_FALSE(B.is_intersect(A));
}

TEST(intersect_2d, Containment_B_inside_A) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<double> B{ P(0.2,0.2), P(0.6,0.2), P(0.2,0.6) };

    // act, assert
    EXPECT_TRUE(A.is_intersect(B));
    EXPECT_TRUE(B.is_intersect(A));
}

TEST(intersect_2d, Containment_OrientationDoesNotMatter) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<double> B{ P(0.2,0.6), P(0.6,0.2), P(0.2,0.2) };

    // act, assert
    EXPECT_TRUE(A.is_intersect(B));
    EXPECT_TRUE(B.is_intersect(A));
}

TEST(intersect_2d, EdgeEdge_ProperCross_NoVertexInside) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(1,2) };
    Triangle<double> B{ P(-0.5,0.5), P(2.5,0.5), P(1.5,-1) };

    // act, assert
    EXPECT_TRUE(A.is_intersect(B));
    EXPECT_TRUE(B.is_intersect(A));
}

TEST(intersect_2d, Touch_VertexOnEdge_PointContact) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<double> B{ P(1,0), P(1.1,-0.2), P(0.9,-0.2) };

    // act, assert
    EXPECT_TRUE(A.is_intersect(B));
    EXPECT_TRUE(B.is_intersect(A));
}

TEST(intersect_2d, Collinear_OverlappingEdges) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<double> B{ P(0,0), P(2,0), P(3,1) };

    // act, assert
    EXPECT_TRUE(A.is_intersect(B));
    EXPECT_TRUE(B.is_intersect(A));
}

TEST(intersect_2d, Touch_AtSingleSharedVertexOnly) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<double> B{ P(2,0), P(3,0), P(2,1) };

    // act, assert
    EXPECT_TRUE(A.is_intersect(B));
    EXPECT_TRUE(B.is_intersect(A));
}

TEST(intersect_2d, Disjoint_CloseButSeparated) {
    // arrange
    Triangle<double> A{ P(0,0), P(2,0), P(0,2) };
    Triangle<double> B{ P(2.1,0.0), P(3.0,0.0), P(2.1,0.9) };

    // act, assert
    EXPECT_FALSE(A.is_intersect(B));
    EXPECT_FALSE(B.is_intersect(A));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}