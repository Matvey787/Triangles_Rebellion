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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}