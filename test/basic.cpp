#include <gtest/gtest.h>
#include "Empty_Triangles.hpp"
#include <iostream>

TEST(basic, basic_test)
{
    ASSERT_TRUE(true);
}

TEST(sort, basic)
{
    auto triangles = get_empty_triangles({0, 1, 2, 3}, {Point_2(5, -1), Point_2(1, 1), Point_2(4, 2), Point_2(0, 0)});
    for (Triangle triangle : triangles)
    {
        std::cout << triangle.vertices[0] << " " << triangle.vertices[1] << " " << triangle.vertices[2] << "\n";
    }
}

TEST(collinear_problems, degenerated_triangles){
    auto triangles = get_empty_triangles({0, 1, 2}, {Point_2(5, 0), Point_2(1, 0), Point_2(4, 0)});
    for (Triangle triangle : triangles)
    {
        std::cout << triangle.vertices[0] << " " << triangle.vertices[1] << " " << triangle.vertices[2] << "\n";
    }
    ASSERT_TRUE(triangles.size()==0);
}

TEST(collinear_problems, degenerated_triangles_collinear){
    auto triangles = get_empty_triangles({0, 1, 2, 3, 4}, {Point_2(0, 0), Point_2(1, 1), Point_2(2, 2), Point_2(2,0), Point_2(2,3)});
    for (Triangle triangle : triangles)
    {
        std::cout << triangle.vertices[0] << " " << triangle.vertices[1] << " " << triangle.vertices[2] << "\n";
    }
    ASSERT_TRUE(triangles.size()==4);
}

TEST(collinear_problems, collinear_visibility){
    auto triangles = get_empty_triangles({0, 1, 2, 3}, {Point_2(0, 0), Point_2(3, 1), Point_2(3, 0), Point_2(3, -1)});
    for (Triangle triangle : triangles)
    {
        std::cout << triangle.vertices[0] << " " << triangle.vertices[1] << " " << triangle.vertices[2] << "\n";
    }
    ASSERT_TRUE(triangles.size()==2);
}
