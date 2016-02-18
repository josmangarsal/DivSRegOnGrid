# DivSRegOnGrid
Regular by regular simplex division

DivSRegOnGrid-1.0:

Genera el refinamiento de un simlex unidad (x_i=1.0) mediante diferentes métodos de division.
Ver parámetro -Div.

Si se usa -ep el criterio de parada es w(S) <= ep * w(S_1).
-ep es un porcentaje de reducción de S_1.

Se puede usar -a \in (0,1), lo que adapta los factores de reducción a un grid.

El parámetro -gep \in (0,1), genera un grid de tamaño gep. El algoritmo chequea si todos los puntos del grid se ha visitado en el particionamiento.

Si no se usan parámetros aparecerá la siguiente ayuda:
Options are:
	[ --help To show this help]
	[ -d            <int>: dimension, default=3]
	[ -fr          <real>: fraction. Only in -Div 1, default: (d-1)/d
	  -gep         <real>: Grid points generation size 
	  -ep          <real>: Min size(simplex)=% Initial Size 
	  -a           <real>: Min size(simplex)=% Initial Size 
	[ -ns                : No store final simplices]
	[ -out               : Generates statistic output file]
	[ -Div         <int> : Division method]
	                     :  1 -> 2USC
	                     :  2 -> 2USC(n/n+1)+2USC((n-1)
	                     :  3 -> 2NUSC
	                     :  4 -> 3USC
	                     : [5]-> 2USC Grid
	                     :  6 -> 2NUSC Grid
	[ -w            <int>: Graphic window width (400)]
	[ -tcl          <int>: Graphical output with"| escala.tcl"]
	                       1: normal draw, 2:press key]

Ejemplo de ejecución:
DivSRegOnGrid-1.0 -gep 0.25 -a 0.25 -tcl 1 -w 800 | zoomSCD.tcl
