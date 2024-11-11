//
// Created by philip on 8/19/24.
//

#include "AggregationChordScheme.hpp"

void AggregationChordScheme::write_solution_i(int i, const std::string &output_path) {
    if (i<0 ||i> solutions.size()-1){
        std::cout<<" writing_solution_out_of_range";
        return;
    }
    auto sol=solutions.at(i);
    std::vector<Edge> e_in_res;
    std::vector<Triangle> t_in_res;
    std::vector<Triangle> t_out_res;

    e_in_res.reserve(sol.e_in.size());
    for( auto e:sol.e_in){
        e_in_res.emplace_back(objects.get_edges()[e]);
    }
    t_in_res.reserve(sol.t_in.size());
    for( auto e:sol.t_in){
        t_in_res.emplace_back(objects.get_valid_triangles()[e]);
    }
    t_out_res.reserve(sol.t_out.size());
    for( auto e:sol.t_out){
        t_out_res.emplace_back(objects.get_valid_triangles()[e]);
    }
    std::string output_folder;

    if(i<10){
        output_folder=output_path+"_0000"+std::to_string(i);
    }
    if(i>=10 &&i<100){
        output_folder=output_path+"_000"+std::to_string(i);
    }
    if(i>=100 &&i<1000){
        output_folder=output_path+"_00"+std::to_string(i);
    }
    if(i>=1000 &&i<10000){
        output_folder=output_path+"_0"+std::to_string(i);
    }
    if(i>=10000){
        output_folder=output_path+"_"+std::to_string(i);
    }


    auto data=std::make_tuple(e_in_res,t_in_res,t_out_res);
    // Create the folder if it doesn't exist
    std::filesystem::create_directories(output_folder);

    // Define the file paths
    std::string edgeFileName = output_folder + "/edges";
    std::string triangleFileName = output_folder + "/triangles";

    // Unpack the tuple
    const auto& edges = std::get<0>(data);
    const auto& triangles1 = std::get<1>(data);
    const auto& triangles2 = std::get<2>(data);

    // Write the edges to a space-separated file
    std::ofstream edgeFile(edgeFileName);
    if (edgeFile.is_open()) {
        for (const auto& edge : edges) {
            edgeFile << edge.src << " " << edge.dst << "\n";
        }
        edgeFile.close();
    } else {
        std::cerr << "Unable to open edge file\n";
    }

    // Write the triangles to a space-separated file
    std::ofstream triangleFile(triangleFileName);
    if (triangleFile.is_open()) {
        for (const auto& triangle : triangles1) {
            triangleFile << triangle.vertices[0] << " "
                         << triangle.vertices[1] << " "
                         << triangle.vertices[2] << " 0\n";
        }
        for (const auto& triangle : triangles2) {
            triangleFile << triangle.vertices[0] << " "
                         << triangle.vertices[1] << " "
                         << triangle.vertices[2] << " 1\n";
        }
        triangleFile.close();
    } else {
        std::cerr << "Unable to open triangle file\n";
    }


}

AggregationChordScheme::AggregationChordScheme(std::vector<Point_2> vertices, std::vector<std::vector<int>> polygons, bool use_left_right_constraints) :
        objects((vertices),(polygons)){
    objects.calculate_triangulations();
    solver.set_objects(objects);
    solver.create_model(use_left_right_constraints);
}

void AggregationChordScheme::chord_recursion(AggregationSolution &solution_left, AggregationSolution &solution_right) {
    if(solution_right.area==solution_left.area && solution_left.perimeter==solution_right.area){
        return;
    }
    double alpha_middle= compute_crossing_alpha(solution_left,solution_right);
    if (alpha_middle<=0){
        return;
    }
    solver.solve(alpha_middle);
    AggregationSolution solution_middle(solver.get_result_values(),solver.get_result_triangulation_index_based());

    if((solution_left.eval_alpha(alpha_middle)<(1+eps)*solution_middle.eval_alpha(alpha_middle))||
       (solution_right.eval_alpha(alpha_middle)<(1+eps)*solution_middle.eval_alpha(alpha_middle))){
        return;
    }
    solutions.emplace_back(solution_middle);
    chord_recursion(solution_left,  solution_middle);
    chord_recursion(solution_middle, solution_right);


}

double
AggregationChordScheme::compute_crossing_alpha(const AggregationSolution &sol_1, const AggregationSolution &sol_2) {
    double alpha=-1;
    alpha= (sol_1.area-sol_2.area)/(sol_2.perimeter-sol_1.perimeter);
//        assert(alpha>=0);
    return alpha;
}

bool AggregationChordScheme::test_solution_nestedness() {
    std::cout<<std::endl<<"started testing for solution nestedness with following number of solutions: "<<solutions.size()<<std::endl;
    auto verts=objects.get_vertices();
    for(int i=0;i<solutions.size()-1;i++){
        auto current=solutions[i];
        auto next=solutions[i+1];
        for(auto eid: current.e_in){
            for(auto fid:next.e_in){
                auto e=objects.get_edges()[eid];
                auto f=objects.get_edges()[fid];
                if(do_arcs_intersect(verts[e.src],verts[e.dst],verts[f.src],verts[f.dst])){
                    std::cout<<"--------- is not nested ---------"<<std::endl;
                    return false;
                }
            }
        }
    }
    std::cout<<"******* is nested ********"<<std::endl;
    return true;
}


