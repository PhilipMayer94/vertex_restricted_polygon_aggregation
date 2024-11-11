#include "TriangulationObjects.hpp"
#include "Empty_Triangles.hpp"
#include <utility>

TriangulationObjects::TriangulationObjects(std::vector<Point_2> points, std::vector<std::vector<int>> polygons)
{
    this->points = points;
    this->polygons = std::vector<Polygon>();
    for(int i=0;i<polygons.size();i++){
        this->polygons.emplace_back(Polygon(polygons[i]));
    }
}

const std::vector<Point_2> & TriangulationObjects::get_vertices(){
    return this->points;
}

const std::vector<Edge> & TriangulationObjects::get_edges()
{
    return this->edges;
}

const std::vector<int> & TriangulationObjects::get_types_for_valid_edges()
{
    return this->edge_classifications;
}

const std::vector<Triangle> & TriangulationObjects::get_valid_triangles(){
    return this->triangles;
}

const std::vector<std::pair<std::vector<int>,std::vector<int>>> & TriangulationObjects::get_left_and_right_triangles_for_valid_edges(){
    return left_right_triangles;
}

void TriangulationObjects::calculate_triangulations()
{
    if(has_run){
        return;
    }
    else{
        has_run=true;
    }


    Edge_Data_Storage edge_storage = Edge_Data_Storage(points.size());
    // mark polygon edges as such
    for (Polygon polygon : polygons)
    {
        for (int i = 0; i < polygon.points.size() - 1; i++)
        {
            edge_storage.set_is_polygon_edge(polygon.points[i], polygon.points[i] + 1);
        }
        edge_storage.set_is_polygon_edge(polygon.points[polygon.points.size()-1], polygon.points[0]);
    }
    // find invalid edges
    // iterate over all edges
    for (int i = 0; i < points.size() - 1; i++)
    {
        for (int j = i + 1; j < points.size(); j++)
        {
            if (!edge_storage.get_is_polygon_edge(i, j))
            {
                Point_2 point_i = points[i];
                Point_2 point_j = points[j];
                // iterate over polygons
                for (Polygon polygon : polygons)
                {
                    // iterate over edges of polygon
                    int index1 = polygon.points[polygon.points.size() - 1];
                    for (int index2 : polygon.points)
                    {
                        if (!((i == index1 && j == index2) || (j == index1 && i == index2)))
                        {
                            if (do_segments_intersect(point_i, point_j, points[index1], points[index2]))
                            {
                                edge_storage.invalidate(i, j);
                            }
                        }
                        index1 = index2;
                    }

                    if (is_edge_inside_polygon(point_i, point_j, polygon, points))
                    {
                        edge_storage.invalidate(i, j);
                    }
                }
            }
        }
    }
    std::vector<int> indices;
    for(int i=0;i<points.size();i++){
        indices.emplace_back(i);
    }
    triangles = get_empty_triangles(indices, points);
    // delete triangles with invalid edges
    for (int i = 0; i < triangles.size(); i++)
    {
        std::array<int, 3> triangle_indices = triangles[i].vertices;
        if (edge_storage.get_is_invalid(triangle_indices[0], triangle_indices[1]) || edge_storage.get_is_invalid(triangle_indices[1], triangle_indices[2]) || edge_storage.get_is_invalid(triangle_indices[0], triangle_indices[2]))
        {
            triangles[i] = triangles[triangles.size() - 1];
            triangles.pop_back();
            i--;
        }
        // if a polygon is a triangle, it should not be an empty triangle; it is assumed that polygons are simple and not touching nor intersecting
        else if(edge_storage.get_is_polygon_edge(triangle_indices[0], triangle_indices[1]) && edge_storage.get_is_polygon_edge(triangle_indices[1], triangle_indices[2]) && edge_storage.get_is_polygon_edge(triangle_indices[0], triangle_indices[2])){
            triangles[i] = triangles[triangles.size() - 1];
            triangles.pop_back();
            i--;
            std::cout << "triangle polygon\n";
        }
    }
    // put valid edges into a vector
    for (int i = 0; i < points.size() - 1; i++)
    {
            for (int j = i + 1; j < points.size(); j++)
            {
                if (!edge_storage.get_is_invalid(i, j))
                {
                    edges.emplace_back(Edge(i, j));
                    edge_storage.set_index(i, j, edges.size() - 1);
                }
            }
    }

    // for each edge: put all its triangles in their respective list, depending on whether it lies on the left or right side of edge
    this->left_right_triangles = std::vector<std::pair<std::vector<int>,std::vector<int>>>(edges.size(), std::pair<std::vector<int>,std::vector<int>>(std::vector<int>(), std::vector<int>()));

    int p1;
    int p2;
    int p3;
    for (int i=0;i<triangles.size();i++)
    {
            p1 = triangles[i].vertices[0];
            p2 = triangles[i].vertices[1];
            p3 = triangles[i].vertices[2];

            if (p1 == 4 && p2 == 5 && p3 == 7)
            {
                std::cout << "break";
            }
            add_triangle_to_respective_list(points[p1], points[p2], points[p3], edge_storage.get_index(p1, p2), i);
            add_triangle_to_respective_list(points[p1], points[p3], points[p2], edge_storage.get_index(p1, p3), i);
            add_triangle_to_respective_list(points[p2], points[p3], points[p1], edge_storage.get_index(p2, p3), i);
    }

    // classify each edge
    edge_classifications = std::vector<int>(edges.size());
    for (int i = 0; i < edges.size(); i++)
    {
            if (left_right_triangles[i].first.size() == 0 && left_right_triangles[i].second.size() == 0)
            {
                edge_classifications[i] = 0;
            }
            else if (left_right_triangles[i].first.size() == 0 || left_right_triangles[i].second.size() == 0)
            {
                if(left_right_triangles[i].first.size() == 0 ){
                    std::swap(left_right_triangles[i].second,left_right_triangles[i].first);

                }


                if (edge_storage.get_is_polygon_edge(edges[i].src, edges[i].dst))
                {
                    edge_classifications[i] = 1;
                }
                else
                {
                    edge_classifications[i] = 2;
                }
            }
            else
            {
                edge_classifications[i] = 3;
            }
    }
    }

void TriangulationObjects::add_triangle_to_respective_list(Point_2 p1, Point_2 p2, Point_2 p3, int index, int triangle_index)
    {
        if (pointOrientation(p1, p2, p3) == 1)
        {
            left_right_triangles[index].second.emplace_back(triangle_index);
        }
        else
        {
            left_right_triangles[index].first.emplace_back(triangle_index);
        }
    }

void
TriangulationObjects::set_polygons(const std::vector<Point_2> &points, const std::vector<std::vector<int>> &polygons) {
    this->points = points;
    this->polygons = std::vector<Polygon>();
    for(int i=0;i<polygons.size();i++){
        this->polygons.emplace_back(Polygon(polygons[i]));
    }
}

void TriangulationObjects::writeVerticesAndPolygons(const std::string &folderName) {
    std::vector<Point_2> vertices = points;
    // Create the folder if it doesn't exist
    std::filesystem::create_directories(folderName);

    // Define the file paths
    std::string verticesFileName = folderName + "/vertices";
    std::string polygonsFileName = folderName + "/polygons";

    // Write the vertices to a space-separated file
    std::ofstream verticesFile(verticesFileName);
    if (verticesFile.is_open()) {
        for (const auto& vertex : vertices) {
            verticesFile << vertex.x << " " << vertex.y << "\n";
        }
        verticesFile.close();
    } else {
        std::cerr << "Unable to open vertices file\n";
    }

    // Write the polygons to a space-separated file
    std::ofstream polygonsFile(polygonsFileName);
    if (polygonsFile.is_open()) {
        for (const auto& polygon : polygons) {
            for (size_t i = 0; i < polygon.points.size(); ++i) {
                polygonsFile << polygon.points[i];
                if (i < polygon.points.size() - 1) {
                    polygonsFile << " ";  // Space between indices
                }
            }
            polygonsFile << "\n";
        }
        polygonsFile.close();
    } else {
        std::cerr << "Unable to open polygons file\n";
    }
}
