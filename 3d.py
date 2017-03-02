import numpy as np
import mpl_toolkits.mplot3d as a3
import matplotlib.colors as colors
import pylab as pl
import scipy as sp
import math

def distVertices(v1, v2):
    ...

class Simplex:
    dimension = 0
    vertices = []
    size = 0.0

    def __init__(self, dimension, vertices):
            self.dimension = dimension
            self.vertices = vertices
            self.size()

    def size(self):
        for i in xrange(self.dimension):
            for j in xrange(self.dimension):
                ...

        self.size = 1.0

    def LEB(self, v1, v2):
        ...

        s1 = Simplex(self.dimension, v1)
        s2 = Simplex(self.dimension, v2)

        return (s1, s2)

    def printSimplex(self):
        print 'Dim=', self.dimension
        print 'Vertices=', self.vertices
        print 'Size=', self.size

ax = a3.Axes3D(pl.figure())

# draw cube
def drawCube(self):
    r = [0, 1]
    for s, e in combinations(np.array(list(product(r, r, r))), 2):
        if np.sum(np.abs(s-e)) == r[1]-r[0]:
            ax.plot3D(*zip(s, e), color="black")

# draw a point
def drawPoint(v):
    ax.scatter(v[0], v[1], v[2], color="g", s=100)

# draw a plain
def drawPlain(v1, v2, v3):
    vtx = [v1,v2,v3]
    tri = a3.art3d.Poly3DCollection([vtx])
    tri.set_color(colors.rgb2hex(sp.rand(3)))
    tri.set_edgecolor('k')
    ax.add_collection3d(tri)

# draw a plain with lines
def drawLine(v1, v2, v3):
    vtx = [v1,v2,v3,v1]
    tri = a3.art3d.Line3DCollection([vtx])
    tri.set_edgecolor('k')
    ax.add_collection3d(tri)

def drawSimplex(simplex3D):
    simplex3D[3] = simplex3D[3]*-1
    simplex3D = simplex3D+1

    for vertice in simplex3D:
        drawPoint(vertice)

    drawLine(simplex3D[0], simplex3D[1], simplex3D[2])
    drawLine(simplex3D[0], simplex3D[1], simplex3D[3])
    drawLine(simplex3D[0], simplex3D[2], simplex3D[3])
    drawLine(simplex3D[1], simplex3D[2], simplex3D[3])

def to3D(simplex):
    matrix3D = [[1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
            [-1/math.sqrt(2), 1/math.sqrt(6), 1/math.sqrt(12)],
            [0, -2/math.sqrt(6), 1/math.sqrt(12)],
            [0, 0, -3/math.sqrt(12)]]

    if len(simplex) != 4:
        print 'Error: Vertices is not a 4 dimension simplex'

    return np.matmul(simplex,matrix3D)

simplexInicial = [[1,0,0,0],
                [0,1,0,0],
                [0,0,1,0],
                [0,0,0,1]]

sInicial = Simplex(4, simplexInicial)

sInicial.printSimplex()

simplex3D = to3D(simplexInicial)
drawSimplex(simplex3D)

simplex1 = [[0.5,0.5,0,0],
            [0.5,0.5,0,0],
            [0,0,1,0],
            [0,0,0,1]]

simplex1 = to3D(simplex1)
drawSimplex(simplex1)

pl.show()
