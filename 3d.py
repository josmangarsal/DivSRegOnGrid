import matplotlib.pyplot as plt
import numpy as np
import mpl_toolkits.mplot3d as a3
import matplotlib.colors as colors
import pylab as pl
import scipy as sp
import math

# draw cube
def drawCube(self):
    r = [0, 1]
    for s, e in combinations(np.array(list(product(r, r, r))), 2):
        if np.sum(np.abs(s-e)) == r[1]-r[0]:
            ax.plot3D(*zip(s, e), color="black")

# draw a point
def drawPoint(v):
    ax.scatter(v[0], v[1], v[2], color="g", s=100)

def drawPlain(v1, v2, v3):
    vtx = [v1,v2,v3]
    tri = a3.art3d.Poly3DCollection([vtx])
    tri.set_color(colors.rgb2hex(sp.rand(3)))
    tri.set_edgecolor('k')
    ax.add_collection3d(tri)

def drawLine(v1, v2, v3):
    vtx = [v1,v2,v3,v1]
    tri = a3.art3d.Line3DCollection([vtx])
    tri.set_edgecolor('k')
    ax.add_collection3d(tri)

ax = a3.Axes3D(pl.figure())

uno = math.sqrt(0.75)

v1 = [0, 0, 0]
v2 = [1, 0, 0]
v3 = [0.5, uno, 0]
v4 = [0.5, uno/2, uno]

drawLine(v1, v2, v3) # Base
drawLine(v1, v2, v4)
drawLine(v2, v3, v4)
drawLine(v1, v3, v4)

drawPoint(v1)
drawPoint(v2)
drawPoint(v3)
drawPoint(v4)

v1 = [0, 0, 0]
v2 = [0.5, 0, 0]
v3 = [0.5/2, uno/2, 0]
v4 = [0.5/2, uno/2/2, uno/2]

drawPlain(v1, v2, v3)
drawPlain(v1, v2, v4)
drawPlain(v2, v3, v4)
drawPlain(v1, v3, v4)

v1 = [0.5, 0, 0]
v2 = [1, 0, 0]
v3 = [0.5+0.5/2, uno/2, 0]
v4 = [0.5+0.5/2, uno/2/2, uno/2]

drawPlain(v1, v2, v3)
drawPlain(v1, v2, v4)
drawPlain(v2, v3, v4)
drawPlain(v1, v3, v4)

v1 = [0.5/2, uno/2, 0]
v2 = [0.5+0.5/2, uno/2, 0]
v3 = [0.5, uno, 0]
v4 = [0.5, uno/2+uno/2/2, uno/2]

drawPlain(v1, v2, v3)
drawPlain(v1, v2, v4)
drawPlain(v2, v3, v4)
drawPlain(v1, v3, v4)

v1 = [0.5/2, uno/2/2, uno/2]
v2 = [0.5+0.5/2, uno/2/2, uno/2]
v3 = [0.5, uno/2+uno/2/2, uno/2]
v4 = [0.5, uno/2, uno]

drawPlain(v1, v2, v3)
drawPlain(v1, v2, v4)
drawPlain(v2, v3, v4)
drawPlain(v1, v3, v4)

pl.show()