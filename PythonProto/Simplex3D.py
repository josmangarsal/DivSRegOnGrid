"""2D and 3D simplex division"""

import sys, getopt
import math
import numpy as np
import mpl_toolkits.mplot3d as a3
import matplotlib.colors as colors
import pylab as pl
import scipy as sp

# GLOBAL VARIABLES

EPS = 0.9
DIM = 3
DIV = '2usc'
DRAW = True
PAUSE = 0

# GLOBAL METHODS

def dist_points(point1, point2):
    """Distance between two vertices"""
    sum = 0.0

    for i in xrange(DIM):
        sum = sum + math.pow(point2[i] - point1[i], 2)

    return math.sqrt(sum)

# CLASSES

class Simplex(object):
    """Simplex"""

    def __init__(self, vertices):
        self.vertices = vertices
        self.distances = []
        self.size()

    def size(self):
        """Size of each edge of the simplex, sort and LE calculation"""
        for i in xrange(DIM):
            for j in range(i+1, DIM):
                self.distances.append(
                    (dist_points(self.vertices[i], self.vertices[j]), (i, j)))

        self.distances.sort(key=lambda x: (-x[0], x[1][0], x[1][0]))
        self.longest_edge = self.distances[0]

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

    def __init__(self, centre, radius, size, up_orientation):
        self.centre = centre
        self.radius = radius
        self.size = size
        self.up_orientation = up_orientation # True is normal orientation

    def divide_2usc(self, red_factor):
        """2USC regular division"""
        if self.size <= EPS:
            return None

        new_radius = self.radius * red_factor
        new_size = self.size * red_factor

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

            new_simplices.append(CDSimplex(new_center, new_radius, new_size, True))

        return new_simplices

    def divide_2musc(self, red_factor):
        """2MUSC regular division"""
        if self.size <= EPS:
            return None

        new_2nusc_simplex = None

        if red_factor < ((DIM - 1.0) / DIM):
            up_down_ratio = DIM * (1.0 - red_factor) - 1.0

            new_radius = self.radius * up_down_ratio
            new_size = self.size * up_down_ratio

            new_centre = []
            for i in self.centre:
                new_centre.append(i)

            new_2nusc_simplex = CDSimplex(new_centre, new_radius, new_size, False)

        new_2usc_simplices = self.divide_2usc(red_factor)

        if new_2usc_simplices is None:
            return None

        if new_2nusc_simplex is not None:
            new_2usc_simplices.append(new_2nusc_simplex)

        return new_2usc_simplices

    def musc_centers(self):
        """Calculation of Ct1..tn"""
        centers = []

        # TODO Recursive to any DIM
        for i in range(0, DIM+1):
            for j in range(0, DIM+1):
                for k in range(0, DIM+1):
                    if i + j + k == DIM:
                        centers.append([i, j, k])

        return centers

    def divide_musc(self, simplices_per_edge, red_factor):
        """MUSC regular division"""
        if self.size <= EPS:
            return None

        matrix_to_vertices = [] # D (d1..dn)
        for i in xrange(DIM):
            coor = []
            for j in xrange(DIM):
                if i == j:
                    coor.append((DIM - 1.0) / DIM)
                else:
                    coor.append(-1.0 / DIM)
            matrix_to_vertices.append(coor)

        t_ns = self.musc_centers() # t1..tn

        #number_new_simplices = math.factorial(simplices_per_edge + DIM - 2) / (math.factorial(simplices_per_edge - 1) * math.factorial(DIM - 3))

        new_radius = self.radius * red_factor
        new_size = self.size * red_factor

        new_simplices = []
        for t_n in t_ns:
            new_center = []

            for i in range(DIM):
                sumita = 0
                j = 0
                for elem in t_n:
                    sumita += elem * matrix_to_vertices[j][i]
                    j += 1

                center = self.centre[i] + (((1 - red_factor) * self.radius) / (simplices_per_edge - 1)) * sumita
                new_center.append(center)

            new_simplices.append(CDSimplex(new_center, new_radius, new_size, True))

        return new_simplices

    def print_cdsimplex(self):
        """Print simplex"""
        print 'Dim=', DIM
        print 'Centre=', self.centre
        print 'Radius=', self.radius
        print 'Size=', self.size
        print 'Up=', self.up_orientation

    def to_vertices(self):
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
                if self.up_orientation is True:
                    coor.append(self.centre[j] + self.radius * matrix_to_vertices[i][j])
                else:
                    coor.append(self.centre[j] - self.radius * matrix_to_vertices[i][j])
            vertices.append(coor)

        return vertices

# DRAWING

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

def draw_cdsimplex_3d_init(cdsimplex):
    """Draw cd simplex using centre, radius and vertices"""
    matrix_to_3d = [[1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [-1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [0, -2/math.sqrt(6), 1/math.sqrt(12)],
                    [0, 0, -3/math.sqrt(12)]]

    centre_3d = np.matmul(cdsimplex.centre, matrix_to_3d)
    draw_point(centre_3d, 'blue')

    vertices = cdsimplex.to_vertices()

    vertices_3d = np.matmul(vertices, matrix_to_3d)

    draw_line(vertices_3d[0], vertices_3d[1], vertices_3d[2])
    draw_line(vertices_3d[0], vertices_3d[1], vertices_3d[3])
    draw_line(vertices_3d[0], vertices_3d[2], vertices_3d[3])
    draw_line(vertices_3d[1], vertices_3d[2], vertices_3d[3])

    for vertex in vertices_3d:
        draw_point(vertex, 'red')

def draw_cdsimplex_3d(cdsimplex):
    """Draw cd simplex using centre, radius and vertices"""
    matrix_to_3d = [[1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [-1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
                    [0, -2/math.sqrt(6), 1/math.sqrt(12)],
                    [0, 0, -3/math.sqrt(12)]]

    vertices = cdsimplex.to_vertices()

    vertices_3d = np.matmul(vertices, matrix_to_3d)

    draw_line(vertices_3d[0], vertices_3d[1], vertices_3d[2])
    draw_line(vertices_3d[0], vertices_3d[1], vertices_3d[3])
    draw_line(vertices_3d[0], vertices_3d[2], vertices_3d[3])
    draw_line(vertices_3d[1], vertices_3d[2], vertices_3d[3])

def draw_cdsimplex_2d_init(cdsimplex):
    """Draw cd simplex using centre, radius and vertices"""
    matrix_to_2d = [[1/math.sqrt(2), 1/math.sqrt(6)],
                    [-1/math.sqrt(2), 1/math.sqrt(6)],
                    [0, -2/math.sqrt(6)]]

    np_centre_3d = np.matmul(cdsimplex.centre, matrix_to_2d)
    centre_3d = [np_centre_3d[0], np_centre_3d[1], 0.0]

    draw_point(centre_3d, 'blue')

    vertices = cdsimplex.to_vertices()

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

def draw_cdsimplex_2d(cdsimplex):
    """Draw cd simplex using centre, radius and vertices"""
    matrix_to_2d = [[1/math.sqrt(2), 1/math.sqrt(6)],
                    [-1/math.sqrt(2), 1/math.sqrt(6)],
                    [0, -2/math.sqrt(6)]]

    vertices = cdsimplex.to_vertices()

    np_vertices_3d = np.matmul(vertices, matrix_to_2d)
    vertices_3d = []
    for vertex in np_vertices_3d:
        vertex_3d = []
        vertex_3d.append(vertex[0])
        vertex_3d.append(vertex[1])
        vertex_3d.append(0.0)
        vertices_3d.append(vertex_3d)

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

# Mains for testing

def main_leb():
    """Main LEB"""
    global EPS

    working_list = []

    initial_unit_simplex_vertices = []
    for i in xrange(DIM):
        coor = []
        for j in xrange(DIM):
            if i == j:
                coor.append(1.0)
            else:
                coor.append(0.0)
        initial_unit_simplex_vertices.append(coor)

    simplex_inicial = Simplex(initial_unit_simplex_vertices)
    working_list.append(simplex_inicial)

    EPS *= simplex_inicial.size

    if DRAW:
        if DIM == 3:
            draw_2d_puntos(simplex_inicial)
        elif DIM == 4:
            vertices = to_3d(simplex_inicial)
            for vertex in vertices:
                draw_point(vertex, 'g')

        if PAUSE != 0.0:
            pl.pause(PAUSE)

    for simplex in working_list:
        if DRAW:
            if DIM == 3:
                draw_2d(simplex)
            elif DIM == 4:
                draw_simplex(simplex)

            if PAUSE != 0.0:
                pl.pause(PAUSE)

        (vertices1, vertices2) = simplex.leb()

        if vertices1 == 0 and vertices2 == 0:
            continue

        working_list.append(Simplex(vertices1))
        working_list.append(Simplex(vertices2))

# RbR general methods

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

def main_rbr():
    """Main RbR"""
    global EPS

    working_list = []
    initial_unit_simplex_vertices = []

    for i in xrange(DIM):
        coor = []
        for j in xrange(DIM):
            if i == j:
                coor.append(1.0)
            else:
                coor.append(0.0)
        initial_unit_simplex_vertices.append(coor)

    simplex_cdinicial = vertices_to_cdsimplex(initial_unit_simplex_vertices)

    EPS *= simplex_cdinicial.size

    if DRAW:
        if DIM == 4:
            draw_cdsimplex_3d_init(simplex_cdinicial)
        elif DIM == 3:
            draw_cdsimplex_2d_init(simplex_cdinicial)

        if PAUSE != 0.0:
            pl.pause(PAUSE)

    beta = 0.0
    rho = 0.0

    if DIV == '2usc':
        beta = (DIM - 1.0) / DIM
    elif DIV == '2musc':
        rho = (DIM - 2.0) / (DIM - 1.0)

    working_list.append(simplex_cdinicial)

    for cdsimplex in working_list:
        if DRAW:
            if DIM == 4:
                draw_cdsimplex_3d(cdsimplex)
            elif DIM == 3:
                draw_cdsimplex_2d(cdsimplex)

            if PAUSE != 0.0:
                pl.pause(PAUSE)

        cdsimplices = None
        if DIV == '2usc':
            cdsimplices = cdsimplex.divide_2usc(beta)
        elif DIV == '2musc':
            cdsimplices = cdsimplex.divide_2musc(rho)

        if cdsimplices is not None:
            for new_cdsimplex in cdsimplices:
                working_list.append(new_cdsimplex)

def main_musc():
    """Main mUSC"""
    global EPS

    working_list = []
    initial_unit_simplex_vertices = []

    for i in xrange(DIM):
        coor = []
        for j in xrange(DIM):
            if i == j:
                coor.append(1.0)
            else:
                coor.append(0.0)
        initial_unit_simplex_vertices.append(coor)

    simplex_cdinicial = vertices_to_cdsimplex(initial_unit_simplex_vertices)

    EPS *= simplex_cdinicial.size

    if DRAW:
        if DIM == 4:
            draw_cdsimplex_3d_init(simplex_cdinicial)
        elif DIM == 3:
            draw_cdsimplex_2d_init(simplex_cdinicial)

        if PAUSE != 0.0:
            pl.pause(PAUSE)

    m_simplices_per_edge = 4 # Simplices per edge
    reduction_factor = (DIM - 1.0) / (m_simplices_per_edge + DIM - 2.0)

    working_list.append(simplex_cdinicial)

    for cdsimplex in working_list:
        if DRAW:
            if DIM == 4:
                draw_cdsimplex_3d(cdsimplex)
            elif DIM == 3:
                draw_cdsimplex_2d(cdsimplex)

            if PAUSE != 0.0:
                pl.pause(PAUSE)

        cdsimplex.print_cdsimplex()

        cdsimplices = cdsimplex.divide_musc(m_simplices_per_edge, reduction_factor)

        if cdsimplices is not None:
            for new_cdsimplex in cdsimplices:
                working_list.append(new_cdsimplex)

def menu(argument):
    """Menu"""
    switcher = {
        ('leb', 3): main_leb,
        ('leb', 4): main_leb,
        ('2usc', 3): main_rbr,
        ('2usc', 4): main_rbr,
        ('2musc', 3): main_rbr,
        ('2musc', 4): main_rbr,
        ('musc', 3): main_musc,
    }

    func = switcher.get(argument, lambda: "nothing")

    return func()

def help_commands():
    """Command help"""
    print 'Draw2d.py -D <division_method> -d <dimension> -e <epsilon> -P <time>'
    print 'Division methods: leb, 2usc or 2musc'
    print 'Time: 0 to show final result, 0.05 to show step by step or -1 to not to draw'
    sys.exit(2)

def main(argv):
    """Main"""
    global DIM
    global EPS
    global DIV
    global PAUSE
    global DRAW

    try:
        opts, args = getopt.getopt(argv, "D:d:e:P:")
    except getopt.GetoptError:
        help_commands()
    print opts
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            help_commands()
        elif opt == "-d":
            DIM = int(arg)
        elif opt == "-e":
            EPS = float(arg)
        elif opt == "-D":
            if arg in ('leb', '2usc', '2musc', 'musc'):
                DIV = arg
            else:
                print 'Division methods: leb, 2usc or 2musc'
                sys.exit(1)
        elif opt == "-P":
            PAUSE = float(arg)
            if PAUSE == -1:
                DRAW = False


    if DRAW:
        if DIM == 4:
            GRAPHIC.view_init(elev=156., azim=100)
        elif DIM == 3:
            GRAPHIC.view_init(elev=90., azim=90)
        pl.axis('off')
        if PAUSE != 0.0:
            pl.ion()

    menu((DIV, DIM))

    if DRAW:
        if PAUSE != 0.0:
            pl.ioff()
        pl.show()

if __name__ == '__main__':
    main(sys.argv[1:])
