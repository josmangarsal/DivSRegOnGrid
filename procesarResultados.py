import sys, os

if len(sys.argv) != 2:
    print "Se esperaba la ruta de los resultados";
    exit(0)

columnasMostrar = ['N.S.', 'N.V.'] #['Simplices', 'Vertices', 'Visited', 'GridSize']

if len(columnasMostrar) == 0:
    print 'Se debe mostrar como minimo 1 columna'
    sys.exit(0)

listD = []
listG = []

# Generar tabla
for filename in os.listdir(sys.argv[1]):
    ext = filename.split('.')
    if(ext[1] == 'out'): # Procesar solo ficheros de resultados
        valores = ext[0].split('-')
        d = int(valores[0].replace('d', ''))
        listD.append(d)

        g = int(valores[1].replace('g', ''))
        listG.append(g)

# Quitar duplicados
listD = list(set(listD))
listG = list(set(listG))

# Por cada d 4 columnas (Simplices, Vertices, Visited, GridSize) mas una columnna para la g y una fila por g mas al cabecera (G | ... | ...) y mas una fila por la dimension
filas = len(listG) + 1 + 1
columnas = 1 + len(listD) * 4

# Cabecera Latex de la tabla

cabecera = "r|" + ''.join([''.join(['r' for k in xrange(len(columnasMostrar))]) + '|' for k in xrange(len(listD))])

resultados = [['-' for k in xrange(columnas)] for j in xrange(filas)]

# Primera fila, dimension
resultados[0][0] = ' '
cont = 0
i = 0
for j in xrange(columnas):
    if j % 4 == 1:
        resultados[i][j] = '\multicolumn{' + str(len(columnasMostrar)) + '}{c|}{$d$=' + str(listD[cont]) + '}'
        cont += 1

# Segunda fila, cabeceras
resultados[1][0] = '$G$'
cont = 0
i = 1
for j in xrange(columnas):
    if j % 4 == 1:
        resultados[i][j] = 'N.S.'
    if j % 4 == 2:
        resultados[i][j] = 'N.V.'
    if j % 4 == 3:
        resultados[i][j] = 'N.G.V.'
    if j != 0 and j % 4 == 0:
        resultados[i][j] = 'N.G.'

j = 0
for i in range(2, filas):
    resultados[i][j] = listG[i-2]

# Obtener resultados de los ficheros
for filename in os.listdir(sys.argv[1]):
    ext = filename.split('.')
    if(ext[1] == 'out'): # Procesar solo ficheros de resultados
        file = open(filename, 'r')

        # Valores que Busco
        D = 0
        G = 0
        Grid = 0
        Visited = 0
        Vertices = 0
        Simplices = 0

        while True:
            line = file.readline()

            if not line:
                break

            datos = line.split()

            if(len(datos) > 0): # EXIT: -d 3 -g 14 -ep 0.071429 -Div 1 -Grid 120 -Formula 120 -Visited 120 -Vertices 120 -Simplices 263
                if datos[0] == 'EXIT:':
                    i = 0
                    while i < len(datos):
                        if(datos[i] == '-d'):
                            D = int(datos[i+1])
                        if(datos[i] == '-g'):
                            G = int(datos[i+1])
                        if(datos[i] == '-Grid'):
                            Grid = int(datos[i+1])
                        if(datos[i] == '-Visited'):
                            Visited = int(datos[i+1])
                        if(datos[i] == '-Vertices'):
                            Vertices = int(datos[i+1])
                        if(datos[i] == '-Simplices'):
                            Simplices = int(datos[i+1])
                        i = i + 1

                    break

        if D != 0:
            # Interpretar resultados

            # Columna a partir de la dimension
            columnaBaseActual = 0
            cont = 1
            for i in listD:
                if D == i:
                    columnaBaseActual = cont

                    if columnaBaseActual == 1:
                        columnaBaseActual = 1
                    if columnaBaseActual == 2:
                        columnaBaseActual = 5
                    if columnaBaseActual == 3:
                        columnaBaseActual = 9

                    break
                cont += 1

            # Fila a partir del g
            filaActual = 0
            cont = 2
            for i in listG:
                if G == i:
                    filaActual = cont

                    break
                cont += 1

            resultados[filaActual][columnaBaseActual] = "{:,d}".format(Simplices)
            if Visited == Grid:
                resultados[filaActual][columnaBaseActual+1] = '\\ccg ' + str("{:,d}".format(Vertices))
            else:
                resultados[filaActual][columnaBaseActual+1] = "{:,d}".format(Vertices)
            resultados[filaActual][columnaBaseActual+2] = "{:,d}".format(Visited)
            resultados[filaActual][columnaBaseActual+3] = "{:,d}".format(Grid)

        file.close()

# Salida por pantalla

print '\\begin{tabular}{' + cabecera + '}'

# Muestro solo las columnas que indica columnasMostrar
i = 1
for j in range(1, columnas):
    igual = 0

    for r in columnasMostrar:
        if resultados[i][j] == r:
            igual = 1
            break

    if igual == 0:
        for f in range(1, filas):
            resultados[f][j] = '.'

i = 0
print str(resultados[i][0]),
for j in range(1, columnas):
    if(resultados[i][j] != '-'):
        print '& ' + str(resultados[i][j]),
print '\\\\'

i = 1
print str(resultados[i][0]),
for j in range(1, columnas):
    if (resultados[i][j] != '.'):
        if (resultados[i][j] != '-'):
            print '& ' + str(resultados[i][j]),
        if (resultados[i][j] == '-'):
            print '& ',
print '\\\\'

print '\\hline'

for i in range(2, filas):
    print str(resultados[i][0]),
    for j in range(1, columnas):
        if (resultados[i][j] != '.'):
            if (resultados[i][j] != '-'):
                print '& ' + str(resultados[i][j]),
            if (resultados[i][j] == '-'):
                print '& ',
    print '\\\\'

print '\\end{tabular}'

sys.exit(0)
