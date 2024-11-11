#include <gtest/gtest.h>
#include "TriangulationObjects.hpp"
#include "data_handling.hpp"
#include "config.hpp"

TEST(first_tests, basic_example) {
    std::vector<Point_2> points = {Point_2(0, 0), Point_2(3, -1), Point_2(2, 2), Point_2(5, -4), Point_2(9, -4),
                                   Point_2(9, -2), Point_2(7, -2), Point_2(10, 0), Point_2(10, 2), Point_2(8, 2),
                                   Point_2(8, 0)};
    std::vector<std::vector<int>> polygons = {{0, 1, 2},
                                              {3, 4, 5, 6},
                                              {7, 8, 9, 10}};
    std::vector<int> indices;
    for (int i = 0; i < points.size(); i++) {
        indices.emplace_back(i);
    }
    TriangulationObjects triangulations = TriangulationObjects(points, polygons);
    triangulations.calculate_triangulations();
    write_edges_to_file(DATA_ROOT + "results/testing_outputs/edges.txt", triangulations.get_edges(),
                        triangulations.get_types_for_valid_edges());
}