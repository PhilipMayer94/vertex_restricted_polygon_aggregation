#ifndef EMPTY_TRIANGLES_BASIC_GEOMETRY_HPP
#define EMPTY_TRIANGLES_BASIC_GEOMETRY_HPP
#include "BasicDataStructures.hpp"

// calculate orientation of vectors ab, bc and ca in relation to each other
// returns one of the three following values:
// 0 if a, b, c collinear
// 1 if vectors are oriented clockwise
// -1 if they are oriented counter-clockwise
int pointOrientation(const Point_2 &a, const Point_2 &b, const Point_2 &c);


bool is_point_inside_triangle(const int point, const Triangle &triangle, const std::vector<Point_2> &points);

#endif