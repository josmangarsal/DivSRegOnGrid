/******************************************************************************
 listCDSimplex.h  - description
 ----------------------
 begin		: August 2010
 copywirght	: (C) 2010 by L.G.Casado.

 *******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#include "getmem.h"
#include "utils.h"
#include "btvertex.h"
#include "CDSimplex.h"
#include "listCDSimplex.h"

/*---------------------------------------------------------------------------*/
PLCDSNODE NewLCDSNode(PCDSIMPLEX pCDS) {
	PLCDSNODE plcdsn;

	plcdsn = (PLCDSNODE) GetMem((SIZE) 1, (SIZE) sizeof(LCDSNODE), "NewLCDSNode");
	plcdsn->pCDS = pCDS;
	plcdsn->pnext = NULL;

	return plcdsn;
}

/*---------------------------------------------------------------------------*/
/*Free the memory allocated by a one node of the list.                       */
/*---------------------------------------------------------------------------*/
PLCDSNODE FreeLCDSNode(PLCDSNODE l) {
	if (l == NULL) {
		fprintf(stderr, "FreeLCDSNode: Trying to free a NULL node\n");
		exit(1);
	} else if (l->pCDS != NULL)
		l->pCDS = FreeCDSimplex(l->pCDS);
	else {
		fprintf(stderr, "FreeLCDSNode: The contained simplex is NULL.\n");
		exit(1);
	}

	free((PVOID) l);

	return NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PLISTCDS NewListCDS(PLISTCDS plcds) {
	plcds = (PLISTCDS) GetMem(1, sizeof(LISTCDS), "NewListCDS");
	plcds->pFirstLCDS = NULL;
	plcds->NElem = 0;
	plcds->MaxNElem = 0;
	return plcds;
}

/*---------------------------------------------------------------------------*/
PLISTCDS FreeListCDS(PLISTCDS plcds) {
	PLCDSNODE aux;

	if (plcds->pFirstLCDS != NULL) {
		while (plcds->pFirstLCDS != NULL) {
			aux = plcds->pFirstLCDS;
			plcds->pFirstLCDS = plcds->pFirstLCDS->pnext;
			aux = FreeLCDSNode(aux);
		}
	}

	free((PVOID) plcds);
	plcds = NULL;
	return plcds;
}

/*---------------------------------------------------------------------------*/
/*Insert at beginning of the list                                            */
/*---------------------------------------------------------------------------*/
VOID InsertListCDS(PLISTCDS plcds, PCDSIMPLEX pCDS) {
	PLCDSNODE pnew;

	pnew = NewLCDSNode(pCDS);

	if (plcds->pFirstLCDS == NULL)
		plcds->pFirstLCDS = pnew;
	else {
		pnew->pnext = plcds->pFirstLCDS;
		plcds->pFirstLCDS = pnew;
	}

	plcds->NElem++;

	if (plcds->NElem > plcds->MaxNElem)
		plcds->MaxNElem = plcds->NElem;
}

/*---------------------------------------------------------------------------*/
/*Extract the first Element of the list.                                     */
/*---------------------------------------------------------------------------*/
PCDSIMPLEX ExtractListCDS(PLISTCDS plcds) {
	PLCDSNODE paux;
	PCDSIMPLEX pCDS;

	if (plcds->pFirstLCDS == NULL) {
		fprintf(stderr, "Trying to extract from a NULL List of CDSimplexes\n");
		exit(1);
	}

	paux = plcds->pFirstLCDS;
	plcds->pFirstLCDS = plcds->pFirstLCDS->pnext;
	pCDS = paux->pCDS;
	paux->pCDS = NULL;
	free((PVOID) paux);
	plcds->NElem--;

	return pCDS;
}
/*---------------------------------------------------------------------------*/
VOID PrintListCDS(PLISTCDS plcds, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat) {
	PLCDSNODE pprev, pnext;

	if (plcds->pFirstLCDS == NULL) {
		fputs("PrintListCDS: Empty List", stderr);
		exit(1);
	} else {
		fputs("\n--------------------PrintLisCDS---------------------\n", stderr);
		fprintf(stderr, "nElem = %d.\n", plcds->NElem);

		pprev = plcds->pFirstLCDS;
		pnext = pprev->pnext;
		PrintCDSimplex(pprev->pCDS, NDim, ppVCoorT, ppCDSToVMat);
		fputs("->", stderr);
		while (pnext != NULL) {
			pprev = pnext;
			pnext = pnext->pnext;
			PrintCDSimplex(pprev->pCDS, NDim, ppVCoorT, ppCDSToVMat);
		}
	}
	fputs("null", stderr);
}

/*---------------------------------------------------------------------------*/
BOOL IsCDSCovered(PLISTCDS pLCDS, PCDSIMPLEX pCDS, INT NDim) {
	INT j;
	PLCDSNODE pLCDSaux;
	PCDSIMPLEX pCDSaux;

	BOOL IsCovered = False;
	BOOL Up;

	pLCDSaux = pLCDS->pFirstLCDS;

	if (pLCDSaux == NULL)
		return False;

	while (pLCDSaux != NULL && IsCovered == False) {
		pCDSaux = pLCDSaux->pCDS;
		if (pCDS->Up == pCDSaux->Up) { //Only if they has the same orientation
			IsCovered = True;
			Up = pCDS->Up;
			for (j = 0; j < NDim; j++)
				if ((Up == True && LT(pCDS->pCentre[j] - pCDSaux->pCentre[j] + (pCDSaux->R - pCDS->R) / (REAL) NDim, 0.0))
						|| (Up == False && LT(pCDSaux->pCentre[j] - pCDS->pCentre[j] + (pCDSaux->R - pCDS->R) / (REAL) NDim, 0.0))) {
					IsCovered = False;
					break;
				}
		}
		//next simplex in the list.
		pLCDSaux = pLCDSaux->pnext;
	}

	return IsCovered;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
