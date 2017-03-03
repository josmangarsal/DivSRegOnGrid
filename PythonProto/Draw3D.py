import math
import numpy as np
import mpl_toolkits.mplot3d as a3
import matplotlib.colors as colors
import pylab as pl
import scipy as sp

#LADOS_SIMPLEX = [0, 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120]
EPS = 0.5
DIM = 4

class Simplex(object):
    """Simplex"""
    vertices = []
    longest_edge = (0, (0, 0))
    distances = []

    def __init__(self, vertices):
        self.vertices = vertices
        self.distances = []
        self.size()

    def size(self):
        """Size of each edge of the simplex, sort and LE calculation"""
        for i in xrange(DIM):
            for j in range(i+1, DIM):
                self.distances.append(
                    (self.dist_vertices(self.vertices[i], self.vertices[j]),
                     (i, j)))

        self.distances.sort(key=lambda x: (-x[0], x[1][0], x[1][0]))
        self.longest_edge = self.distances[0]

    def dist_vertices(self, vertice1, vertice2):
        """Distance between two vertices"""
        sum = 0.0

        for i in xrange(DIM):
            sum = sum + math.pow(vertice2[i] - vertice1[i], 2)

        return math.sqrt(sum)

    def copy_vertices(self):
        """Copy vertices to child"""
        vertices_copy = []

        for vertex in self.vertices:
            new_vertex = []
            for coordinate in vertex:
                new_vertex.append(coordinate)
            vertices_copy.append(new_vertex)

        return vertices_copy

    def leb(self):
        """Generate two children from a edge to divide by LEB"""
        if self.longest_edge[0] <= EPS:
            return (0, 0)

        vertices1 = self.copy_vertices()
        vertices2 = self.copy_vertices()

        for i in xrange(DIM):
            pos = (self.vertices[self.longest_edge[1][0]][i] +
                   self.vertices[self.longest_edge[1][1]][i]) / 2.0

            vertices1[self.longest_edge[1][1]][i] = pos
            vertices2[self.longest_edge[1][0]][i] = pos

        return (vertices1, vertices2)

    def print_simplex(self):
        """Print simplex"""
        print 'Dim=', DIM
        print 'Vertices=', self.vertices
        print 'LE=', self.longest_edge
        print 'Dist=', self.distances

GRAPHIC = a3.Axes3D(pl.figure())

def draw_point(vertex):
    """Draw a point"""
    GRAPHIC.scatter(vertex[0], vertex[1], vertex[2], color="g", s=100)

def draw_plain(vertex1, vertex2, vertex3):
    """draw a plain"""
    vtx = [vertex1, vertex2, vertex3]
    tri = a3.art3d.Poly3DCollection([vtx])
    tri.set_color(colors.rgb2hex(sp.rand(3)))
    tri.set_edgecolor('k')
    GRAPHIC.add_collection3d(tri)

def draw_line(vertex1, vertex2, vertex3):
    """Draw a plain with lines"""
    vtx = [vertex1, vertex2, vertex3, vertex1]
    tri = a3.art3d.Line3DCollection([vtx])
    tri.set_edgecolor('k')
    GRAPHIC.add_collection3d(tri)

def draw_simplex(simplex3d):
    """Draw simplex using lines"""
    #simplex3d[3] = simplex3d[3]*-1
    #simplex3d = simplex3d+1

    for vertex in simplex3d:
        draw_point(vertex)

    draw_line(simplex3d[0], simplex3d[1], simplex3d[2])
    draw_line(simplex3d[0], simplex3d[1], simplex3d[3])
    draw_line(simplex3d[0], simplex3d[2], simplex3d[3])
    draw_line(simplex3d[1], simplex3d[2], simplex3d[3])

def to_3d(simplex):
    """4 dimension simplex to 3 dimension representation"""
    matrix_to_3d = [[1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [-1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [0, -2/math.sqrt(6), 1/math.sqrt(12)],
                    [0, 0, -3/math.sqrt(12)]]

    if len(simplex.vertices) != 4:
        print 'Error: Vertices is not a 4 dimension simplex'

    return np.matmul(simplex.vertices, matrix_to_3d)

def main():
    """Main"""

    working_list = []

    initial_unit_simplex_vertices = [[1, 0, 0, 0],
                                     [0, 1, 0, 0],
                                     [0, 0, 1, 0],
                                     [0, 0, 0, 1]]

    simplex_inicial = Simplex(initial_unit_simplex_vertices)
    working_list.append(simplex_inicial)

    for simplex in working_list:
        draw_simplex(to_3d(simplex))
        (vertices1, vertices2) = simplex.leb()

        if vertices1 == 0 and vertices2 == 0:
            continue

        working_list.append(Simplex(vertices1))
        working_list.append(Simplex(vertices2))

    pl.show()

if __name__ == '__main__':
    main()
