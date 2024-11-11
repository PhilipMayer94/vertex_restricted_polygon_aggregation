#include <iostream>
#include <AggregationChordScheme.hpp>
#include <random>
#include <regex>
#include "data_handling.hpp"
#include "json.hpp"
#include "config.hpp"

using json = nlohmann::json;


std::vector<Edge> read_solution_i(int i, const std::string &input_path) {
    std::string edgeFileName = input_path + "/edges";
    std::string triangleFileName = input_path + "/triangles";

    std::vector<Edge> sol;

    // Read the edges
    std::ifstream edgeFile(edgeFileName);
    if (edgeFile.is_open()) {
        int src, dst;
        while (edgeFile >> src >> dst) {
            Edge e = {src, dst};  // Recreate the edge
            sol.emplace_back(e);  // Map back to internal indices
        }
        edgeFile.close();
    } else {
        std::cerr << "Unable to open edge file: " << edgeFileName << "\n";
    }
    return sol;
}


std::vector<std::vector<Edge>> read_all_solutions(const std::string &input_folder) {
    std::vector<std::pair<int, std::vector<Edge>>> indexed_solutions;  // Store index and solutions
    std::regex folder_regex("sol_([0-9]{5})");  // Regex to match folder names like "sol_00001"
    std::smatch match;

    // Iterate through the directories in the input folder
    for (const auto &entry: std::filesystem::directory_iterator(input_folder)) {
        if (entry.is_directory()) {
            std::string folder_name = entry.path().filename().string();
            // Check if the folder name matches the expected pattern
            if (std::regex_match(folder_name, match, folder_regex)) {
                int i = std::stoi(match[1].str());  // Extract the solution index from the folder name
                indexed_solutions.emplace_back(i, read_solution_i(i, input_folder + "/" + folder_name));
            }
        }
    }

    // Sort solutions based on the index
    std::sort(indexed_solutions.begin(), indexed_solutions.end(),
              [](const auto &a, const auto &b) { return a.first < b.first; });

    // Extract only the solutions, discarding the index
    std::vector<std::vector<Edge>> sorted_solutions;
    for (const auto &pair: indexed_solutions) {
        sorted_solutions.push_back(pair.second);
    }

    return sorted_solutions;
}

std::pair<std::vector<Point_2>, std::vector<Polygon>> readVerticesAndPolygons(const std::string &folderName) {
    std::vector<Point_2> points;
    std::string verticesFileName = folderName + "/vertices";
    std::string polygonsFileName = folderName + "/polygons";

    // Read vertices from the file
    std::ifstream verticesFile(verticesFileName);
    if (verticesFile.is_open()) {
        points.clear();  // Clear existing vertices
        double x, y;
        while (verticesFile >> x >> y) {
            points.emplace_back(x, y);  // Assuming Point is a struct with x, y fields
        }
        verticesFile.close();
    } else {
        std::cerr << "Unable to open vertices file\n";
    }

    // Read polygons from the file
    std::ifstream polygonsFile(polygonsFileName);
    std::vector<Polygon> polygons;
    if (polygonsFile.is_open()) {
        polygons.clear();  // Clear existing polygons
        std::string line;
        while (std::getline(polygonsFile, line)) {
            std::istringstream iss(line);
            Polygon polygon;
            int index;
            while (iss >> index) {
                polygon.points.push_back(index);  // Assuming points is a vector of indices
            }
            polygons.push_back(polygon);
        }
        polygonsFile.close();
    } else {
        std::cerr << "Unable to open polygons file\n";
    }
    return {points, polygons};
}

bool test_solution_nestedness(std::vector<Point_2> verts, std::vector<std::vector<Edge>> edge_solutions) {
    std::cout << std::endl << "&&&&&" << edge_solutions.size() << std::endl;
    for (int i = 0; i < edge_solutions.size() - 1; i++) {
        std::cout << std::endl;
        std::cout << i << std::endl;
        auto current = edge_solutions[i];
        auto next = edge_solutions[i + 1];
        for (auto eid: current) {
            for (auto fid: next) {
                auto e = eid;
                auto f = fid;
                if (do_arcs_intersect(verts[e.src], verts[e.dst], verts[f.src], verts[f.dst])) {
                    return false;
                }
            }
        }
    }
    return true;
}


void do_one_instance(const std::string & input_path, const std::string& output_path, bool contraction, bool left_right_constraints, double epsilon){
    auto instance = load_aggregation_instance(input_path);
    AggregationChordScheme aggregator = AggregationChordScheme(instance.first, instance.second,left_right_constraints);
    aggregator.run_with_epsilon(epsilon,contraction);
    aggregator.write_all_solutions(output_path);
}



std::vector<std::pair<double,double>> generateRandomPoints(int size, unsigned int seed) {
    std::vector<std::pair<double,double>> result;

    // Seed the random number generator with the provided seed
    std::mt19937 gen(seed);

    // Define the distribution for x and y in the range [0, 1000]
    std::uniform_real_distribution<> dis(0.1, M_PI*0.9);
    std::uniform_real_distribution<> dist(2, 100);

    double sum=22;
    while(sum>M_PI*1.3){
        result.clear();
        for(int i = 0; i < size; ++i) {
            result.emplace_back(dis(gen),dist(gen));
        }
        sum=0;
        for(auto p:result){
            sum+=p.first;
        }
    }
    result.pop_back();
    return result;
}

Point_2 get_next_point(Point_2 start, Point_2 end, double angle, double length){
    std::cout << "angle: " << angle << std::endl;

    // Calculate the direction vector from start to end
    auto direction = end - start;

    // Rotate the direction vector by the given angle
    double x = cos(angle) * direction.x - sin(angle) * direction.y;
    double y = sin(angle) * direction.x + cos(angle) * direction.y;

    // Normalize the rotated direction vector
    double norm = sqrt(x * x + y * y);
    if (norm == 0) {
        throw std::runtime_error("Invalid direction vector with zero length.");
    }
    x = length * x / norm;
    y = length * y / norm;

    // Return the new point relative to the start point
    return {end.x + x, end.y + y};
}

std::vector<Point_2> get_polyline_poly_line(int size, int seed){
    std::vector<Point_2> result;
    auto randoms=generateRandomPoints(size-1, seed);
    Point_2 end(0,0);

    result.emplace_back(end);
    Point_2 prev(-1,0);
    for(auto p:randoms){
        auto res= get_next_point(prev,end,p.first,p.second);
        result.emplace_back(res);
        prev=end;
        end=res;
    }
    return result;
}

std::vector<Point_2> get_convex_poly_line(int size, int seed){
    bool is_not_fine=true;
    int counter=seed;
    std::vector<Point_2> current;
    while(is_not_fine){
        current=get_polyline_poly_line(size,counter);
        is_not_fine=false;
        for(int i=0;i<current.size();i++){
            int ind1=i;
            int ind2=(i+1)%(int)current.size();
            int ind3=(i+2)%(int)current.size();
            auto test= is_left_oriented(current[ind1],current[ind2],current[ind3]);
            for(int j=0;j<current.size();j++){
                if(j==ind1||j==ind2){
                    continue;
                }
                if(is_left_oriented(current[ind1],current[ind2],current[j])!=test){
                    is_not_fine=true;
                }

            }
        }
        counter++;
    }
    return current;

}

std::vector<Point_2> get_pentagon_polygon(int size, int seed){
    std::vector<Point_2> result;
    auto line =get_convex_poly_line(size,  seed);
    double x_offset=1;
    double y_offset=1;

    Point_2 direction=line[line.size()-1]-line[0];
    Point_2 normal(-direction.y,direction.x);

    Point_2 test=line[0]+normal;
    if(is_left_oriented(line[line.size()-1],line[0],test)!=is_left_oriented(line[line.size()-1],line[0],line[1])){
        normal.x=direction.y;
        normal.y=-direction.x;
        test=line[0]+normal;
    }
    direction.x=direction.x*1.02;

    bool x_offset_wrong=true;
    Point_2 right(0,0);
    Point_2 left(0,0);

    while(x_offset_wrong){
        right=line[line.size()-1]+Point_2(x_offset*direction.x,direction.y*x_offset);
        left=line[0]+Point_2(-x_offset*direction.x,-direction.y*x_offset);

        x_offset_wrong=false;
        Point_2 right_directed=right+normal;
        Point_2 left_directed=left+normal;

        auto test_1= is_left_oriented(right_directed,right,line[0]);
        auto test_2= is_left_oriented(left_directed,left,line[0]);
        for(auto p:line){
            if(is_left_oriented(right_directed,right,p)!=test_1){
                x_offset_wrong=true;
            }
            if(is_left_oriented(left_directed,left,p)!=test_2){
                x_offset_wrong=true;
            }
        }
        x_offset+=1;
    }


    bool y_offset_wrong=true;
    Point_2 top_right(0,0);
    Point_2 top_left(0,0);
    while(y_offset_wrong){
        top_right=right+Point_2(y_offset*normal.x,normal.y*y_offset);
        top_left=left+Point_2(y_offset*normal.x,normal.y*y_offset);
        y_offset_wrong=false;

        auto test_1= is_left_oriented(top_right,top_left,line[0]);
        for(auto p:line){
            if(is_left_oriented(top_right,top_left,p)!=test_1){
                y_offset_wrong=true;
            }
        }
        y_offset+=1;
    }

    line.emplace_back(right);
    line.emplace_back(top_right);
    line.emplace_back(top_left);
    line.emplace_back(left);




    return line;

}


int main(int argc, char *argv[]){


    /*
     *
     * this is the instance laoder for a very specific file format
     * more generic ones are not implemented
     */

    auto instance = load_aggregation_instance(DATA_ROOT + "/first_testing_data/selection_3.csv");


    /*
     *
     * AggregationChordScheme expects
     * First Argument: std::vector of 2D points given in Point_2
     * Second Argument: std::vector<std::vector<int> where every inner vector corresponds to a polygon
     *                  the points are given as indices in the first vector and the first point is not
     *                  repeated in the vector e.g. a quadrilateral is given by 4 indices not 5
     * Third Argument: you can ignore it just set it to true always
     *
     */
    AggregationChordScheme aggregator = AggregationChordScheme(instance.first, instance.second, true);

    /*
     *
     * After initializing call the run_method with some error threshold epsilon if you want all solutions just call it with 1e-15
     * that should be sufficient
     * First Argument: std::vector of 2D points given in Point_2
     *
     */
    aggregator.run_with_epsilon(1e-12);

    /*
     *
     *this methods checks for nestedness by checking if sucessive solutions have properly intersecting boundary edges
     * NOT TESTED!
     *
     */
    aggregator.test_solution_nestedness();


    /*
     *
     * writes the solution into files that can be visualized with some python scripts
     *
     */
    aggregator.write_all_solutions("data/results/village_no_contraction");







    /*
     *
     * you can also read solutions that have already been written to somewhere and then test the nestedness
     *
     */
//    auto res = read_all_solutions("/home/philip/Projects/polygon_aggregation_via_ilp/data/results/bock10");
//    auto res_points = readVerticesAndPolygons("/home/philip/Projects/polygon_aggregation_via_ilp/data/results/bock10");
//    std::cout << std::endl << "-----they are nested: " << std::endl << test_solution_nestedness(res_points.first, res)
//              << std::endl;


    return 0;



}