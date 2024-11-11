//
// Created by philip on 8/14/24.
//

#ifndef POLYGONAGGREGATION_AGGREGATIONILPSOLVER_HPP
#define POLYGONAGGREGATION_AGGREGATIONILPSOLVER_HPP

#include <cassert>
#include <gurobi_c++.h>
#include <filesystem>
#include "BasicDataStructures.hpp"
#include "TriangulationObjects.hpp"

class AggregationIlpSolver{
public:
    ~AggregationIlpSolver(){
        delete(model);
        delete(env);
    }

    explicit AggregationIlpSolver( TriangulationObjects & obj):objects(std::move(obj)){
        env=new GRBEnv();
        env->set(GRB_IntParam_OutputFlag,0);
        model=new GRBModel(env);
    }

    explicit AggregationIlpSolver( ){
        env=new GRBEnv();
        model=new GRBModel(env);
    }

    void solve(double lambda);
    void create_model(bool use_left_right_const=true);
    void set_objects(TriangulationObjects & obj);


    std::pair<double,std::pair<double,double>> get_result_values();
    std::pair<std::vector<int>,std::pair<std::vector<int>,std::vector<int>>> get_result_triangulation_index_based();
    std::tuple<std::vector<Edge>,std::vector<Triangle>,std::vector<Triangle>> get_result_triangulation_triangle_based();

    void print_result();
    void write_result_csv(const std::string& folderName);

    void reset_additional_constraints(){
        for(auto c:added_constraints){
            model->remove(c);
        }
        added_constraints.clear();
    }


    void add_constraints_to_model(const std::vector<int>& tr_in, const std::vector<int>& tr_out){
        reset_additional_constraints();

        for(auto t: tr_in){
            added_constraints.emplace_back( model->addConstr(t_in[t], GRB_EQUAL, 1, ("additional_plus_"+std::to_string(t))));
            added_constraints.emplace_back( model->addConstr(t_out[t], GRB_EQUAL, 0, ("additional_plus_"+std::to_string(t))));
        }
        for(auto t: tr_out){
            added_constraints.emplace_back( model->addConstr(t_out[t], GRB_EQUAL, 1, ("additional_minus_"+std::to_string(t))));
            added_constraints.emplace_back( model->addConstr(t_in[t], GRB_EQUAL, 0, ("additional_minus_"+std::to_string(t))));
        }
    }


private:

    void init_variables();
    void init_constraints(bool use_left_right_const);
    void init_left_right_constraints();
    void init_xor_constraints();

    void init_objective();




    TriangulationObjects objects;

    GRBModel * model;
    GRBEnv * env;
    double lambda=-1;


    std::vector<GRBVar> t_in;
    std::vector<GRBVar> t_out;
    std::vector<GRBVar> e_in;
    std::vector<GRBVar> xor_left;
    std::vector<GRBVar> xor_right;

    std::vector<GRBConstr> added_constraints;


    bool use_left_right_constraints=true;
};




#endif //POLYGONAGGREGATION_AGGREGATIONILPSOLVER_HPP