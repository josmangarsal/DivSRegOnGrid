"""2D and 3D simplex division"""

import math
import numpy as np
import mpl_toolkits.mplot3d as a3
import matplotlib.colors as colors
import pylab as pl
import scipy as sp

#LADOS_SIMPLEX = [0, 0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120]
EPS = 0.7
DIM = 3

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

class CDSimplex(object):
    """CD Simplex"""
    centre = []
    radius = 0.0
    size = 0.0
    up_orientation = True # True is normal orientation

    def __init__(self, centre, radius, size, up_orientation):
        self.centre = centre
        self.radius = radius
        self.size = size
        self.up_orientation = up_orientation

    def divide_2usc(self, red_factor):
        """2USC regular division"""
        new_simplices = []
        for j in xrange(DIM): # Number of new simplices
            new_center = []

            for k in xrange(DIM): # New center
                if self.up_orientation is True:
                    new_center.append(self.centre[k] - (1.0 - red_factor) * self.radius / DIM)
                else:
                    new_center.append(self.centre[k] + (1.0 - red_factor) * self.radius / DIM)

            if self.up_orientation is True:
                new_center[j] += self.radius * (1.0 - red_factor)
            else:
                new_center[j] -= self.radius * (1.0 - red_factor)

            new_simplices.append(CDSimplex(new_center, red_factor * self.radius,
                                           red_factor * self.size, True))

        return new_simplices

    def divide_2musc(self, red_factor):
        """2MUSC regular division"""
        new_2nusc_simplex = None

        beta = ((DIM - 1.0) / DIM)

        if red_factor < beta:
            up_down_ratio = DIM * (1.0 - red_factor) - 1.0
            new_2nusc_simplex = CDSimplex(self.centre, up_down_ratio * self.radius,
                                          up_down_ratio * self.size, False)

        new_2usc_simplices = self.divide_2usc(red_factor)

        if new_2nusc_simplex is not None:
            new_2usc_simplices.append(new_2nusc_simplex)

        return new_2usc_simplices

    def print_cdsimplex(self):
        """Print simplex"""
        print 'Dim=', DIM
        print 'Centre=', self.centre
        print 'Radius=', self.radius
        print 'Size=', self.size
        print 'Up=', self.up_orientation

# Drawing

GRAPHIC = a3.Axes3D(pl.figure())

def draw_point(vertex, color):
    """Draw a point"""
    GRAPHIC.scatter(vertex[0], vertex[1], vertex[2], c=color, s=100)

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
    simplex3d = to_3d(simplex3d)

    draw_line(simplex3d[0], simplex3d[1], simplex3d[2])
    draw_line(simplex3d[0], simplex3d[1], simplex3d[3])
    draw_line(simplex3d[0], simplex3d[2], simplex3d[3])
    draw_line(simplex3d[1], simplex3d[2], simplex3d[3])

def draw_simplex_3d_vertices(simplex3d):
    """Draw simplex using lines"""
    simplex3d = to_3d(simplex3d)

    draw_line(simplex3d[0], simplex3d[1], simplex3d[2])
    draw_line(simplex3d[0], simplex3d[1], simplex3d[3])
    draw_line(simplex3d[0], simplex3d[2], simplex3d[3])
    draw_line(simplex3d[1], simplex3d[2], simplex3d[3])

    for vertex in simplex3d:
        draw_point(vertex, 'g')

def cdsimplex_to_vertices(cdsimplex):
    """CDStoV"""
    matrix_to_vertices = []
    for i in xrange(DIM):
        coor = []
        for j in xrange(DIM):
            if i == j:
                coor.append((DIM - 1.0) / DIM)
            else:
                coor.append(-1.0 / DIM)
        matrix_to_vertices.append(coor)

    vertices = []
    for i in xrange(DIM):
        coor = []
        for j in xrange(DIM):
            if cdsimplex.up_orientation is True:
                coor.append(cdsimplex.centre[j] + cdsimplex.radius * matrix_to_vertices[i][j])
            else:
                coor.append(cdsimplex.centre[j] - cdsimplex.radius * matrix_to_vertices[i][j])
        vertices.append(coor)

    return vertices

def draw_cdsimplex_3d(cdsimplex):
    """Draw cd simplex using centre, radius and vertices"""
    matrix_to_3d = [[1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [-1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [0, -2/math.sqrt(6), 1/math.sqrt(12)],
                    [0, 0, -3/math.sqrt(12)]]

    centre_3d = np.matmul(cdsimplex.centre, matrix_to_3d)

    draw_point(centre_3d, 'blue')

    vertices = cdsimplex_to_vertices(cdsimplex)

    vertices_3d = np.matmul(vertices, matrix_to_3d)

    draw_line(vertices_3d[0], vertices_3d[1], vertices_3d[2])
    draw_line(vertices_3d[0], vertices_3d[1], vertices_3d[3])
    draw_line(vertices_3d[0], vertices_3d[2], vertices_3d[3])
    draw_line(vertices_3d[1], vertices_3d[2], vertices_3d[3])

    for vertex in vertices_3d:
        draw_point(vertex, 'red')

def draw_cdsimplex_2d(cdsimplex):
    """Draw cd simplex using centre, radius and vertices"""
    matrix_to_2d = [[1/math.sqrt(2), 1/math.sqrt(6)],
                    [-1/math.sqrt(2), 1/math.sqrt(6)],
                    [0, -2/math.sqrt(6)]]

    np_centre_3d = np.matmul(cdsimplex.centre, matrix_to_2d)
    centre_3d = [np_centre_3d[0], np_centre_3d[1], 0.0]

    draw_point(centre_3d, 'blue')

    vertices = cdsimplex_to_vertices(cdsimplex)

    np_vertices_3d = np.matmul(vertices, matrix_to_2d)
    vertices_3d = []
    for vertex in np_vertices_3d:
        vertex_3d = []
        vertex_3d.append(vertex[0])
        vertex_3d.append(vertex[1])
        vertex_3d.append(0.0)
        vertices_3d.append(vertex_3d)

    for vertex in vertices_3d:
        draw_point(vertex, 'red')

    vtx = [vertices_3d[0], vertices_3d[1], vertices_3d[2], vertices_3d[0]]
    tri = a3.art3d.Line3DCollection([vtx])
    tri.set_edgecolor('k')
    GRAPHIC.add_collection3d(tri)

# Convert dimension

def to_3d(simplex):
    """4 dimension simplex to 3 dimension representation"""
    matrix_to_3d = [[1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [-1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [0, -2/math.sqrt(6), 1/math.sqrt(12)],
                    [0, 0, -3/math.sqrt(12)]]

    if len(simplex.vertices) != 4:
        print 'Error: Vertices is not a 4 dimension simplex'

    return np.matmul(simplex.vertices, matrix_to_3d)

# Dim 2 convert and draw

def draw_2d(simplex):
    """3 dimension simplex to 2 dimension representation"""
    matrix_to_2d = [[1/math.sqrt(2), 1/math.sqrt(6)],
                    [-1/math.sqrt(2), 1/math.sqrt(6)],
                    [0, -2/math.sqrt(6)]]

    if len(simplex.vertices) != 3:
        print 'Error: Vertices is not a 3 dimension simplex'

    vertices = np.matmul(simplex.vertices, matrix_to_2d)

    vertices_3d = []
    for vertex in vertices:
        vertex_3d = []
        vertex_3d.append(vertex[0])
        vertex_3d.append(vertex[1])
        vertex_3d.append(0.0)
        vertices_3d.append(vertex_3d)

    vtx = [vertices_3d[0], vertices_3d[1], vertices_3d[2], vertices_3d[0]]
    tri = a3.art3d.Line3DCollection([vtx])
    tri.set_edgecolor('k')
    GRAPHIC.add_collection3d(tri)

def draw_2d_puntos(simplex):
    """3 dimension simplex to 2 dimension representation"""
    matrix_to_2d = [[1/math.sqrt(2), 1/math.sqrt(6)],
                    [-1/math.sqrt(2), 1/math.sqrt(6)],
                    [0, -2/math.sqrt(6)]]

    if len(simplex.vertices) != 3:
        print 'Error: Vertices is not a 3 dimension simplex'

    vertices = np.matmul(simplex.vertices, matrix_to_2d)

    vertices_3d = []
    for vertex in vertices:
        vertex_3d = []
        vertex_3d.append(vertex[0])
        vertex_3d.append(vertex[1])
        vertex_3d.append(0.0)
        vertices_3d.append(vertex_3d)

        draw_point(vertex_3d, 'g')

    vtx = [vertices_3d[0], vertices_3d[1], vertices_3d[2], vertices_3d[0]]
    tri = a3.art3d.Line3DCollection([vtx])
    tri.set_edgecolor('k')
    GRAPHIC.add_collection3d(tri)

def show_draw():
    """Show draw"""
    GRAPHIC.view_init(elev=156., azim=100)
    pl.axis('off')
    pl.show()

# Mains for testing

def mainleb3d():
    """Main"""
    global DIM
    DIM = 4
    working_list = []

    initial_unit_simplex_vertices = [[1, 0, 0, 0],
                                     [0, 1, 0, 0],
                                     [0, 0, 1, 0],
                                     [0, 0, 0, 1]]

    simplex_inicial = Simplex(initial_unit_simplex_vertices)
    working_list.append(simplex_inicial)

    vertices = to_3d(simplex_inicial)
    for vertex in vertices:
        draw_point(vertex, 'g')

    for simplex in working_list:
        draw_simplex(simplex)
        (vertices1, vertices2) = simplex.leb()

        if vertices1 == 0 and vertices2 == 0:
            continue

        working_list.append(Simplex(vertices1))
        working_list.append(Simplex(vertices2))

    show_draw()

def mainleb2d():
    """Main"""
    global DIM
    DIM = 3
    working_list = []

    initial_unit_simplex_vertices = [[1, 0, 0],
                                     [0, 1, 0],
                                     [0, 0, 1]]

    simplex_inicial = Simplex(initial_unit_simplex_vertices)
    working_list.append(simplex_inicial)

    draw_2d_puntos(simplex_inicial)

    for simplex in working_list:
        draw_2d(simplex)
        (vertices1, vertices2) = simplex.leb()

        if vertices1 == 0 and vertices2 == 0:
            continue

        working_list.append(Simplex(vertices1))
        working_list.append(Simplex(vertices2))

    show_draw()

def dist_points(point1, point2):
    """Distance between two vertices"""
    sum = 0.0

    for i in xrange(DIM):
        sum = sum + math.pow(point2[i] - point1[i], 2)

    return math.sqrt(sum)

def initial_radius():
    """initial_radius"""
    vertices = []
    for i in xrange(DIM):
        coor = []
        for j in xrange(DIM):
            if i == j:
                coor.append(1.0)
            else:
                coor.append(0.0)
        vertices.append(coor)

    centre = []

    for i in xrange(DIM):
        aux_centre = 0.0
        for j in xrange(DIM):
            aux_centre += vertices[j][i]
        aux_centre /= DIM
        centre.append(aux_centre)

    return dist_points(centre, vertices[0])

def vertices_to_cdsimplex(vertices):
    """VtoCDS"""
    centre = []

    for i in xrange(DIM):
        aux_centre = 0.0
        for j in xrange(DIM):
            aux_centre += vertices[j][i]
        aux_centre /= DIM
        centre.append(aux_centre)

    size = dist_points(vertices[0], vertices[1])
    radius = dist_points(centre, vertices[0]) / initial_radius()

    return CDSimplex(centre, radius, size, True)

def main2usc3d():
    """Main"""
    global DIM
    DIM = 4
    initial_unit_simplex_vertices = [[1, 0, 0, 0],
                                     [0, 1, 0, 0],
                                     [0, 0, 1, 0],
                                     [0, 0, 0, 1]]
    simplex_cdinicial = vertices_to_cdsimplex(initial_unit_simplex_vertices)
    simplex_cdinicial.print_cdsimplex()
    draw_cdsimplex_3d(simplex_cdinicial)

    beta = (DIM - 1.0) / DIM

    cdsimplices = simplex_cdinicial.divide_2usc(beta)

    for cdsimplex in cdsimplices:
        draw_cdsimplex_3d(cdsimplex)

    show_draw()

def main2usc2d():
    """Main"""
    global DIM
    DIM = 3
    initial_unit_simplex_vertices = [[1, 0, 0],
                                     [0, 1, 0],
                                     [0, 0, 1]]
    simplex_cdinicial = vertices_to_cdsimplex(initial_unit_simplex_vertices)
    draw_cdsimplex_2d(simplex_cdinicial)

    beta = (DIM - 1.0) / DIM

    cdsimplices = simplex_cdinicial.divide_2usc(beta)

    for cdsimplex in cdsimplices:
        draw_cdsimplex_2d(cdsimplex)

    show_draw()

def main2musc3d():
    """Main"""
    global DIM
    DIM = 4
    initial_unit_simplex_vertices = [[1, 0, 0, 0],
                                     [0, 1, 0, 0],
                                     [0, 0, 1, 0],
                                     [0, 0, 0, 1]]
    simplex_cdinicial = vertices_to_cdsimplex(initial_unit_simplex_vertices)
    draw_cdsimplex_3d(simplex_cdinicial)

    rho = (DIM - 2.0) / (DIM - 1.0)

    cdsimplices = simplex_cdinicial.divide_2musc(rho)

    for cdsimplex in cdsimplices:
        draw_cdsimplex_3d(cdsimplex)

    show_draw()

if __name__ == '__main__':
    main2musc3d()
