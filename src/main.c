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
#include "listInt.h"

BOOL IsCovered(PCDSIMPLEX pCDSO, PCDSIMPLEX pCDSD, INT NDim) {
	INT j;
	BOOL IsCovered = False;
	BOOL Up;

	if (pCDSO->Up == pCDSD->Up) { //Only if they has the same orientation
		IsCovered = True;
		Up = pCDSO->Up;
		for (j = 0; j < NDim; j++)
			if ((Up == True && LT(pCDSO->pCentre[j] - pCDSD->pCentre[j] + (pCDSD->R - pCDSO->R) / (REAL) NDim, 0.0))
					|| (Up == False && LT(pCDSD->pCentre[j] - pCDSO->pCentre[j] + (pCDSD->R - pCDSO->R) / (REAL) NDim, 0.0))) {
				IsCovered = False;
				break;
			}
	}

	return IsCovered;
}

int main(int argc, char *argv[]) {
	// START
	INT InitNGrid = 4;
	REAL Epsilon = 1.0 / InitNGrid;
	INT WWidth = 600;

	PCDSIMPLEX simplex1, simplex2, simplex3, simplex4, simplex5, aux;
	INT i, j;
	INT NDim = 4;
	REAL IniXi = 1.0;
	REAL IniLength = sqrt(2.0 * IniXi * IniXi);

	BOOL Draw = False;

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
	pCentreT2[0] = pCentreT1[0];
	pCentreT2[1] = pCentreT1[1] / 2;
	pCentreT2[2] = pCentreT1[2];
	pCentreT2[3] = pCentreT1[3] / 2;
	simplex2 = NewCDSimplex(NDim, pCentreT2, 0.707107, 0.5, True, False, 2, 0, 2);

	PREAL pCentreT3 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	pCentreT3[0] = pCentreT1[0] / 2;
	pCentreT3[1] = pCentreT1[1] / 2;
	pCentreT3[2] = pCentreT1[2] / 2;
	pCentreT3[3] = pCentreT1[3] / 2;
	simplex3 = NewCDSimplex(NDim, pCentreT3, 0.707107, 0.5, True, False, 3, 0, 3);

	PREAL pCentreT4 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	pCentreT4[0] = pCentreT1[0] / 2 + 0.5;
	pCentreT4[1] = pCentreT1[1] / 2;
	pCentreT4[2] = pCentreT1[2] / 2;
	pCentreT4[3] = pCentreT1[3] / 2;
	simplex4 = NewCDSimplex(NDim, pCentreT4, 0.707107, 0.5, True, False, 4, 0, 3);

	PREAL pCentreT5 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	pCentreT5[0] = 0.625;
	pCentreT5[1] = 0.0625;
	pCentreT5[2] = 0.0625;
	pCentreT5[3] = 0.25;
	simplex5 = NewCDSimplex(NDim, pCentreT5, sqrt(2) / 4, 0.25, True, False, 5, 0, 4);

	//PrintCDSimplex(simplex3, NDim, ppVCoorT, ppCDSToVMat);
	//PrintCDSimplex(simplex4, NDim, ppVCoorT, ppCDSToVMat);

	BOOL isCovered = IsCovered(simplex4, simplex3, NDim);
	fprintf(stderr, "S4 is covered S3 ? %d\n", isCovered);

	// DRAW
	if (Draw) {
		printf("%d\n", WWidth);
		printf("%d\n", WWidth);
		printf("0.0\n");
		printf("IniLength\n");
		printf("0.0\n");
		printf("IniLength\n");
		printf("%2.4f\n", Epsilon);
		printf("%s\n", "Regular partition");

		DrawCDSimplex(simplex1, ppVCoorT, ppCDSToVMat, Draw, NDim, WWidth, "Black");
		DrawCDSimplex(simplex2, ppVCoorT, ppCDSToVMat, Draw, NDim, WWidth, "Red");
		DrawCDSimplex(simplex3, ppVCoorT, ppCDSToVMat, Draw, NDim, WWidth, "Blue");
		DrawCDSimplex(simplex4, ppVCoorT, ppCDSToVMat, Draw, NDim, WWidth, "Green");
		DrawCDSimplex(simplex5, ppVCoorT, ppCDSToVMat, Draw, NDim, WWidth, "Yellow");
	}

	// LIST
	PLISTCDSBYLEVEL lista = NULL;
	lista = NewListCDSByLevel(lista);

	// INSERT
	InsertListCDSByLevel(lista, simplex1);
	InsertListCDSByLevel(lista, simplex2);
	InsertListCDSByLevel(lista, simplex3);
	InsertListCDSByLevel(lista, simplex4);
	InsertListCDSByLevel(lista, simplex5);

	PrintListCDSByLevel(lista);

	fprintf(stderr, "--------------------------------ListInt------------------------------------\n");
	PLISTINT listInt = NULL;
	listInt = NewListInt(listInt);

	InsertListInt(listInt, NewIntNode(1, 1));
	InsertListInt(listInt, NewIntNode(2, 1));
	InsertListInt(listInt, NewIntNode(3, 2));
	InsertListInt(listInt, NewIntNode(4, 4));

	PrintListInt(listInt);

	PINTNODE intNode = ExtractFirstListInt(listInt);
	fprintf(stderr, "\nExtracted (%lld, %d) from listInt\n", intNode->id, intNode->level);
	FreeIntNode(intNode);
	PrintListInt(listInt);

	intNode = ExtractFirstListInt(listInt);
	fprintf(stderr, "\nExtracted (%lld, %d) from listInt\n", intNode->id, intNode->level);
	FreeIntNode(intNode);
	PrintListInt(listInt);

	intNode = GetNodeIntByIndex(listInt, 1);
	fprintf(stderr, "\nGetted (%lld, %d) from listInt with index 1\n", intNode->id, intNode->level);
	PrintListInt(listInt);

	FreeListInt(listInt);
	fprintf(stderr, "---------------------------------------------------------------------------\n");

	// COVERING
	//TODO Working on
	isCovered = IsCDSByLevelCovered(lista, simplex4, NDim);
	fprintf(stderr, "is covered in list ? %d\n", isCovered);

	// EXTRACT
	aux = ExtractListCDSByLevel(lista);
	fprintf(stderr, "\nExtracted %lld from list\n", aux->NSimplex);
	PrintListCDSByLevel(lista);

	aux = ExtractListCDSByLevel(lista);
	fprintf(stderr, "\nExtracted %lld from list\n", aux->NSimplex);
	PrintListCDSByLevel(lista);

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
