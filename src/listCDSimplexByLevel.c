/******************************************************************************
 listCDSimplexByLevel.h  - description
 ----------------------
 begin		: April 2016
 copywirght	: (C) 2016 by J.M.G.Salmerón

 *******************************************************************************/

#include "listCDSimplexByLevel.h"

/**
 * Create new list by level
 */
PLISTCDSBYLEVEL NewListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel) {
	plcdsbylevel = (PLISTCDSBYLEVEL) GetMem(1, sizeof(LISTCDSBYLEVEL), "NewListCDSByLevel");
	plcdsbylevel->pFirstLCDS = NULL;
	plcdsbylevel->NElem = 0;
	plcdsbylevel->MaxNElem = 0;
	return plcdsbylevel;
}

/**
 * Free list by level
 */
PLISTCDSBYLEVEL FreeListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel) {
	PLCDSNODEBYLEVEL aux;

	while (plcdsbylevel->pFirstLCDS != NULL) {
		aux = plcdsbylevel->pFirstLCDS;
		plcdsbylevel->pFirstLCDS = plcdsbylevel->pFirstLCDS->pnext;
		aux = FreeLCDSNodeByLevel(aux);
	}

	free((PVOID) plcdsbylevel);
	plcdsbylevel = NULL;
	return plcdsbylevel;
}

/**
 * New node by level
 */
PLCDSNODEBYLEVEL NewLCDSNodeByLevel(PLISTCDS plcds) {
	PLCDSNODEBYLEVEL plcsnbylevel;

	plcsnbylevel = (PLCDSNODEBYLEVEL) GetMem((SIZE) 1, (SIZE) sizeof(LCDSNODEBYLEVEL), "NewLCDSNodeByLevel");
	plcsnbylevel->plcds = plcds;
	plcsnbylevel->level = -1;
	plcsnbylevel->pnext = NULL;

	return plcsnbylevel;
}

/**
 * Free node by level
 */
PLCDSNODEBYLEVEL FreeLCDSNodeByLevel(PLCDSNODEBYLEVEL plcdsnByLevel) {
	if (plcdsnByLevel == NULL) {
		fprintf(stderr, "FreeLCDSNodeByLevel: Trying to free a NULL node\n");
		exit(1);
	} else if (plcdsnByLevel->plcds != NULL) {
		plcdsnByLevel->plcds = FreeListCDS(plcdsnByLevel->plcds);
	} else {
		fprintf(stderr, "FreeLCDSNodeByLevel: The contained simplex list is NULL.\n");
		exit(1);
	}

	free((PVOID) plcdsnByLevel);
	plcdsnByLevel = NULL;

	return NULL;
}

/**
 * Insert simplex in the right place
 */
VOID InsertListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel, PCDSIMPLEX pCDS) {
	// Search if a list for this simplex level exists
	PLCDSNODEBYLEVEL aux = plcdsbylevel->pFirstLCDS;
	PLCDSNODEBYLEVEL previous = NULL;
	while (aux != NULL) {
		if (aux->level == pCDS->Level) { // Insert here
			InsertListCDS(aux->plcds, pCDS);
			plcdsbylevel->NElem++;
			return;
		} else {
			if (aux->level > pCDS->Level)
				break; // Level list does not exist
			previous = aux;
			aux = aux->pnext;
		}
	}

	// Insert in a new level list in the right place
	PLISTCDS plcds = NULL;
	plcds = NewListCDS(plcds);
	InsertListCDS(plcds, pCDS);
	PLCDSNODEBYLEVEL plcsnbylevelNew = NewLCDSNodeByLevel(plcds);
	plcsnbylevelNew->level = pCDS->Level;
	if (plcdsbylevel->pFirstLCDS != NULL) {
		if (plcdsbylevel->pFirstLCDS->pnext != NULL) {
			plcsnbylevelNew->pnext = previous->pnext;
			previous->pnext = plcsnbylevelNew;
		} else {
			plcdsbylevel->pFirstLCDS->pnext = plcsnbylevelNew;
		}
	} else {
		plcdsbylevel->pFirstLCDS = plcsnbylevelNew;
	}
	plcdsbylevel->NElem++;

	fprintf(stderr, "Insert %lld\n", pCDS->NSimplex);
	PrintListCDSByLevel(plcdsbylevel);
}

/**
 * Check simplex cover
 */
BOOL IsCDSByLevelCovered(PLISTCDSBYLEVEL plcdsbylevel, PCDSIMPLEX pCDS, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat) {
	PLCDSNODEBYLEVEL aux = plcdsbylevel->pFirstLCDS;
	BOOL isCovered = False;

	while (aux != NULL) {
		// Search in pCDS list of levels from pCDS->lastLevel to pCDS->Level
		if (pCDS->lastLevel <= aux->level && aux->level < pCDS->Level) {
			isCovered = IsCDSCovered(aux->plcds, pCDS, NDim, ppVCoorT, ppCDSToVMat);

			if (isCovered) {
				return True;
			}
		}
		aux = aux->pnext;
	}
	return False;
}

/**
 * Print list
 */
VOID PrintListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel) {
	if (plcdsbylevel == NULL) {
		fputs("PrintListCDSByLevel: Empty List\n", stderr);
		exit(1);
	} else {
		fputs("\n--------------------PrintListCDSByLevel---------------------\n", stderr);
		fprintf(stderr, "nElem = %d.\n", plcdsbylevel->NElem);

		PLCDSNODEBYLEVEL levelNode = plcdsbylevel->pFirstLCDS;
		while (levelNode != NULL) {
			fprintf(stderr, "[nElem = %d, level = %d; ", levelNode->plcds->NElem, levelNode->level);
			PLCDSNODE insideNode = levelNode->plcds->pFirstLCDS;
			while (insideNode != NULL) {
				fprintf(stderr, "%lld,", insideNode->pCDS->NSimplex);

				insideNode = insideNode->pnext;
			}
			fprintf(stderr, "]\n");
			levelNode = levelNode->pnext;
		}
	}
}

/**
 * Get first element from list with depth level
 */
PCDSIMPLEX ExtractListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel) {
	PCDSIMPLEX pCDS;

	if (plcdsbylevel->pFirstLCDS == NULL) {
		fprintf(stderr, "ExtractListCDSByLevel: Trying to extract from a NULL List of CDSimplexes\n");
		exit(1);
	}

	PLCDSNODEBYLEVEL levelNode = plcdsbylevel->pFirstLCDS;
	PLCDSNODEBYLEVEL previous = NULL;
	while (levelNode != NULL) {
		if (levelNode->pnext == NULL) {
			pCDS = ExtractListCDS(levelNode->plcds);
			if (levelNode->plcds->NElem == 0) {
				FreeLCDSNodeByLevel(levelNode);
				if (previous)
					previous->pnext = NULL;
			}
			break;
		}
		previous = levelNode;
		levelNode = levelNode->pnext;
	}

	plcdsbylevel->NElem--;

	fprintf(stderr, "Extract %lld\n", pCDS->NSimplex);
	PrintListCDSByLevel(plcdsbylevel);

	return pCDS;
}
