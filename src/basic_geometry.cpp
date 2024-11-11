#include "basic_geometry.hpp"
#include <iostream>

// calculate orientation of vectors ab, bc and ca in relation to each other
// returns one of the three following values:
// 0 if a, b, c collinear
// 1 if vectors are oriented clockwise
// -1 if they are oriented counter-clockwise
int pointOrientation(const Point_2 &a, const Point_2 &b, const Point_2 &c)
{
    double orientation = ((long double)(b.y - a.y)) * ((long double)(c.x - b.x)) -
                         ((long double)(b.x - a.x)) * ((long double)(c.y - b.y));
    if (orientation == 0)
    {
        return 0;
    }
    return (orientation > 0) ? 1 : -1;
}

// for testing (for now):
bool is_point_inside_triangle(const int point, const Triangle &triangle, const std::vector<Point_2> &points)
{
    Point_2 in_point = points[point];
    // if(triangle == Triangle({0, 3, 13}) && in_point == Point_2(26, 63)){
    //     std::cout<<"break";
    // }
    Point_2 a = points[triangle.vertices[0]];
    Point_2 b = points[triangle.vertices[1]];
    Point_2 c = points[triangle.vertices[2]];
    int orientation_edge_a = pointOrientation(in_point, b, c);
    int orientation_edge_b = pointOrientation(in_point, c, a);
    int orientation_edge_c = pointOrientation(in_point, a, b);

    //checks if point is on an edge
    if(orientation_edge_a == 0 && orientation_edge_b == orientation_edge_c){
        return true;
    }
    else if(orientation_edge_b == 0 && orientation_edge_a == orientation_edge_c){
        return true;
    }
    else if(orientation_edge_c == 0 && orientation_edge_a == orientation_edge_b){
        return true;
    }
    
    if (orientation_edge_a != orientation_edge_b || orientation_edge_b != orientation_edge_c)
    {
        return false;
    }

    return true;
}