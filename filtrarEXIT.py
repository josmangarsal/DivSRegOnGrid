import sys, os

if len(sys.argv) != 2:
    print "Se esperaba la ruta del fichero";
    exit(0)

# Abrir fichero de entrada
file = open(sys.argv[1], 'r')
while True:
    line = file.readline()

    if not line:
        break

    datos = line.split()

    if(len(datos) > 0): # EXIT: -d 3 -g 14 -ep 0.071429 -Div 1 -Grid 120 -Formula 120 -Visited 120 -Vertices 120 -Simplices 263
        if datos[0] == 'EXIT:':
            D = 0
            G = 0
            i = 0
            while i < len(datos):
                if(datos[i] == '-d'):
                    D = int(datos[i+1])
                if(datos[i] == '-g'):
                    G = int(datos[i+1])
                i += 1
            # Creo un fichero por cada linea EXIT
            with open("d" + str(D) + "-g" + str(G) + ".out", "w") as text_file: # d4-g26.out
                text_file.write(line)

sys.exit(0)
