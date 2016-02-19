/*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "getmem.h"
#include "gengridpoints.h"
#include "Queue.h"

int teller;

/*---------------------------------------------------------------------------*/
double factorial(int n) {
	double fact = 1;
	int i;

	for (i = 1; i <= n; i++)
		fact = fact * i;

	return fact;
}

/*---------------------------------------------------------------------------*/
int combinatoria(int a, int b) {
	return factorial(a) / (factorial(b) * factorial(a - b));
}

/*---------------------------------------------------------------------------*/
int numberGridPoints(int m, int n) {
	int k;
	int sum = 0;
	for (k = 1; k <= n - 1; k++)
		sum += combinatoria(m, k) * combinatoria(n - 2, k - 1);

	return sum;
}

/*---------------------------------------------------------------------------*/
void gridpoints(PQueue gridPoints, double* xTemp, int d, int n, int div, int suma, int resta) {
	int i;

	if (d == n) {
		// He llegado al final, calculo y añado los nuevos grid points
		xTemp[0] = div - suma;

		double* x = (double*) GetMem((size_t) n, (size_t) sizeof(double), "GridPoints:Point");
		for (i = 0; i < n; i++)
			x[i] = xTemp[i] / div; // El paso de dividir entre 'm-1' lo pongo aqui

		Push(gridPoints, x);
		teller++;
		return;
	}

	for (i = 0; i <= resta; i++) {
		xTemp[d] = i;
		gridpoints(gridPoints, xTemp, d + 1, n, div, suma + i, resta - i);
	}
}

/*---------------------------------------------------------------------------*/
PQueue GenGrid(int m, int n) {
	teller = 1;

	int div = m - 1;

	int i;
	int i2;

	PQueue gridPoints = InitQueue();

	double* initialGrid = (double*) GetMem((size_t) n, (size_t) sizeof(double), "GridPoints:InitialPoint");
	for (i = 0; i < n; i++)
		initialGrid[i] = 0;
	initialGrid[1] = div / div; // El paso de dividir entre 'm-1' lo pongo aqui
	Push(gridPoints, initialGrid);

	double* xTemp = (double*) GetMem((size_t) n, (size_t) sizeof(double), "GridPoints:AuxPoint");
	for (i = 0; i < n; i++)
		xTemp[i] = 0;

	// Para i2 lo hago aqui porque se diferente el bucle
	for (i2 = 0; i2 <= m - 2; i2++) {
		xTemp[1] = i2;
		gridpoints(gridPoints, xTemp, 2, n, div, i2, div - i2);
	}

	free((void*) xTemp);

	return gridPoints;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
