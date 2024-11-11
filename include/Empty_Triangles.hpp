#ifndef EMPTY_TRIANGLES_EMPTY_TRIANGLES_HPP
#define EMPTY_TRIANGLES_EMPTY_TRIANGLES_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include "BasicDataStructures.hpp"
#include "basic_geometry.hpp"
#include "data_handling.hpp"

std::vector<Triangle> get_empty_triangles(std::vector<int> indices, std::vector<Point_2> points);

#endif