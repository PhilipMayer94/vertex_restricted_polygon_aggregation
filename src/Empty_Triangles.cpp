#include "Empty_Triangles.hpp"
#include <queue>
//debugging
#include "config.hpp"
#include "data_handling.hpp"

void proceed(int seen_index, int j, std::vector<std::queue<int>> &queues, std::vector<int> &indices, std::vector<Point_2> &points, std::vector<Triangle> &triangles, int left_point_index, int i)
{
    //std::cout << indices[i] << " " << indices[j] << " " << indices[seen_index] << "\n";
    Point_2 current_point = points[indices[j]];
    // find all points with lower index visible from current point
    Point_2 seen_point = points[indices[seen_index]];
    if (pointOrientation(points[left_point_index], seen_point, current_point) != 0)
    {
        triangles.push_back(Triangle({left_point_index, indices[seen_index], indices[j]}));
    }
    int queue_index = seen_index - i - 1;
    while (!queues[queue_index].empty())
    {
        int front_index = queues[queue_index].front();
        Point_2 front = points[indices[front_index]];
        if (pointOrientation(current_point, seen_point, front) <= 0)
        {
            break;
        }
        proceed(front_index, j, queues, indices, points, triangles, left_point_index, i);
        queues[queue_index].pop();
    }
    queues[j - i - 1].push(seen_index);
}

std::vector<Triangle> get_empty_triangles(std::vector<int> indices, std::vector<Point_2> points)
{
    // sort indices (point) to traverse from lefttmost to rightmost
    //(after sorting, leftmost is first)
    std::sort(indices.begin(), indices.end(), [points](int a, int b)
              { return points[a] < points[b]; });
    std::vector<Triangle> triangles;
    int left_point_index;
    int prev;
    Point_2 left_point;
    //for testing only
    for(int i = 0; i<indices.size()-1;i++){
        if(points[indices[i]]==points[indices[i+1]]){
            std::cout << "double Points \n";
            std::cout << indices[i] << " " << points[indices[i]].x << " " << points[indices[i]].y << "\n";
            write_points_to_file(DATA_ROOT+"results/double_point_test.txt", points);
        }
    }
    // find empty triangles with left_point as their leftmost point
    for (int i = indices.size() - 3; i >= 0; i--)
    {
        left_point_index = indices[i];
        // if(left_point_index == 10){
        //     std::cout<< "break";
        // }
        left_point = points[left_point_index];
        // sort points, so points after left point represent star shaped polygon (points given counter clockwise)
        //if points co
        std::sort(indices.begin() + i + 1, indices.end(), [points, left_point](int a, int b)
                  { int orientation = pointOrientation(left_point, points[a], points[b]);
                    if(orientation == 0){
                        if(points[a].x == points[b].x){
                            return points[a].y > points[b].y;
                        }
                        return points[a].x > points[b].x;
                    }
                    return orientation < 0; });
        // for queue: reference point indices by their position after left_point
        std::vector<std::queue<int>> queues(indices.size() - i - 1, std::queue<int>());
        for (int j = i + 2; j < indices.size(); j++)
        {
            if (j == indices.size() - 1 || pointOrientation(left_point, points[indices[j]], points[indices[j + 1]]) != 0)
            {
                int prev = j - 1;
                while (prev >= 0 && pointOrientation(left_point, points[indices[j]], points[indices[prev]]) == 0)
                {
                    prev--;
                }
                if (prev > i)
                {
                    proceed(prev, j, queues, indices, points, triangles, left_point_index, i);
                }
            }
        }
    }

    return triangles;
}
