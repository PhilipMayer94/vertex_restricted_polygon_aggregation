#include <gtest/gtest.h>
#include "Empty_Triangles.hpp"
#include "data_handling.hpp"
#include "config.hpp"
#include <filesystem>

TEST(num_triangles, first_testing_data)
{
    std::string input_dir = DATA_ROOT + "first_testing_data/";
    for (const auto &entry : std::filesystem::directory_iterator(input_dir))
    {
        std::cout << entry.path().string() << "\n";
        std::vector<Point_2> points = read_points_from_polygon_file(entry.path().string());
        std::vector<int> indices;
        std::vector<Triangle> triangles;
        points = pre_process(points);
        for (int i = 0; i < points.size(); i++)
        {
            indices.emplace_back(i);
        }
        triangles = get_empty_triangles(indices, points);
        std::cout << points.size() << " " << triangles.size() << "\n";
    }
}