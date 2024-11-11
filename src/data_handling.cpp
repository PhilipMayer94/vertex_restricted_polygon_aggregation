#include "data_handling.hpp"
#include <fstream>
// #include <iostream>
#include <iomanip>
#include <limits>
#include <iostream>

void write_triangles_to_file(std::string output_path, std::vector<Triangle> triangles, std::vector<Point_2> points)
{
    std::ofstream output;
    output.open(output_path);
    Point_2 point;
    for (int i = 0; i < triangles.size(); i++)
    {
        for (int point_index : triangles[i].vertices)
        {
            point = points[point_index];
            output << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << i << " " << point.x << " " << point.y << "\n";
        }
    }
    output.close();
}

void write_points_to_file(std::string output_path, std::vector<Point_2> points)
{
    std::ofstream output;
    output.open(output_path);
    for (Point_2 point : points)
    {
        output << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << point.x << " " << point.y << "\n";
    }

    output.close();
}

std::vector<Point_2> read_points_from_file(std::string input_path)
{
    std::vector<Point_2> points;
    std::ifstream datafile(input_path);
    std::string current_line;
    double x;
    double y;
    if (datafile.is_open())
    {
        while (getline(datafile, current_line))
        {
            std::stringstream row(current_line);
            int col = 0;
            std::string token;
            while (std::getline(row, token, ' '))
            {
                if (col == 0)
                {
                    x = std::stod(token); //-353400;
                }
                else if (col == 1)
                {
                    y = std::stod(token); //-5619300;
                }
                col++;
            }
            points.emplace_back(Point_2(x, y));
        }
    }
    return points;
}

std::vector<Point_2> read_points_from_polygon_file2(const std::string input_path)
{
    int last_beginning = 0;
    int line = 0;
    std::vector<Point_2> points;
    std::ifstream datafile(input_path);
    std::string current_line;
    Point_2 first_point;
    bool poly_id_in_second_c;
    bool finished = false;
    double x;
    double y;
    bool first_point_set = false;
    if (datafile.is_open())
    {
        getline(datafile, current_line);
        while (getline(datafile, current_line))
        {
            std::stringstream row(current_line); // >> polygon_id >> part_id >> x >> y;
            int col = 0;
            std::string token;
            while (std::getline(row, token, ','))
            {
                if (col == 2)
                {
                    x = std::stod(token); //-353400;
                }
                else if (col == 3)
                {
                    y = std::stod(token); //-5619300;
                }
                col++;
            }
            Point_2 point = Point_2(x, y);
            if (line == last_beginning || !(point == points[last_beginning]))
            {
                points.emplace_back();
            }
            else
            {
                last_beginning = line + 1;
            }
            line++;
        }
    }
    else
    {
        std::cerr << "file not open";
    }
    datafile.close();
    return points;
}

std::vector<Point_2> read_points_from_polygon_file(const std::string input_path)
{
    std::vector<Point_2> points;
    std::ifstream datafile(input_path);
    std::string current_line;
    std::string polygon_id;
    std::string current_polygon_id = "\"0\"";
    std::string part_id;
    Point_2 first_point;
    bool poly_id_in_second_c;
    bool finished = false;
    double x;
    double y;
    bool first_point_set = false;
    if (datafile.is_open())
    {
        getline(datafile, current_line);
        while (!finished && getline(datafile, current_line))
        {
            std::stringstream row(current_line); // >> polygon_id >> part_id >> x >> y;
            int col = 0;
            std::string token;
            while (std::getline(row, token, ','))
            {
                if (col == 0)
                {
                    polygon_id = token;
                }
                else if (col == 1)
                {
                    part_id = token;
                }
                else if (col == 2)
                {
                    x = std::stod(token); //-353400;
                }
                else if (col == 3)
                {
                    y = std::stod(token); //-5619300;
                }
                col++;
            }

            if (current_polygon_id != polygon_id)
            {
                points.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = polygon_id;
                finished = true;
                poly_id_in_second_c = false;
            }
            else if (current_polygon_id != part_id)
            {
                points.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = part_id;
                finished = true;
                poly_id_in_second_c = true;
            }
            else
            {
                if (!first_point_set)
                {
                    first_point = Point_2(x, y);
                    points.emplace_back(Point_2(x, y));
                    first_point_set = true;
                }
                else if (!(Point_2(x, y) == first_point))
                {
                    points.emplace_back(Point_2(x, y));
                }
            }
        }
        while (getline(datafile, current_line))
        {
            std::stringstream row(current_line); // >> polygon_id >> part_id >> x >> y;
            int col = 0;
            std::string token;
            while (std::getline(row, token, ','))
            {
                if (col == 0)
                {
                    polygon_id = token;
                }
                else if (col == 1)
                {
                    part_id = token;
                }
                else if (col == 2)
                {
                    x = std::stod(token); //-353400;
                }
                else if (col == 3)
                {
                    y = std::stod(token); //-5619300;
                }
                col++;
            }

            if (!poly_id_in_second_c && current_polygon_id != polygon_id)
            {
                // if (!(polygon[0] == polygon[polygon.size() - 1]))
                // {
                //     std::cout << "double points\n";
                // }
                points.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = polygon_id;
            }
            else if (poly_id_in_second_c && current_polygon_id != part_id)
            {
                // if (!(polygon[0] == polygon[polygon.size() - 1]))
                // {
                //     std::cout << "double points\n";
                // }
                points.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = part_id;
            }
            else
            {
                if (!(Point_2(x, y) == first_point))
                {
                    points.emplace_back(Point_2(x, y));
                }
            }
        }
    }
    else
    {
        std::cerr << "file not open";
    }
    datafile.close();
    return points;
}

std::vector<Point_2> pre_process(std::vector<Point_2> &points)
{
    int i = 0;
    Point_2 min_point = points[0];
    for (Point_2 point : points)
    {
        if (point < min_point)
        {
            min_point = point;
        }
    }
    double min_x = min_point.x;
    double min_y = min_point.y;

    for (Point_2 &point : points)
    {
        point.x = point.x - min_x;
        point.y = point.y - min_y;
    }
    std::cout << min_x << " " << min_y << "\n";
    return points;
}

void write_edges_to_file(const std::string output_path, const std::vector<Edge> &edges, const std::vector<int> &classifications){
    std::ofstream output;
    output.open(output_path);

    for (int i = 0; i < edges.size(); i++)
    {
            output << classifications[i] << " " << edges[i].src << " " << edges[i].dst << "\n";
    }
    output.close();
}

std::pair<std::vector<Point_2>, std::vector<std::vector<int>>> load_aggregation_instance(const std::string input_path) {
    auto poly= load_polygons(input_path);
    std::vector<Point_2> res_vertices;
    std::vector<std::vector<int>> res_polygons;

    int c=0;
    for (auto &pol: poly){
        res_polygons.emplace_back();
        for(auto& p:pol ){

            res_polygons[res_polygons.size()-1].emplace_back(c);
            res_vertices.emplace_back(p);
            c++;
        }

    }
    return {res_vertices,res_polygons};
}

std::vector<std::vector<Point_2>> load_polygons(const std::string input_path) {
    std::vector<std::vector<Point_2>> polygons;
    std::vector<Point_2> polygon;
    std::ifstream datafile(input_path);
    std::string current_line;
    std::string polygon_id;
    std::string current_polygon_id = "\"0\"";
    std::string part_id;
    Point_2 first_point;
    bool poly_id_in_second_c;
    bool finished = false;
    double x;
    double y;
    bool first_point_set = false;
    if (datafile.is_open())
    {
        getline(datafile, current_line);
        while (!finished && getline(datafile, current_line))
        {
            std::stringstream row(current_line); // >> polygon_id >> part_id >> x >> y;
            int col = 0;
            std::string token;
            while (std::getline(row, token, ','))
            {
                if (col == 0)
                {
                    polygon_id = token;
                }
                else if (col == 1)
                {
                    part_id = token;
                }
                else if (col == 2)
                {
                    x = std::stod(token); //-353400;
                }
                else if (col == 3)
                {
                    y = std::stod(token); //-5619300;
                }
                col++;
            }

            if (current_polygon_id != polygon_id)
            {
                polygons.emplace_back(polygon);
                polygon = std::vector<Point_2>();
                polygon.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = polygon_id;
                finished = true;
                poly_id_in_second_c = false;
            }
            else if (current_polygon_id != part_id)
            {
                polygons.emplace_back(polygon);
                polygon = std::vector<Point_2>();
                polygon.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = part_id;
                finished = true;
                poly_id_in_second_c = true;
            }
            else
            {
                if (!first_point_set)
                {
                    first_point = Point_2(x, y);
                    polygon.emplace_back(Point_2(x, y));
                    first_point_set = true;
                }
                else if (!(Point_2(x, y) == first_point))
                {
                    polygon.emplace_back(Point_2(x, y));
                }
            }
        }
        while (getline(datafile, current_line))
        {
            std::stringstream row(current_line); // >> polygon_id >> part_id >> x >> y;
            int col = 0;
            std::string token;
            while (std::getline(row, token, ','))
            {
                if (col == 0)
                {
                    polygon_id = token;
                }
                else if (col == 1)
                {
                    part_id = token;
                }
                else if (col == 2)
                {
                    x = std::stod(token); //-353400;
                }
                else if (col == 3)
                {
                    y = std::stod(token); //-5619300;
                }
                col++;
            }

            if (!poly_id_in_second_c && current_polygon_id != polygon_id)
            {
                // if (!(polygon[0] == polygon[polygon.size() - 1]))
                // {
                //     std::cout << "double points\n";
                // }
                polygons.emplace_back(polygon);
                polygon = std::vector<Point_2>();
                polygon.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = polygon_id;
            }
            else if (poly_id_in_second_c && current_polygon_id != part_id)
            {
                // if (!(polygon[0] == polygon[polygon.size() - 1]))
                // {
                //     std::cout << "double points\n";
                // }
                polygons.emplace_back(polygon);
                polygon = std::vector<Point_2>();
                polygon.emplace_back(Point_2(x, y));
                first_point = Point_2(x, y);
                current_polygon_id = part_id;
            }
            else
            {
                if (!(Point_2(x, y) == first_point))
                {
                    polygon.emplace_back(Point_2(x, y));
                }
            }
        }
    }
    else
    {
        std::cerr << "file not open";
    }
    polygons.emplace_back(polygon);

    double min_x=100000000;
    double min_y=100000000;
    for(auto &poly:polygons){
        for(auto& p:poly ){
            if(min_x>p.x){
                min_x=p.x;
            }
            if(min_y>p.y){
                min_y=p.y;
            }
        }
    }

    for(auto &poly:polygons) {
        for (auto &p: poly) {
            p.x = p.x - min_x;
            p.y = p.y - min_y;
        }
    }

    datafile.close();
    return polygons;
}

void write_polygons_to_file(std::string output_path, const std::vector<Polygon> &polygons, const std::vector<Point_2> points)
{
    std::ofstream output;
    output.open(output_path);
    for (int i = 0; i < polygons.size(); i++)
    {
        for (int point : polygons[i].points)
        {
            output << std::fixed << std::setprecision(std::numeric_limits<double>::max_digits10) << i << " " << points[point].x << " " << points[point].y << "\n";
        }
    }
    output.close();
}