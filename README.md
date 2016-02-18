# DivSRegOnGrid
Regular by regular simplex division

DivSRegOnGrid-1.4:

Generado a partir de DivSRegOnGrid-1.3 y DivSRegOnGrid-1.2.

- La linea EXIT: muestra todos los parametros del programa y los resultados

- Solucionado problema con 2NUSC:

	* En la versión 1.3 no se estaba calculando el reduction factor cuando NewNGrid=1.

	En la versión 1.2:
		if (EQ(Fraction*pCDS_O->L, GridSize))
		    NGNSE=1;
		else
		    ... // Calculo de NewNGrid

	NewFraction=((REAL)NGNSE*GridSize)/pCDS_O->L;

	En la versión 1.3:
		if (EQ(RedFact*pCDS_O->L, GridSize))
		    NewNGrid=1;
		else
		    NextRF(pCDS_O, GridSize, RedFact, &NewNGrid, &NewRedFact);

	La diferencia está en la linea donde se obtiene NewRedFact. En la versión 1.3 este valor se obtiene dentro del método NextRF, por lo que cuando se da la condición "if(EQ...)" no se estaba calculando.
