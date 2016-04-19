#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include "argshand.h"
#include "utils.h"
#include "CDSimplex.h"
#include "listCDSimplexByLevel.h"

int main(int argc, char *argv[]) {
	// START
	INT InitNGrid = 4;
	REAL Epsilon = 1.0 / InitNGrid;
	INT WWidth = 600;

	PCDSIMPLEX simplex1, simplex2, simplex3, simplex4, simplex5;
	INT i, j;
	INT NDim = 4;
	REAL IniXi = 1.0;
	REAL IniLength = sqrt(2.0 * IniXi * IniXi);

	PPREAL ppCDSToVMat;
	PPREAL ppVCoorT;
	ppVCoorT = (PPREAL) GetMem2D((SIZE) NDim, (SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:ppVCorrT");
	ppCDSToVMat = (PPREAL) GetMem2D((SIZE) NDim, (SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:ppCDStoVMat");
	for (i = 0; i < NDim; i++)
		for (j = 0; j < NDim; j++)
			if (i != j)
				ppCDSToVMat[i][j] = -1.0 / NDim;
			else
				ppCDSToVMat[i][j] = (REAL) (NDim - 1) / NDim;

	// SIMPLICES
	PREAL pCentreT1 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	for (i = 0; i < NDim; i++)
		pCentreT1[i] = IniXi / (REAL) NDim;
	simplex1 = NewCDSimplex(NDim, pCentreT1, IniLength, 1.0, True, False, 1, 0, 1);

	PREAL pCentreT2 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	pCentreT2[0] = 0.5;//TODO
	pCentreT2[1] = 0.1666666666666;
	pCentreT2[2] = 0.1666666666666;
	pCentreT2[3] = 0.1666666666666;
	simplex2 = NewCDSimplex(NDim, pCentreT2, 0.942809, 0.6666666, True, True, 2, 0, 2);

	PREAL pCentreT3 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	pCentreT3[0] = pCentreT1[0] / 2;
	pCentreT3[1] = pCentreT1[1] / 2;
	pCentreT3[2] = pCentreT1[2] / 2;
	pCentreT3[3] = pCentreT1[3] / 2;
	simplex3 = NewCDSimplex(NDim, pCentreT3, IniLength / 2, 1.0 / 2, True, False, 3, 0, 3);

	PREAL pCentreT4 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	pCentreT4[0] = 0.0;
	pCentreT4[1] = 0.0;
	pCentreT4[2] = 0.0;
	pCentreT4[3] = 0.0;
	simplex4 = NewCDSimplex(NDim, pCentreT4, IniLength, 1.0, True, False, 4, 0, 3);

	PREAL pCentreT5 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	pCentreT5[0] = 0.0;
	pCentreT5[1] = 0.0;
	pCentreT5[2] = 0.0;
	pCentreT5[3] = 0.0;
	simplex5 = NewCDSimplex(NDim, pCentreT5, IniLength, 1.0, True, False, 5, 0, 1);

	// DRAW
	printf("%d\n", WWidth);
	printf("%d\n", WWidth);
	printf("0.0\n");
	printf("IniLength\n");
	printf("0.0\n");
	printf("IniLength\n");
	printf("%2.4f\n", Epsilon);
	printf("%s\n", "Regular partition");

	DrawCDSimplex(simplex1, ppVCoorT, ppCDSToVMat, True, NDim, WWidth, "Black");
	DrawCDSimplex(simplex2, ppVCoorT, ppCDSToVMat, True, NDim, WWidth, "Black");
	DrawCDSimplex(simplex3, ppVCoorT, ppCDSToVMat, True, NDim, WWidth, "Black");

	// LIST
	PLISTCDSBYLEVEL lista = NULL;
	lista = NewListCDSByLevel(lista);

	InsertListCDSByLevel(lista, simplex1);
	InsertListCDSByLevel(lista, simplex2);
	InsertListCDSByLevel(lista, simplex3);
	InsertListCDSByLevel(lista, simplex4);
	//InsertListCDSByLevel(lista, simplex5);

	PrintListCDSByLevel(lista);

	// COVERING
	BOOL isCovered = IsCDSByLevelCovered(lista, simplex5, NDim);
	fprintf(stderr, "%d\n", isCovered);

	// FREE
	FreeListCDSByLevel(lista);
	Free2D((PPVOID) ppVCoorT, NDim);
	Free2D((PPVOID) ppCDSToVMat, NDim);
	free((PVOID) pCentreT1);
	free((PVOID) pCentreT2);
	free((PVOID) pCentreT3);
	free((PVOID) pCentreT4);
	free((PVOID) pCentreT5);

	return 0;
}
