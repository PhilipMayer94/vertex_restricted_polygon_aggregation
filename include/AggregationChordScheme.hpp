//
// Created by philip on 8/19/24.
//

#ifndef POLYGONAGGREGATION_AGGREGATIONCHORDSCHEME_HPP
#define POLYGONAGGREGATION_AGGREGATIONCHORDSCHEME_HPP


#include <utility>

#include "TriangulationObjects.hpp"
#include "AggregationIlpSolver.hpp"

struct AggregationSolution{
    std::vector<int> e_in;
    std::vector<int> t_in;
    std::vector<int> t_out;

    double area;
    double perimeter;
    double lambda;

    double eval_alpha(double alpha) const{
        return area + alpha*perimeter;
    }

    AggregationSolution( const std::pair<double,std::pair<double,double>> &values, const std::pair<std::vector<int>, std::pair<std::vector<int>, std::vector<int>>> & objects ){
        lambda=values.first;
        perimeter=values.second.first;
        area=values.second.second;

        e_in=objects.first;
        t_in=objects.second.first;
        t_out=objects.second.second;


    }

    // Define the comparison operator
    bool operator<(const AggregationSolution& other) const {
        if (lambda < other.lambda) return true;
        if (lambda > other.lambda) return false;
        return area < other.area; // Use area as tiebreaker
    }



};

class AggregationChordScheme {
public:
    AggregationChordScheme(std::vector<Point_2> vertices, std::vector<std::vector<int>> polygons, bool use_left_right_constraints);

    void run_with_epsilon(double epsilon,bool contraction=false){
        try {
            eps = epsilon;
            solver.solve(0);
            AggregationSolution solution_left(solver.get_result_values(),
                                              solver.get_result_triangulation_index_based());

            solver.solve(10000);
            AggregationSolution solution_right(solver.get_result_values(),
                                               solver.get_result_triangulation_index_based());

            solutions.emplace_back(solution_left);
            solutions.emplace_back(solution_right);

            if (!contraction) {
                chord_recursion(solution_left, solution_right);
            } else {
                chord_recursion_contraction(solution_left, solution_right);
            }

            std::sort(solutions.begin(), solutions.end());
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error during final solve: " + std::string(e.what()));
        }

    }
    void chord_recursion( AggregationSolution & solution_left, AggregationSolution & solution_right);

    void chord_recursion_contraction( AggregationSolution & solution_left, AggregationSolution & solution_right){
        if(solution_right.area==solution_left.area && solution_left.perimeter==solution_right.area){
            return;
        }
        double alpha_middle= compute_crossing_alpha(solution_left,solution_right);
        if (alpha_middle<=0){
            return;
        }

        solver.add_constraints_to_model(solution_left.t_in,solution_right.t_out);


        solver.solve(alpha_middle);
        AggregationSolution solution_middle(solver.get_result_values(),solver.get_result_triangulation_index_based());

        if((solution_left.eval_alpha(alpha_middle)<(1+eps)*solution_middle.eval_alpha(alpha_middle))||
           (solution_right.eval_alpha(alpha_middle)<(1+eps)*solution_middle.eval_alpha(alpha_middle))){
            return;
        }
        solutions.emplace_back(solution_middle);
        chord_recursion_contraction(solution_left,  solution_middle);
        chord_recursion_contraction(solution_middle, solution_right);

    }

    bool test_solution_nestedness();


    void write_solution_i(int i, const std::string& output_folder);

    void write_all_solutions(const std::string& output_folder){

        for(const auto& c:solutions){
            std::cout<<c.lambda<<" | "<<c.area<<" "<<c.perimeter<<std::endl;
        }

        std::filesystem::create_directories(output_folder);
        std::string new_folder=output_folder+"/sol";
        for(int i=0;i<solutions.size();i++){
            write_solution_i(i,new_folder);
        }
        objects.writeVerticesAndPolygons(output_folder);
    }
    TriangulationObjects objects;

private:

    static double compute_crossing_alpha(const AggregationSolution & sol_1, const AggregationSolution & sol_2);

    std::vector<AggregationSolution> solutions;
    double eps=1e-18;

    AggregationIlpSolver solver;



};


#endif //POLYGONAGGREGATION_AGGREGATIONCHORDSCHEME_HPP
