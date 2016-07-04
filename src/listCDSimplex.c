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
	PLCDSNODE pnew, paux;

	pnew = NewLCDSNode(pCDS);

	if (plcds->pFirstLCDS == NULL)
		plcds->pFirstLCDS = pnew;
	else {
		pnew->pnext = plcds->pFirstLCDS;
		plcds->pFirstLCDS = pnew;
		/*
		paux = plcds->pFirstLCDS;
		while (paux != NULL) {
			if (paux->pnext != NULL) {
				if (pnew->pCDS->NSimplex < paux->pCDS->NSimplex) {
					pnew->pnext = paux;
					plcds->pFirstLCDS = pnew;
					break;
				} else if (pnew->pCDS->NSimplex < paux->pnext->pCDS->NSimplex) {
					pnew->pnext = paux->pnext;
					paux->pnext = pnew;
					break;
				}
			} else {
				if (pnew->pCDS->NSimplex > paux->pCDS->NSimplex) {
					paux->pnext = pnew;
				} else {
					pnew->pnext = paux;
					plcds->pFirstLCDS = pnew;
				}
				break;
			}

			paux = paux->pnext;
		}
		*/
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
BOOL IsCDSCovered(PLISTCDS pLCDS, PCDSIMPLEX pCDS, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat) {
	INT i, j;
	PLCDSNODE pLCDSaux;
	PCDSIMPLEX pCDSaux;

	BOOL IsCovered = False;
	BOOL Up;

	BOOL IsOverlaped = False;
	REAL distanceCenters;

	pLCDSaux = pLCDS->pFirstLCDS;

	if (pLCDSaux == NULL)
		return False;

	// Center of pCDS
	PREAL pCentreCDS = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "IsCDSCovered->pCentreCDS");
	PREAL pCentreCDSaux = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "IsCDSCovered->pCentreCDSaux");

	RegToV(NDim, pCDS->pCentre, pCDS->R, pCDS->Up, ppVCoorT, ppCDSToVMat);

	for (i = 0; i < NDim; i++) {
		pCentreCDS[i] = 0.0;
		for (j = 0; j < NDim; j++)
			pCentreCDS[i] += ppVCoorT[i][j];
		pCentreCDS[i] /= NDim;
	}

	while (pLCDSaux != NULL && IsCovered == False) {
		pCDSaux = pLCDSaux->pCDS;
		if (pCDS->Up == pCDSaux->Up) { //Only if they has the same orientation
			IsCovered = True;
			Up = pCDS->Up;

			//Overlaping test
			if (Up == True) {
				// Center of pCDSaux
				RegToV(NDim, pCDSaux->pCentre, pCDSaux->R, pCDSaux->Up, ppVCoorT, ppCDSToVMat);

				for (i = 0; i < NDim; i++) {
					pCentreCDSaux[i] = 0.0;
					for (j = 0; j < NDim; j++)
						pCentreCDSaux[i] += ppVCoorT[i][j];
					pCentreCDSaux[i] /= NDim;
				}

				// Distance between centers
				distanceCenters = 0.0;
				for (j = 0; j < NDim; j++)
					distanceCenters += pow(pCentreCDS - pCentreCDSaux, 2);
				//distanceCenters = sqrt(distanceCenters);

				/*
				 * Suma de radios menor a distancia entre centros indica
				 * que los circulos están solapados (aunque no siginifica
				 * que los simplices lo esten)
				 */

				if (pow(pCDS->R + pCDSaux->R, 2) < distanceCenters) {
					IsOverlaped = True;

					for (j = 0; j < NDim; j++)
						if (LT(pCDS->pCentre[j] - pCDSaux->pCentre[j] + (pCDSaux->R - pCDS->R) / (REAL) NDim, 0.0)) {
							IsCovered = False;
							break;
						}
				}
			} else { // Up == False
				//TODO WHEN OVERLAPED? NEVER?
				for (j = 0; j < NDim; j++)
					if (LT(pCDSaux->pCentre[j] - pCDS->pCentre[j] + (pCDSaux->R - pCDS->R) / (REAL) NDim, 0.0)) {
						IsCovered = False;
						break;
					}
			}
		}
		//next simplex in the list.
		pLCDSaux = pLCDSaux->pnext;
	}

	// Not overlaping in level
	if (IsOverlaped == False)
		pCDS->lastLevel++;

	return IsCovered;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
