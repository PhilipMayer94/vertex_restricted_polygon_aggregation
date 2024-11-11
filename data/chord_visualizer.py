import os
import numpy as np

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon as MplPolygon
from matplotlib.collections import PatchCollection
from matplotlib.backends.backend_pdf import PdfPages


def load_vertices_and_polygons(path):
    # Load vertices
    vertices_path = os.path.join(path, 'vertices')
    vertices = np.loadtxt(vertices_path)
    
    # Load polygons
    polygons_path = os.path.join(path, 'polygons')
    with open(polygons_path, 'r') as f:
        polygons = [list(map(int, line.split())) for line in f]
    
    return vertices, polygons

def load_edges_and_triangles(path):
    # Load edges
    edges_path = os.path.join(path, 'edges')
    edges = np.loadtxt(edges_path, dtype=int)
    
    # Load triangles
    triangles_path = os.path.join(path, 'triangles')
    with open(triangles_path, 'r') as f:
        triangles = [list(map(int, line.split())) for line in f]
    
    return edges, triangles


def draw_shapes(vertices, polygons, edges, triangles):
    fig, ax = plt.subplots()

    # Draw and fill polygons
    polygon_patches = []
    for polygon in polygons:
        polygon_vertices = vertices[polygon]  # Get the vertices for the polygon
        polygon_patches.append(MplPolygon(polygon_vertices, closed=True, edgecolor='none'))
    
    polygon_collection = PatchCollection(polygon_patches, facecolor='darkblue', edgecolor='none', alpha=0.5)
    ax.add_collection(polygon_collection)

    # Draw and fill triangles
    triangle_patches_0 = []
    triangle_patches_1 = []
    for triangle in triangles:
        triangle_vertices = vertices[triangle[:3]]  # Get the first three vertices for the triangle
        if triangle[3] == 0:
            triangle_patches_0.append(MplPolygon(triangle_vertices, closed=True, edgecolor='black'))
        elif triangle[3] == 1:
            triangle_patches_1.append(MplPolygon(triangle_vertices, closed=True, edgecolor='black'))
    
    triangle_collection_0 = PatchCollection(triangle_patches_0, facecolor='lightblue', edgecolor='none', alpha=0.5)
    ax.add_collection(triangle_collection_0)
    
    triangle_collection_1 = PatchCollection(triangle_patches_1, facecolor='lightgray', edgecolor='none', alpha=0.5)
    ax.add_collection(triangle_collection_1)

    # Draw edges
    for edge in edges:
        start_point = vertices[edge[0]]
        end_point = vertices[edge[1]]
        ax.plot([start_point[0], end_point[0]], [start_point[1], end_point[1]], color='red', linewidth=0.1)


    """
    for idx, (x, y) in enumerate(vertices):
    ax.text(x, y, str(idx), fontsize=1, ha='right', va='bottom', color='black')
    """


    # Set limits and show plot
    ax.set_aspect('equal')
    ax.autoscale_view()
    #plt.savefig("ilp_visualizer_output", format='pdf')
    #plt.show()
    return fig

# Example usage:
    
def process_directory(directory,path_base):
    path = os.path.join(path_base, directory)
    vertices, polygons = load_vertices_and_polygons(path_base)
    edges, triangles = load_edges_and_triangles(path)
    x = draw_shapes(vertices, polygons, edges, triangles)
    
    
    return x
    
def output_complete_solution(base_path):
    base_path = base_path
    directories = sorted([d for d in os.listdir(base_path) if os.path.isdir(os.path.join(base_path, d)) and d.startswith("sol_")])
    print(directories)
    # Process each directory
    pp = PdfPages('chord_visualizer.pdf')
    for directory in directories:
        x=process_directory(directory,base_path)
        pp.savefig(x)
    pp.close()
    
    
    
def output_complete_solution(base_path,skipper):
    base_path = base_path
    directories = sorted([d for d in os.listdir(base_path) if os.path.isdir(os.path.join(base_path, d)) and d.startswith("sol_")])
    print(directories)
    # Process each directory
    pp = PdfPages('chord_visualizer.pdf')
    counter=0
    for directory in directories:
        counter=counter+1
        if(counter<skipper):
            continue
        else:
            counter=0
            x=process_directory(directory,base_path)
            pp.savefig(x)
    pp.close()

def output_one_solution(path,nr):
    bp = path+"/sol_"+str(nr)
    vertices, polygons = load_vertices_and_polygons(path)
    edges, triangles = load_edges_and_triangles(bp)
    x = draw_shapes(vertices, polygons, edges, triangles)
    pp = PdfPages('chord_visualizer_single.pdf')
    pp.savefig(x)
    pp.close()
    #plt.show()
    

    
base_path = "results/bigtest"
#output_one_solution(base_path,"00010")
output_complete_solution(base_path,0)




    
"""path = "results/sel1/xxx"  # Replace with your path
vertices, polygons = load_vertices_and_polygons(path)
edges, triangles = load_edges_and_triangles(path)
x=draw_shapes(vertices, polygons, edges, triangles)
pp = PdfPages('foo.pdf')
pp.savefig(x)





print("Vertices:\n", vertices)
print("Polygons:\n", polygons)
print("Edges:\n", edges)
print("Triangles:\n", triangles)"""