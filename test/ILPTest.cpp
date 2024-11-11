//
// Created by philip on 8/14/24.
//

#include <gtest/gtest.h>
#include "TriangulationObjects.hpp"
#include "data_handling.hpp"
#include "config.hpp"
#include "AggregationIlpSolver.hpp"

//TEST(first_tests_ilp, basic_example)
//{
//    std::vector<Point_2> points = {Point_2(-16,8), Point_2(-14,6), Point_2(-14,10), Point_2(5, -4), Point_2(9, -4), Point_2(9, -2), Point_2(7, -2), Point_2(10, -1), Point_2(10, 1), Point_2(8, 1), Point_2(8, -1)};
//    std::vector<std::vector<int>> polygons = {{0, 1, 2}, {3, 4, 5, 6}, {7, 8, 9, 10}};
//    std::vector<int> indices;
//    for(int i=0;i<points.size(); i++){
//        indices.emplace_back(i);
//    }
//    TriangulationObjects triangulations = TriangulationObjects(points, polygons);
//    triangulations.calculate_triangulations();
//    write_edges_to_file(DATA_ROOT+ "results/edges.txt", triangulations.get_edges(), triangulations.get_types_for_valid_edges());
//    auto x=AggregationIlpSolver(triangulations);
//    x.create_model();
//    x.solve();
//    x.print_result();
//}

TEST(Second_ilp_test, basic_example) {
    double lambda=2100;


    auto instance = load_aggregation_instance(DATA_ROOT + "/relevant_ilp_instances/bockelskamp_simplfied.csv");
    TriangulationObjects triangulations = TriangulationObjects(instance.first, instance.second);
    triangulations.calculate_triangulations();

    triangulations.writeVerticesAndPolygons(("data/output_"+std::to_string(static_cast<int>(lambda))));
    write_edges_to_file(DATA_ROOT + "results/edges.txt", triangulations.get_edges(),
                        triangulations.get_types_for_valid_edges());
    auto x = AggregationIlpSolver(triangulations);
    x.create_model(true);
    x.solve(lambda);
    x.print_result();
    x.get_result_values();
    x.write_result_csv(("data/output_"+std::to_string(static_cast<int>(lambda))));
}