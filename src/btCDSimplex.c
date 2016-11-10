/******************************************************************************
 btCDSimplex.h  - description
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
#include "btCDSimplex.h"

/*---------------------------------------------------------------------------*/
PBTCDSNODE NewBTCDSNODE(PCDSIMPLEX pCDS) {
	PBTCDSNODE pBTCDSNode;

	pBTCDSNode = GetMem(1, sizeof(BTCDSNODE), "NewBTCDSNODE");
	pBTCDSNode->pCDS = pCDS;
	pBTCDSNode->Balance = EQUAL;
	pBTCDSNode->pleft = NULL;
	pBTCDSNode->pright = NULL;
	return pBTCDSNode;
}

/*---------------------------------------------------------------------------*/
PBTCDSNODE FreeBTCDSNode(PBTCDSNODE pBTCDSNode) {
	if (pBTCDSNode == NULL) {
		fprintf(stderr, "Trying to free a NULL BTCDSNODE pointer.\n");
		exit(1);
	}
	pBTCDSNode->pCDS = FreeCDSimplex(pBTCDSNode->pCDS);
	free((PVOID) pBTCDSNode);
	pBTCDSNode = NULL;
	return pBTCDSNode;
}

/*----------------------------------------------------------------------------*/
/*Swap the nodes, but the structure of the BLTree does not change.            */
VOID SwapBTCDSNode(PBTCDSNODE pBTCDSNodeO, PBTCDSNODE pBTCDSNodeT) {
	PCDSIMPLEX pCDS;

	pCDS = pBTCDSNodeO->pCDS;
	pBTCDSNodeO->pCDS = pBTCDSNodeT->pCDS;
	pBTCDSNodeT->pCDS = pCDS;
}

/*----------------------------------------------------------------------------*/
/*Swap the data and copy the Balance and pointers of the BTCDSNodes.          */
/*Node2 will be removed with data of node1 after exchange.                    */
/*----------------------------------------------------------------------------*/
VOID ExchangeCDSNode(PBTCDSNODE node1, PBTCDSNODE node2) {
	SwapBTCDSNode(node1, node2);
	node1->Balance = node2->Balance;
	node1->pleft = node2->pleft;
	node1->pright = node2->pright;

	node2->pleft = NULL;
	node2->pright = NULL;
}

/*---------------------------------------------------------------------------*/
void PrintBTCDSSubTreeInOrder(PBTCDSNODE node, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat) {
	if (node != NULL) {
		PrintBTCDSSubTreeInOrder(node->pleft, NDim, ppVCoorT, ppCDSToVMat);
		fprintf(stderr, "(%d)->", node->Balance);
		PrintCDSimplex(node->pCDS, NDim, ppVCoorT, ppCDSToVMat);
		PrintBTCDSSubTreeInOrder(node->pright, NDim, ppVCoorT, ppCDSToVMat);
	} else
		fprintf(stderr, "N ");
}

/*---------------------------------------------------------------------------*/
void PrintBTCDSSubTree(PBTCDSNODE node, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat) {
	if (node != NULL) {
		fprintf(stderr, "(%d)->", node->Balance);
		PrintCDSimplex(node->pCDS, NDim, ppVCoorT, ppCDSToVMat);
		PrintBTCDSSubTree(node->pleft, NDim, ppVCoorT, ppCDSToVMat);
		PrintBTCDSSubTree(node->pright, NDim, ppVCoorT, ppCDSToVMat);
	} else
		fprintf(stderr, "N ");
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PBTCDS NewBTCDS(PBTCDS pBTCDS) {
	pBTCDS = GetMem(1, sizeof(BTCDS), "NewBTCDS");
	pBTCDS->NElem = 0;
	pBTCDS->MaxNElem = 0;
	pBTCDS->pFirstBTCDSNode = NULL;
	return pBTCDS;
}

/*----------------------------------------------------------------------------*/
void FreeBTCDSSubTree(PBTCDSNODE root) {
	if (root != NULL) {
		FreeBTCDSSubTree(root->pleft);
		FreeBTCDSSubTree(root->pright);
		root = FreeBTCDSNode(root);
	}
}

/*----------------------------------------------------------------------------*/
PBTCDS FreeBTCDS(PBTCDS pbtcds) {
	if (pbtcds != NULL) {
		FreeBTCDSSubTree(pbtcds->pFirstBTCDSNode);
		pbtcds->pFirstBTCDSNode = NULL;
		free((PVOID) pbtcds);
		pbtcds = NULL;
	}
	return NULL;
}

/*---------------------------------------------------------------------------*/
void PrintBTCDS(PBTCDS pbtcds, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat) {
	if (pbtcds != NULL) {
		PrintBTCDSSubTree(pbtcds->pFirstBTCDSNode, NDim, ppVCoorT, ppCDSToVMat);
		fprintf(stderr, "Number of Elements     = %d\n", pbtcds->NElem);
		fprintf(stderr, "Max Number of Elements = %d\n", pbtcds->MaxNElem);
	} else
		fprintf(stderr, "N ");
}

/*----------------------------------------------------------------------------*/
/*Rotate left a binary Tree                                                   */
/*----------------------------------------------------------------------------*/
PBTCDSNODE RotateBTCDSLeft(PBTCDSNODE root) {
	PBTCDSNODE Temp;

	if (root == NULL) {
		fprintf(stderr, "BTCDSimplex::RotateBTCDSLeft :");
		;
		fprintf(stderr, " Can not rotate to left an empty tree \n");
		exit(1);
	} else if (root->pright == NULL) {
		fprintf(stderr, "BTCDSimplex::RotateBTCDSLeft: Can not rotate to left\n");
		exit(1);
	} else {
		Temp = root->pright;
		root->pright = Temp->pleft;
		Temp->pleft = root;
		root = Temp;
	}
	return Temp;
}

/*----------------------------------------------------------------------------*/
/*Rotate right a binary Tree                                                  */
/*----------------------------------------------------------------------------*/
PBTCDSNODE RotateBTCDSRight(PBTCDSNODE root) {
	PBTCDSNODE Temp;

	if (root == NULL) {
		fprintf(stderr, "BTCDSimplex::RotateBTCDSRight :");
		fprintf(stderr, " Can not rotate to right an empty tree \n");
		exit(1);
	} else if (root->pleft == NULL) {
		fprintf(stderr, "BTCDSimplex:RotateBTCDSRight:Can not rotate to right\n");
		exit(1);
	} else {
		Temp = root->pleft;
		root->pleft = Temp->pright;
		Temp->pright = root;
		root = Temp;
	}
	return Temp;
}

/*----------------------------------------------------------------------------*/
/*Insert in the left branch maintaining the global balance. Taller noticed if */
/*the insertion has increased the heigh or not, so the balance factors can be */
/*changed appropiately and rotations may be done. Taller has efects only as   */
/* return value. We initiate it to false.                                     */
/*----------------------------------------------------------------------------*/
PBTCDSNODE LeftInsertBTCDSBalance(PBTCDSNODE root, PBOOL ptaller) {
	PBTCDSNODE Left, LeftRight;

	Left = root->pleft;
	switch (Left->Balance) {
	case LEFT:
		root->Balance = EQUAL;
		Left->Balance = EQUAL;
		root = RotateBTCDSRight(root);
		*ptaller = False;
		break;
	case EQUAL:
		fprintf(stderr, "BTCDSimplex:LeftInsertBTCDSBalance :");
		fprintf(stderr, " The tree is already balanced\n");
		exit(1);
		break;
	case RIGHT:
		LeftRight = Left->pright;
		switch (LeftRight->Balance) {
		case LEFT:
			root->Balance = RIGHT;
			Left->Balance = EQUAL;
			break;
		case EQUAL:
			root->Balance = EQUAL;
			Left->Balance = EQUAL;
			break;
		case RIGHT:
			root->Balance = EQUAL;
			Left->Balance = LEFT;
			break;
		}
		LeftRight->Balance = EQUAL;
		root->pleft = RotateBTCDSLeft(root->pleft);
		root = RotateBTCDSRight(root);
		*ptaller = False;
		break;
	}
	return root;
}

/*---------------------------------------------------------------------------*/
/*Analogous to previous one but on the right.                                */
/*---------------------------------------------------------------------------*/
PBTCDSNODE RightInsertBTCDSBalance(PBTCDSNODE root, PBOOL ptaller) {
	PBTCDSNODE Right, RightLeft;

	Right = root->pright;
	switch (Right->Balance) {
	case LEFT:
		RightLeft = Right->pleft;
		switch (RightLeft->Balance) {
		case LEFT:
			root->Balance = EQUAL;
			Right->Balance = RIGHT;
			break;
		case EQUAL:
			root->Balance = EQUAL;
			Right->Balance = EQUAL;
			break;
		case RIGHT:
			root->Balance = LEFT;
			Right->Balance = EQUAL;
			break;
		}
		RightLeft->Balance = EQUAL;
		root->pright = RotateBTCDSRight(root->pright);
		root = RotateBTCDSLeft(root);
		*ptaller = False;
		break;
	case EQUAL:
		fprintf(stderr, "BTCDSimplex:RightInsertBTCDSBalance: ");
		fprintf(stderr, "The tree is already balanced\n");
		exit(1);
		break;
	case RIGHT:
		root->Balance = EQUAL;
		Right->Balance = EQUAL;
		root = RotateBTCDSLeft(root);
		*ptaller = False;
		break;
	}
	return root;
}

/*---------------------------------------------------------------------------*/
/*Do the actual insertion in the BTSimplex.                                  */
/*If exist return True in pAdd                                               */
/*---------------------------------------------------------------------------*/PBTCDSNODE InsertBTCDS1(PBTCDSNODE root, PCDSIMPLEX pCDS, INT NDim,
		PBOOL ptaller, PBOOL pAdd) {
	COMPARE CompCoor;

	if (root == NULL) {
		root = NewBTCDSNODE(pCDS);
		*pAdd = True;
		*ptaller = True; /*Only in this case the tree balanced label are changed*/
	} else {
		CompCoor = CompareVR(pCDS->pCentre, root->pCDS->pCentre, NDim);
		if (CompCoor == Equal)
			(*pAdd) = False; //It is already, but we reinforce it.
		else if (CompCoor == Smaller) {
			root->pleft = InsertBTCDS1(root->pleft, pCDS, NDim, ptaller, pAdd);
			if (*ptaller)
				switch (root->Balance) {
				case LEFT:
					root = LeftInsertBTCDSBalance(root, ptaller);
					break;
				case EQUAL:
					root->Balance = LEFT;
					break;
				case RIGHT:
					root->Balance = EQUAL;
					*ptaller = False;
					break;
				}
		} else {
			root->pright = InsertBTCDS1(root->pright, pCDS, NDim, ptaller, pAdd);
			if (*ptaller)
				switch (root->Balance) {
				case LEFT:
					root->Balance = EQUAL;
					*ptaller = False;
					break;
				case EQUAL:
					root->Balance = RIGHT;
					break;
				case RIGHT:
					root = RightInsertBTCDSBalance(root, ptaller);
					break;
				}
		}
	}
	return root;
}

/*---------------------------------------------------------------------------*/
VOID InsertBTCDS(PBTCDS pbtcds, PCDSIMPLEX pCDS, INT NDim, PBOOL pAdd) {
	BOOL taller = False;

	if (pbtcds->pFirstBTCDSNode == NULL)
		pbtcds->Size = pCDS->L;

	/* With 2NUSC this is not longer true
	 if (!EQ(pbtcds->Size,pCDS->L))
	 {
	 fprintf(stderr,"InsertBTCDS, pCDS-L = %f != Size=%f.\n",
	 pCDS->L,pbtcds->Size);
	 exit(1);
	 }
	 */

	*pAdd = False;
	pbtcds->pFirstBTCDSNode = InsertBTCDS1(pbtcds->pFirstBTCDSNode, pCDS, NDim, &taller, pAdd);
	if (*pAdd == True) {
		pbtcds->NElem++;

		if (pbtcds->NElem > pbtcds->MaxNElem)
			pbtcds->MaxNElem = pbtcds->NElem;
	}
}

/*---------------------------------------------------------------------------*/
/*Update the balance factors and do the necesary rotations when a left node  */
/*is removed.                                                                */
/*---------------------------------------------------------------------------*/
PBTCDSNODE LeftDelBTCDSBalance(PBTCDSNODE root, PBOOL pshorter) {
	PBTCDSNODE Left, LeftRight;

	Left = root->pleft;
	switch (Left->Balance) {
	case LEFT:
		root->Balance = Left->Balance = EQUAL;
		root = RotateBTCDSRight(root);
		break;
	case EQUAL:
		root->Balance = LEFT;
		Left->Balance = RIGHT;
		root = RotateBTCDSRight(root);
		*pshorter = False;
		break;
	case RIGHT:
		LeftRight = Left->pright;
		switch (LeftRight->Balance) {
		case LEFT:
			root->Balance = RIGHT;
			Left->Balance = EQUAL;
			break;
		case EQUAL:
			root->Balance = Left->Balance = EQUAL;
			break;
		case RIGHT:
			root->Balance = EQUAL;
			Left->Balance = LEFT;
			break;
		}
		LeftRight->Balance = EQUAL;
		root->pleft = RotateBTCDSLeft(root->pleft);
		root = RotateBTCDSRight(root);
		break;
	}
	return root;
}

/*---------------------------------------------------------------------------*/
/*Analogous to previous one but on the right.                                */
/*---------------------------------------------------------------------------*/
PBTCDSNODE RightDelBTCDSBalance(PBTCDSNODE root, PBOOL pshorter) {
	PBTCDSNODE Right, RightLeft;

	Right = root->pright;
	switch (Right->Balance) {
	case LEFT:
		RightLeft = Right->pleft;
		switch (RightLeft->Balance) {
		case LEFT:
			root->Balance = EQUAL;
			Right->Balance = RIGHT;
			break;
		case EQUAL:
			root->Balance = Right->Balance = EQUAL;
			break;
		case RIGHT:
			root->Balance = LEFT;
			Right->Balance = EQUAL;
			break;
		}
		RightLeft->Balance = EQUAL;
		root->pright = RotateBTCDSRight(root->pright);
		root = RotateBTCDSLeft(root);
		break;
	case EQUAL:
		root->Balance = RIGHT;
		Right->Balance = LEFT;
		root = RotateBTCDSLeft(root);
		*pshorter = False;
		break;
	case RIGHT:
		root->Balance = Right->Balance = EQUAL;
		root = RotateBTCDSLeft(root);
		break;
	}
	return root;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
