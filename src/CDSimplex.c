/******************************************************************************
 CDSimplex.c  - description
 ----------------------
 begin		: August 2010
 copywirght	: (C) 2010 by L.G.Casado.
 *******************************************************************************/

#include "CDSimplex.h"

/*---------------------------------------------------------------------------*/
PCDSIMPLEX GetMemCDSimplex(INT NDim) {
	PCDSIMPLEX pCDS;

	pCDS = (PCDSIMPLEX) GetMem((SIZE) 1, (SIZE) sizeof(CDSIMPLEX), "GetMemCDSimplex");
	pCDS->pCentre = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "GetMemCDSimplex->pCentre");
	return pCDS;
}

/*---------------------------------------------------------------------------*/
/*VOID VToCDS (INT NDim, PPREAL ppCoor, PCDSIMPLEX pCDS)
 {
 INT i,j;

 for (i=0;i<NDim;i++)
 {
 pCDS->pCentre[i]=0.0;
 for (j=0;j<NDim;j++)
 pCDS->pCentre[i]+=ppCoor[j][i];
 pCDS->pCentre[i]/=NDim;
 }

 //Calculate the Simplex's Length. TO-DO: it is known from father
 pCDS->L=DistPoints(ppCoor[0], ppCoor[1], NDim);


 //Calculate the Simplex's radius
 //The following radius is wrong because it is normalices to 1 ¿?
 pCDS->R=DistPoints(pCDS->pCentre, ppCoor[0], NDim);


 }
 */
/*---------------------------------------------------------------------------*/
VOID RegToV(INT NDim, PREAL pCentre, REAL R, BOOL Up, PPREAL ppVCoor, PPREAL ppCDSToVMat) {
	INT i, j;

	for (i = 0; i < NDim; i++)
		for (j = 0; j < NDim; j++)
			if (Up)
				ppVCoor[i][j] = pCentre[j] + R * ppCDSToVMat[i][j];
			else
				ppVCoor[i][j] = pCentre[j] - R * ppCDSToVMat[i][j];
}

/*---------------------------------------------------------------------------*/
PCDSIMPLEX NewCDSimplex(INT NDim, PREAL pCentre, REAL L, REAL R, BOOL Up, BOOL Overlap, ULLINT NSimplex, UCHAR DivPhase, INT Level) {

	PCDSIMPLEX pCDS;

	pCDS = GetMemCDSimplex(NDim);

	CopyVR(pCDS->pCentre, pCentre, NDim);
	pCDS->NSimplex = NSimplex;
	pCDS->L = L;
	pCDS->R = R;
	pCDS->Up = Up;
	pCDS->Overlap = Overlap;
	pCDS->DivPhase = DivPhase;
	pCDS->Level = Level;

	pCDS->lastLevel = 0;

	return pCDS;
}

/*---------------------------------------------------------------------------*/
/* Use the memory allocated to an useless simplex.                           */
/*---------------------------------------------------------------------------*/
VOID UpdateCDSimplex(PCDSIMPLEX pCDS, INT NDim, PREAL pCentre, REAL L, REAL R, BOOL Up, BOOL Overlap, ULLINT NSimplex, UCHAR DivPhase) {
	if (pCDS == NULL) {
		fprintf(stderr, "UpdateCDSimplexCoor: pCDS=NULL.\n");
		exit(1);
	}
	CopyVR(pCDS->pCentre, pCentre, NDim);
	pCDS->NSimplex = NSimplex;
	pCDS->L = L;
	pCDS->R = R;
	pCDS->Up = Up;
	pCDS->Overlap = Overlap;
	pCDS->DivPhase = DivPhase;
}

/*---------------------------------------------------------------------------*/
PCDSIMPLEX FreeCDSimplex(PCDSIMPLEX pCDS) {
	if (pCDS == NULL) {
		fputs("FreeCDSimplex:Trying to free a null CDSimplex pointer.", stderr);
		exit(1);
	} else {
		free((PVOID) pCDS->pCentre);
		free((PVOID) pCDS);
		return NULL;
	}
}

/*---------------------------------------------------------------------------*/
VOID PrintCDSimplex(PCDSIMPLEX pCDS, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat) {
	INT i;

	fprintf(stderr, "NSimplex=%llu.\n", pCDS->NSimplex);
	PrintVR(stderr, pCDS->pCentre, NDim);
	fprintf(stderr, "Length=%f.\n", pCDS->L);
	fprintf(stderr, "Radius=%f.\n", pCDS->R);
	fprintf(stderr, "Up=%d.\n", pCDS->Up);
	fprintf(stderr, "Overlap=%d.\n", pCDS->Overlap);

	RegToV(NDim, pCDS->pCentre, pCDS->R, pCDS->Up, ppVCoorT, ppCDSToVMat);

	for (i = 0; i < NDim; i++) {
		fprintf(stderr, "v[%d]=\n", i);
		PrintVR(stderr, ppVCoorT[i], NDim);
	}

	fputs("\n", stderr);
}

/*---------------------------------------------------------------------------*/
VOID StoreVertexCDSimplex(PCDSIMPLEX pCDS, PPREAL ppVCoorT, PPREAL ppCDSToVMat, PBTV pbtv, INT NDim, PBTV pbtvGridPoints) {
	BOOL AddVertex;
	INT i;

	//Get the Simplex's V coordinates
	RegToV(NDim, pCDS->pCentre, pCDS->R, pCDS->Up, ppVCoorT, ppCDSToVMat);

	for (i = 0; i < NDim; i++) {
		InsertBTV(pbtv, NDim, ppVCoorT[i], &AddVertex);
		VISITED(pbtvGridPoints->pFirstBTVNode, NDim, ppVCoorT[i]);
	}
}

/*---------------------------------------------------------------------------*/
BOOL CDSInX123(PPREAL ppVCoorT, PINT pWhich, INT NDim) {
	INT i;
	INT NPointsIn3D;

	if (NDim == 3) {
		NPointsIn3D = 3;
		for (i = 0; i < NDim; i++)
			pWhich[i] = i;
		return True;
	}

	//Only simplices with 3 vertices at x1,x2,x3 neq 0 are drawn.
	if (NDim > 3) {
		NPointsIn3D = 0;
		for (i = 0; i < NDim; i++) //Para cada vertice
			if (PointInX123(ppVCoorT[i], NDim)) {
				pWhich[NPointsIn3D] = i;
				NPointsIn3D++;
			}
	}

	if (NPointsIn3D > 3) {
		fprintf(stderr, "CDSInX123: NPointsIn3D=%d.\n", NPointsIn3D);

		for (i = 0; i < NDim; i++) {
			fprintf(stderr, "v[%d]=\n", i);
			PrintVR(stderr, ppVCoorT[i], NDim);
		}

		fputs("\n", stderr);
		exit(1);
	}
	if (NPointsIn3D == 3)
		return True;
	else
		return False;

}

/*---------------------------------------------------------------------------*/
VOID DrawCDSimplex(PCDSIMPLEX pCDS, PPREAL ppVCoorT, PPREAL ppCDSToVMat, UCHAR Draw, INT NDim, INT WWidth, PCHAR Color) {
	INT i;
	INT pWhich[64];

	//ppVCoorT, to reduce memory allocation

	//Get the Simplex's V coordinates
	RegToV(NDim, pCDS->pCentre, pCDS->R, pCDS->Up, ppVCoorT, ppCDSToVMat);

	if (CDSInX123(ppVCoorT, pWhich, NDim)) {
		printf("DrawLineTriangle\n");
		for (i = 0; i < 3; i++) {
			printf("%f\n", XInWindow(ppVCoorT[pWhich[i]], WWidth));
			printf("%f\n", YInWindow(ppVCoorT[pWhich[i]], WWidth));
		}
		printf("%s\n", Color);
		printf("%llu\n", pCDS->NSimplex);

		if (Draw == 2)
			puts("Wait");

		fflush(stdout);
	}
}

/*---------------------------------------------------------------------------*/
VOID DelCDSimplex(PCDSIMPLEX pCDS, UCHAR Draw) {
	printf("DelLineTriangle\n");
	printf("%llu\n", pCDS->NSimplex);

	if (Draw == 2)
		puts("Wait");

	fflush(stdout);
}

/*---------------------------------------------------------------------------*/
VOID DrawGridPoints(PQueue gridPoints, INT NDim, INT WWidth, PCHAR Color) {
	ULLINT p = 1;

	while (Front(gridPoints) != NULL) {
		double* point = Front(gridPoints);

		if (PointInX123(point, NDim)) {
			printf("DrawPoint\n");

			printf("%f\n", XInWindow(point, WWidth));
			printf("%f\n", YInWindow(point, WWidth));

			printf("%s\n", Color);
			printf("%llu\n", p);

			// No libero porque son los mismo punteros que tiene el arbol
			//free((void*)point);
		}
		Pop(gridPoints);
		p++;
	}
	fflush(stdout);
}
/*---------------------------------------------------------------------------*/
