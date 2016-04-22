/******************************************************************************
 listCDSimplexByLevel.h  - description
 ----------------------
 begin		: April 2016
 copywirght	: (C) 2016 by J.M.G.Salmerón

 *******************************************************************************/

#include "listInt.h"

/**
 * Create new list
 */
PLISTINT NewListInt(PLISTINT plint) {
	plint = (PLISTINT) GetMem(1, sizeof(LISTINT), "NewListInt");
	plint->pFirstInt = NULL;
	plint->NElem = 0;
	plint->MaxNElem = 0;
	return plint;
}

/**
 * Free list
 */
PLISTINT FreeListInt(PLISTINT plint) {
	PINTNODE aux;

	while (plint->pFirstInt != NULL) {
		aux = plint->pFirstInt;
		plint->pFirstInt = plint->pFirstInt->pnext;
		aux = FreeIntNode(aux);
	}

	free((PVOID) plint);
	plint = NULL;
	return plint;
}

/**
 * New node
 */
PINTNODE NewIntNode(ULLINT id, INT level) {
	PINTNODE plnode;

	plnode = (PINTNODE) GetMem((SIZE) 1, (SIZE) sizeof(INTNODE), "NewIntNode");
	plnode->id = id;
	plnode->level = level;
	plnode->pnext = NULL;

	return plnode;
}

/**
 * Free node
 */
PINTNODE FreeIntNode(PINTNODE plnode) {
	if (plnode == NULL) {
		fprintf(stderr, "FreeIntNode: Trying to free a NULL node\n");
		exit(1);
	}

	free((PVOID) plnode);

	return NULL;
}

/**
 * Insert new node in first position
 */
VOID InsertListInt(PLISTINT plint, PINTNODE pnew) {
	if (plint->pFirstInt == NULL)
		plint->pFirstInt = pnew;
	else {
		pnew->pnext = plint->pFirstInt;
		plint->pFirstInt = pnew;
	}

	plint->NElem++;

	if (plint->NElem > plint->MaxNElem)
		plint->MaxNElem = plint->NElem;
}

/**
 * Print list
 */
VOID PrintListInt(PLISTINT plint) {
	if (plint == NULL) {
		fputs("PrintListInt: Empty List\n", stderr);
		exit(1);
	} else {
		fputs("\n--------------------PrintListInt---------------------\n", stderr);
		fprintf(stderr, "nElem = %d.\n", plint->NElem);

		PINTNODE intNode = plint->pFirstInt;
		fprintf(stderr, "[nElem = %d: ", plint->NElem);
		while (intNode != NULL) {
			fprintf(stderr, "(%lld, %d); ", intNode->id, intNode->level);
			intNode = intNode->pnext;
		}
		fprintf(stderr, "]\n");
	}
}

//TODO Doing

/**
 * Get element by id
 */
PINTNODE ExtractListIntByID(PLISTINT plint, ULLINT id) {
	PCDSIMPLEX pCDS;

	if (plint->pFirstInt == NULL) {
		fprintf(stderr, "ExtractListIntByID: Trying to extract from a NULL list\n");
		exit(1);
	}

	PLCDSNODEBYLEVEL levelNode = plcdsbylevel->pFirstLCDS;
	PLCDSNODEBYLEVEL previous = NULL;
	while (levelNode != NULL) {
		if (levelNode->pnext == NULL) {
			pCDS = ExtractListCDS(levelNode->plcds);
			if (levelNode->plcds->NElem == 0) {
				FreeListCDS(levelNode->plcds);
				if (previous)
					previous->pnext = NULL;
			}
			break;
		}
		previous = levelNode;
		levelNode = levelNode->pnext;
	}

	plcdsbylevel->NElem--;

	return pCDS;
}
