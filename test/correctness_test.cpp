#include <gtest/gtest.h>
#include "Empty_Triangles.hpp"
#include "data_handling.hpp"
#include "config.hpp"
#include <filesystem>

TEST(correctness, correctness)
{
    srand(time(NULL));
    std::string input_dir = DATA_ROOT + "first_testing_data/";
    for (const auto &entry : std::filesystem::directory_iterator(input_dir))
    {
        std::vector<Point_2> points;
        std::vector<int> indices;
        std::vector<Triangle> triangles;
        std::cout << entry.path().string() << "\n";
        // points = read_points_from_polygon_file(entry.path().string());
        // points = read_points_from_file(DATA_ROOT + "test_points.txt");
        for (int i = 0; i < 200; i++)
        {
            points.emplace_back(Point_2(rand() % 10000, rand() % 10000));
            indices.emplace_back(i);
        }
        for (int x = 0; x < points.size(); x++)
        {
            for (int y = x + 1; y < points.size(); y++)
            {
                for (int z = y + 1; z < points.size(); z++)
                {
                    if (x == 0 && y == 3 && z == 7)
                    {
                        std::cout << "break\n";
                    }
                    if (pointOrientation(points[x], points[y], points[z]) != 0)
                    {
                        Triangle triangle = Triangle({x, y, z});
                        bool is_empty = true;
                        for (int i = 0; i < points.size(); i++)
                        {
                            if (i != x && i != y && i != z)
                            {
                                if (is_point_inside_triangle(i, triangle, points))
                                {
                                    is_empty = false;
                                    break;
                                }
                            }
                        }
                        if (is_empty)
                        {
                            triangles.emplace_back(triangle);
                        }
                    }
                }
            }
        }
        std::cout << "finished brute force\n";
        write_points_to_file(DATA_ROOT + "results/testing_outputs/test_points.txt", points);
        auto triangles_2 = get_empty_triangles(indices, points);
        std::sort(triangles_2.begin(), triangles_2.end(), [](Triangle a, Triangle b)
                  { return a < b; });
        std::sort(triangles.begin(), triangles.end(), [](Triangle a, Triangle b)
                  { return a < b; });
        // for (Triangle triangle : triangles)
        // {
        //     std::cout << triangle.points[0] << " " << triangle.points[1] << " " << triangle.points[2] << "\n";
        // }
        std::cout << "next\n";
        // write_triangles_to_file(DATA_ROOT + "empty_triangles.txt", triangles, points);
        write_triangles_to_file(DATA_ROOT + "results/testing_outputs/empty_triangles2.txt", triangles_2, points);
        // for (Triangle triangle : triangles_2)
        // {
        //     std::cout << triangle.points[0] << " " << triangle.points[1] << " " << triangle.points[2] << "\n";
        // }
        std::cout << triangles.size() << "\n";
        std::cout << triangles_2.size() << "\n";
        ASSERT_TRUE(triangles.size() == triangles_2.size());
        for (int i = 0; i < triangles.size(); i++)
        {
            if (!(triangles[i] == triangles_2[i]))
            {
                std::cout << "Polygon " << i << "\n";
                Triangle triangle = triangles[i];
                std::cout << triangle.vertices[0] << " " << triangle.vertices[1] << " " << triangle.vertices[2] << "\n";
                triangle = triangles_2[i];
                std::cout << triangle.vertices[0] << " " << triangle.vertices[1] << " " << triangle.vertices[2] << "\n";
            }
            ASSERT_TRUE(triangles[i] == triangles_2[i]);
        }
    }
}