#ifndef EMPTY_TRIANGLES_TRIANGULATIONS_HPP
#define EMPTY_TRIANGLES_TRIANGULATIONS_HPP

#include "BasicDataStructures.hpp"
#include "Geometry.hpp"
#include "Edge_Data_Storage.hpp"
#include <utility>
#include <fstream>
#include <filesystem>

class TriangulationObjects
{
public:

    explicit TriangulationObjects(){};
    explicit TriangulationObjects(std::vector<Point_2> points, std::vector<std::vector<int>> polygons);
    
    void calculate_triangulations();

    void set_polygons(const std::vector<Point_2>& points, const std::vector<std::vector<int>>& polygons);

    const std::vector<Point_2> & get_vertices();

    const std::vector<Triangle> &get_valid_triangles();

    const std::vector<Edge> &get_edges();

    const  std::vector<int> &get_types_for_valid_edges();

    const std::vector<std::pair<std::vector<int>,std::vector<int>>>& get_left_and_right_triangles_for_valid_edges();

    void writeVerticesAndPolygons( const std::string& folderName);


private: 
    void add_triangle_to_respective_list(Point_2 p1, Point_2 p2, Point_2 p3, int index, int triangle_index);

    bool has_run=false;

    std::vector<Polygon> polygons;

    std::vector<Point_2> points;

    //std::vector<int> indices;

    std::vector<std::pair<std::vector<int>, std::vector<int>>> left_right_triangles;

    std::vector<Triangle> triangles;

    std::vector<Edge> edges;


    //0=irrelevant 1=polygon 2=convex 3=free
    std::vector<int> edge_classifications;
};



#endif
