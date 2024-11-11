#ifndef EMPTY_TRIANGLES_DATA_HANDLING_HPP
#define EMPTY_TRIANGLES_DATA_HANDLING_HPP

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "BasicDataStructures.hpp"

void write_triangles_to_file(std::string output, std::vector<Triangle> triangles, std::vector<Point_2> points);

void write_points_to_file(std::string output, std::vector<Point_2> points);

std::vector<Point_2> read_points_from_file(std::string input_path);

//std::vector<Point_2> read_points_from_polygon_file2(const std::string input_path);

std::vector<Point_2> pre_process(std::vector<Point_2> &points);

std::vector<Point_2> read_points_from_polygon_file(const std::string input_path);

void write_edges_to_file(const std::string output_path, const std::vector<Edge> &edges, const std::vector<int> &classifications);

void write_polygons_to_file(std::string output_path, const std::vector<Polygon> &polygons, const std::vector<Point_2> &points);




std::vector<std::vector<Point_2>> load_polygons(const std::string input_path);

std::pair<std::vector<Point_2>,std::vector<std::vector<int>>>  load_aggregation_instance(const std::string input_path);


#endif