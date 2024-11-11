//
// Created by philip on 8/14/24.
//

#include "AggregationIlpSolver.hpp"

void AggregationIlpSolver::init_left_right_constraints() {
    for(int i=0;i<objects.get_edges().size();i++){
        int type=objects.get_types_for_valid_edges()[i];
        std::vector<int>left_triangles=objects.get_left_and_right_triangles_for_valid_edges()[i].first;
        std::vector<int>right_triangles=objects.get_left_and_right_triangles_for_valid_edges()[i].second;
        std::string str = "edge_l_r_" + std::to_string(type)+  "_" +std::to_string(i);

        GRBLinExpr expr;

        if(type==0){
            continue;
        }

        if(type==1 || type==2){
            assert(right_triangles.empty());
            assert(!left_triangles.empty());
            for(auto t:left_triangles){
                expr +=  t_in[t] + t_out[t];
            }
            model->addConstr(expr, GRB_EQUAL, 1, str);
        }

        if(type==3){
            for(auto t:left_triangles){
                expr +=  t_in[t] + t_out[t];
            }

            for(auto t:right_triangles){
                expr +=  (-1) *t_in[t] + (-1) * t_out[t];
            }
            model->addConstr(expr, GRB_EQUAL, 0, str);
        }

    }
}

void AggregationIlpSolver::init_xor_constraints() {

    for(int i=0;i<objects.get_edges().size();i++){
        int type=objects.get_types_for_valid_edges()[i];
        std::vector<int>left_triangles=objects.get_left_and_right_triangles_for_valid_edges()[i].first;
        std::vector<int>right_triangles=objects.get_left_and_right_triangles_for_valid_edges()[i].second;

        GRBLinExpr expr_left;
        GRBLinExpr expr_right;
        std::string str_l = "xor_left_" + std::to_string(type)+  "_" +std::to_string(i);
        std::string str_r = "xor_right_" + std::to_string(type)+  "_" +std::to_string(i);

        //removeable (conv*poly)
        if(type==0){
            expr_left=xor_left[i];

            expr_right=xor_right[i];

            model->addConstr(expr_left, GRB_EQUAL, 0, str_l);
            model->addConstr(expr_right, GRB_EQUAL, 1, str_r);
        }

        //poly
        if(type==1){
            for(auto t:left_triangles){
                expr_left += t_in[t] ;
            }
            expr_left+= (-1)* xor_left[i];

            expr_right=xor_right[i];

            model->addConstr(expr_left, GRB_EQUAL, 0, str_l);
            model->addConstr(expr_right, GRB_EQUAL, 1, str_r);
        }

        //conv
        if(type==2){
            for(auto t:left_triangles){
                expr_left +=   t_in[t] ;
            }
            expr_left+= (-1)* xor_left[i];

            expr_right=xor_right[i];

            model->addConstr(expr_left, GRB_EQUAL, 0, str_l);
            model->addConstr(expr_right, GRB_EQUAL, 0, str_r);
        }


        //free
        if(type==3){
            for(auto t:left_triangles){
                expr_left +=   t_in[t] ;
            }
            expr_left+= (-1)* xor_left[i];

            for(auto t:right_triangles){
                expr_right +=  t_in[t] ;
            }
            expr_right+= (-1)* xor_right[i];

            model->addConstr(expr_left, GRB_EQUAL, 0, str_l);
            model->addConstr(expr_right, GRB_EQUAL, 0, str_r);
        }
    }

    //xor constraints
    for(int i=0;i<objects.get_edges().size();i++){
        GRBLinExpr expr_1=xor_left[i]+xor_right[i]+e_in[i];
        GRBLinExpr expr_2=(-1)*xor_left[i]+(-1)*xor_right[i]+e_in[i];;
        GRBLinExpr expr_3=xor_left[i]+(-1)*xor_right[i]+(-1)*e_in[i];;
        GRBLinExpr expr_4=(-1)*xor_left[i]+xor_right[i]+(-1)*e_in[i];;
        std::string str_1 = "xor_1_" +std::to_string(i);
        std::string str_2 = "xor_2_" +std::to_string(i);
        std::string str_3 = "xor_3_" +std::to_string(i);
        std::string str_4 = "xor_4_" +std::to_string(i);
        model->addConstr(expr_1, GRB_LESS_EQUAL, 2, str_1);
        model->addConstr(expr_2, GRB_LESS_EQUAL, 0, str_2);
        model->addConstr(expr_3, GRB_LESS_EQUAL, 0, str_3);
        model->addConstr(expr_4, GRB_LESS_EQUAL, 0, str_4);
    }
}

void AggregationIlpSolver::init_objective() {
    GRBLinExpr expr_objective_edges;

    auto edges=objects.get_edges();
    auto verts=objects.get_vertices();
    auto triangles=objects.get_valid_triangles();
    for(int i=0;i<objects.get_edges().size();i++){
        auto e=objects.get_edges()[i];
        expr_objective_edges+=distance(verts[e.src],verts[e.dst])*e_in[i];
    }

    GRBLinExpr expr_objective_triangles;
    for(int i=0;i<objects.get_valid_triangles().size();i++){
        auto t=objects.get_valid_triangles()[i].vertices;
        expr_objective_triangles+=area_of_triangle(verts[t[0]],verts[t[1]],verts[t[2]])*t_in[i];
    }

    if(lambda<0){
        std::cout<<"lambda was negative"<<std::endl;
        model->setObjectiveN(expr_objective_edges, 0, 1,1, 0,
                             0,"objective_length" );
        model->setObjectiveN(0, 1, 0,1, 0,
                             0,"objective_area" );
    }
    else{
        model->setObjectiveN(expr_objective_edges, 0, 1,lambda, 0,
                             0,"objective_length" );
        model->setObjectiveN(expr_objective_triangles, 1, 1,1, 0,
                             0,"objective_area" );
    }


}

void AggregationIlpSolver::init_variables() {
    for(int i=0;i<objects.get_valid_triangles().size();i++){
        std::string str_in = "T_in_" + std::to_string(i);
        std::string str_out = "T_out_" + std::to_string(i);
        t_in.emplace_back(model->addVar(0.0, 1.0, 0.0, GRB_BINARY, str_in));
        t_out.emplace_back(model->addVar(0.0, 1.0, 0.0, GRB_BINARY, str_out));
    }
    for(int i=0;i<objects.get_edges().size();i++){
        std::string str_xleft = "e_xleft_" + std::to_string(i);
        std::string str_xright = "e_xright_" + std::to_string(i);
        std::string str_e= "e_var_"+ std::to_string(i);
        xor_left.emplace_back(model->addVar(0.0, 1.0, 0.0, GRB_BINARY, str_xleft));
        xor_right.emplace_back(model->addVar(0.0, 1.0, 0.0, GRB_BINARY, str_xright));
        e_in.emplace_back(model->addVar(0.0, 1.0, 0.0, GRB_BINARY, str_e));
    }
}

void AggregationIlpSolver::print_result() {
    auto obj=get_result_triangulation_triangle_based();

    std::cout<<"active triangles:   ";
    for( auto t:std::get<1>(obj) ){
        t.print();
    }
    std::cout<<std::endl;
    std::cout<<"inactive triangles: ";
    for( auto t:std::get<2>(obj) ){
        t.print();
    }
    std::cout<<std::endl;
    std::cout<<"active edges:       ";
    for( auto e:std::get<0>(obj) ){
        e.print();
    }
    std::cout<<std::endl;

}

void AggregationIlpSolver::create_model(bool use_left_right_const) {
    init_variables();
    init_constraints(use_left_right_const);
    model->write("multiobj_c++.lp");
}

void AggregationIlpSolver::solve(double tradeoff) {
    try{
        lambda=tradeoff;
        init_objective();
        model->optimize();
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Error during final solve: " + std::string(e.what()));
    }


//        std::cout<<"result:  "<<model->get(GRB_DoubleAttr_ObjVal)<<std::endl;
}

void AggregationIlpSolver::init_constraints(bool use_left_right_const) {

    if(use_left_right_const){
        init_left_right_constraints();
    }
    init_xor_constraints();
}

std::pair<double, std::pair<double, double>> AggregationIlpSolver::get_result_values() {
    model->set(GRB_IntParam_ObjNumber, 0);
    auto x=model->get(GRB_DoubleAttr_ObjNVal);
    model->set(GRB_IntParam_ObjNumber, 1);
    auto y= model->get(GRB_DoubleAttr_ObjNVal);
    std::cout<<"length: "<<x<<" area: "<<y<< " tradeoff:  "<<lambda<<" tradeoff_value: " << lambda*x+y;
    return {lambda ,std::make_pair(x,y)};
}

std::pair<std::vector<int>, std::pair<std::vector<int>, std::vector<int>>>
AggregationIlpSolver::get_result_triangulation_index_based() {
    std::vector<int> e_in_res;
    std::vector<int> t_in_res;
    std::vector<int> t_out_res;
    for (int i = 0; i < t_in.size(); i++) {
        if (t_in[i].get(GRB_DoubleAttr_X) > 0.9) {
            t_in_res.emplace_back(i);
        }
        if (t_out[i].get(GRB_DoubleAttr_X) > 0.9) {
            t_out_res.emplace_back(i);
        }
    }
    for(int i=0;i<e_in.size();i++){
        if(e_in[i].get(GRB_DoubleAttr_X) > 0.9){
            e_in_res.emplace_back(i);
        }
    }

    return {e_in_res, std::make_pair(t_in_res,t_out_res) };
}

std::tuple<std::vector<Edge>, std::vector<Triangle>, std::vector<Triangle>>
AggregationIlpSolver::get_result_triangulation_triangle_based() {
    auto ob=get_result_triangulation_index_based();
    std::vector<Edge> e_in_res;
    std::vector<Triangle> t_in_res;
    std::vector<Triangle> t_out_res;

    for( auto e:ob.first){
        e_in_res.emplace_back(objects.get_edges()[e]);
    }
    for( auto e:ob.second.first){
        t_in_res.emplace_back(objects.get_valid_triangles()[e]);
    }
    for( auto e:ob.second.second){
        t_out_res.emplace_back(objects.get_valid_triangles()[e]);
    }

    return {e_in_res,t_in_res,t_out_res};
}

void AggregationIlpSolver::write_result_csv(const std::string &folderName) {
    auto data=get_result_triangulation_triangle_based();
    // Create the folder if it doesn't exist
    std::filesystem::create_directories(folderName);

    // Define the file paths
    std::string edgeFileName = folderName + "/edges";
    std::string triangleFileName = folderName + "/triangles";

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

void AggregationIlpSolver::set_objects(TriangulationObjects &obj) {
    objects=obj;
}
