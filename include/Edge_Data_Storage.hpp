#ifndef EMPTY_TRIANGLES_EDGE_DATA_STORAGE_HPP
#define EMPTY_TRIANGLES_EDGE_DATA_STORAGE_HPP

#include <vector>

struct Edge_Data
{
    bool is_polygon_edge;
    bool is_invalid;
    int index;

    Edge_Data(){
        is_polygon_edge = false;
        is_invalid = false;
    }
};


struct Edge_Data_Storage
{
    //"(n-1)x(n-1)" upper triangle Matrix
    std::vector<std::vector<Edge_Data>> edge_storage;

    Edge_Data_Storage(int size)
    {
        edge_storage = std::vector<std::vector<Edge_Data>>(size - 1);
        for (int i = 0; i < size - 1; i++)
        {
            edge_storage[i] = std::vector<Edge_Data>(size - 1 - i, Edge_Data());
        }
    }

    void set_is_polygon_edge(int index1, int index2)
    {
        if (index1 > index2)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }
        if (index1 == 0 && index2 == 6)
        {
            std::cout << "break";
        }
        edge_storage[index1][edge_storage.size() - index2].is_polygon_edge = true;
    }

    void set_index(int index1, int index2, int index)
    {
        if (index1 > index2)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }
        edge_storage[index1][edge_storage.size() - index2].index = index;
    }

    int get_index(int index1, int index2)
    {
        if (index1 > index2)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }
        return edge_storage[index1][edge_storage.size() - index2].index;
    }

    int get_is_polygon_edge(int index1, int index2)
    {
        if (index1 > index2)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }
        return edge_storage[index1][edge_storage.size() - index2].is_polygon_edge;
    }

    void invalidate(int index1, int index2)
    {
        if (index1 > index2)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }
        if (index1 == 0 && index2 == 2)
        {
            std::cout << "break";
        }
        edge_storage[index1][edge_storage.size() - index2].is_invalid = true;
    }

    void set_data(int index1, int index2, Edge_Data data)
    {
        if (index1 > index2)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }
        edge_storage[index1][edge_storage.size() - index2] = data;
    }

    bool get_is_invalid(int index1, int index2)
    {
        if (index1 > index2)
        {
            int tmp = index1;
            index1 = index2;
            index2 = tmp;
        }
        return edge_storage[index1][edge_storage.size() - index2].is_invalid;
    }
};

#endif