# DivSRegOnGrid
Regular by regular simplex division

DivSRegOnGrid-1.3:

Generado a partir de DivSRegOnGrid-1.2

Genera el refinamiento de un simlex unidad (x_i=1.0) mediante diferentes métodos de division.
Ver parámetro -Div.

El parámetro -ep \in (0,1), genera un grid de tamaño GridSize. 
El algoritmo chequea si todos los puntos del grid se ha visitado en el particionamiento.

Internamente el GridSize que se obtine de -ep y la division se adapta a ese grid.

Se puede usar el parametro -g -> epsilon=1/g.

Hay salida gráfica para x1,x2,x3 para d>3

Se va ha añadir el cálculo de los factores de reducción en modo hacia atrás.


