/******************************************************************************
 btCDSimplex.h  - description
 ----------------------
 begin		: August 2014
 copywirght	: (C) 2014 by L.G.Casado.
 ********************************************************************

 Purpose: Define a balance binary tree of CDSimplexes with the same size
 sorted by their centroids and define the nodes of the
 balance binary tree.

 BTSNODE: Node of the balances binary tree of simplexes:
 Stores:
 pCDS: A simplex.
 BALANCETYPE: The balance of the subtree which has this node as root.
 pleft,pright: Pointers to the lest and right braches.

 BTCDS: Balanced Binary Tree of CDSimplexes.
 Stores:
 NElem: Number of Elemets in the tree.
 MaxNElem: Maximum number of vertexes ever in the tree.
 pFirstBTCDSNode: Pointer to the fisrt node in the tree.

 Functions:
 NewBTCDS: Gets memory for a BTCDS.
 PrintBTCDS: Print the BTCDS.
 InsertBTCDS: Insert the CDSimplex pCDS in the BTCDS sorted by the
 centroid.


 *******************************************************************************/
#ifndef __BTCDS__
#define __BTCDS__

/*---------------------------------------------------------------------------*/
struct btcdsnode {
	CHAR Balance;
	PCDSIMPLEX pCDS;
	struct btcdsnode * pleft;
	struct btcdsnode * pright;
};

typedef struct btcdsnode BTCDSNODE;
typedef BTCDSNODE * PBTCDSNODE;
typedef PBTCDSNODE * PPBTCDSNODE;

/*---------------------------------------------------------------------------*/
struct BalancedTreeCDS {
	int NElem;
	int MaxNElem;
	REAL Size;
	PBTCDSNODE pFirstBTCDSNode;
};

typedef struct BalancedTreeCDS BTCDS;
typedef BTCDS * PBTCDS;
typedef PBTCDS * PPBTCDS;

/*---------------------------------------------------------------------------*/
PBTCDS NewBTCDS(PBTCDS pbtCDS);
PBTCDS FreeBTCDS(PBTCDS pbtCDS);
VOID PrintBTCDS(PBTCDS pbtCDS, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat);
VOID InsertBTCDS(PBTCDS pbtCDS, PCDSIMPLEX pCDS, INT NDim, PBOOL pAdd);

#endif /*__BTCDS__*/		     
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
