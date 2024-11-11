

//
// Created by philip on 8/14/24.
//

#include <gtest/gtest.h>
#include "TriangulationObjects.hpp"
#include "data_handling.hpp"
#include "config.hpp"
#include "AggregationIlpSolver.hpp"
#include "AggregationChordScheme.hpp"

//
//TEST(chord_test, basic_example) {
//    double lambda=-1;
//
//
//    auto instance = load_aggregation_instance(DATA_ROOT + "/first_testing_data/selection_2.csv");
//    AggregationChordScheme aggregator = AggregationChordScheme(instance.first, instance.second);
//    aggregator.run_with_epsilon(1e-1);
//    //aggregator.write_all_solutions("data/results/sel2_contraction");
//}
//
//TEST(chord_test, harder_example_contraction) {
//    auto instance = load_aggregation_instance(DATA_ROOT + "/first_testing_data/selection_1.csv");
//    AggregationChordScheme aggregator = AggregationChordScheme(instance.first, instance.second);
//    aggregator.run_with_epsilon(1e-3,true);
//    aggregator.write_all_solutions("data/results/sel1_contraction");
//}
//
//TEST(chord_test, harder_example) {
//    auto instance = load_aggregation_instance(DATA_ROOT + "/first_testing_data/selection_1.csv");
//    AggregationChordScheme aggregator = AggregationChordScheme(instance.first, instance.second);
//    aggregator.run_with_epsilon(1e-3);
//    aggregator.write_all_solutions("data/results/sel1");
//}

TEST(chord_test, village_contraction) {
    auto instance = load_aggregation_instance(DATA_ROOT + "/first_testing_data/village_4.csv");
    AggregationChordScheme aggregator = AggregationChordScheme(instance.first, instance.second,true);
    aggregator.run_with_epsilon(1e-12,true);
    aggregator.write_all_solutions("data/results/village_contraction");
}

TEST(chord_test, village_no_contraction) {
    auto instance = load_aggregation_instance(DATA_ROOT + "/first_testing_data/village_4.csv");
    AggregationChordScheme aggregator = AggregationChordScheme(instance.first, instance.second,true);
    aggregator.run_with_epsilon(1e-12);
    aggregator.write_all_solutions("data/results/village_no_contraction");
}