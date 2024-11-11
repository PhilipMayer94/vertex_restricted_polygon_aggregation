#include <gtest/gtest.h>
#include "Geometry.hpp"
#include "BasicDataStructures.hpp"

TEST(intersection_test, overlapping_edges){
    Point_2 a = Point_2(0,0);
    Point_2 b = Point_2(10,0);
    Point_2 c = Point_2(8,0);

    ASSERT_TRUE(do_segments_intersect(a, b, b, c));
}

TEST(intersection_test, weird_instance){
    //(30, 320)(82, 330)-----(82, 290)(68, 330)

    Point_2 a = Point_2(30, 320);
    Point_2 b = Point_2(82, 330);
    Point_2 c = Point_2(82, 290);
    Point_2 d = Point_2(68, 330);

    ASSERT_TRUE(do_segments_intersect(a, b, c, d));
}

TEST(intersection_test, simple_intersection){
    Point_2 a = Point_2(0,0);
    Point_2 b = Point_2(7,-2);
    Point_2 c = Point_2(2,2);
    Point_2 d = Point_2(3,-1);

    ASSERT_TRUE(do_segments_intersect(a, b, c, d));
}