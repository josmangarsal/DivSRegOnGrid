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
void gridpoints(PQueue gridPoints, int G, int d, int N, int m, double* x) {
	int k, j;
	double sum;

	for (k = 0; k < N; k++) {
		x[m] = k;

		if(m > 1) {
			gridpoints(gridPoints, G, d, N-k+1, m-1, x);
		} else {
			sum = 0.0;
			for(j = 1; j < d; j++)
				sum += x[j];
			x[0] = G - sum;

			double* gridPoint = (double*) GetMem((size_t) d, (size_t) sizeof(double), "GridPoints:Point");
			for (j = 0; j < d; j++)
				gridPoint[j] = x[j] / G;

			Push(gridPoints, gridPoint);
		}
	}

}

/*---------------------------------------------------------------------------*/
PQueue GenGrid(double e, int d) {
	int G = ceil(1/e);

	int i;

	PQueue gridPoints = InitQueue();

	double* x = (double*) GetMem((size_t) d, (size_t) sizeof(double), "GridPoints:AuxPoint");
	for (i = 0; i < d; i++)
		x[i] = 0.0;

	gridpoints(gridPoints, G, d, G, d-1, x);

	free((void*) x);

	return gridPoints;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
