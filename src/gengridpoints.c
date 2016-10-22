/*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "getmem.h"
#include "gengridpoints.h"
#include "Queue.h"

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
void gridpoints(PQueue gridPoints, double* P, int d, int G, int i, int c) {
	int j;

	if (i == d) {
		P[0] = G - c;

		double* Q = (double*) GetMem((size_t) d, (size_t) sizeof(double), "GridPoints:Point");
		for (j = 0; j < d; j++)
			Q[j] = P[j] / G;

		Push(gridPoints, Q);
		return;
	}

	for (j = 0; j <= G - c; j++) {
		P[i] = j;
		gridpoints(gridPoints, P, d, G, i + 1, c + j);
	}
}

/*---------------------------------------------------------------------------*/
PQueue GenGrid(double e, int d) {
	int G = ceil(1/e);

	int i;
	int j;

	PQueue gridPoints = InitQueue();

	double* initialGrid = (double*) GetMem((size_t) d, (size_t) sizeof(double), "GridPoints:InitialPoint");
	for (i = 0; i < d; i++)
		initialGrid[i] = 0.0;
	initialGrid[1] = 1.0;
	Push(gridPoints, initialGrid);

	double* P = (double*) GetMem((size_t) d, (size_t) sizeof(double), "GridPoints:AuxPoint");
	for (i = 0; i < d; i++)
		P[i] = 0.0;

	// Para i2 lo hago aqui porque se diferente el bucle
	for (j = 0; j <= G - 1; j++) {
		P[1] = j;
		gridpoints(gridPoints, P, d, G, 2, j);
	}

	free((void*) P);

	return gridPoints;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
