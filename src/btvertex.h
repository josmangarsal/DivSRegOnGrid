/******************************************************************************
 btvertex.h  - description
 ----------------------
 begin		: May 2004
 copywirght	: (C) 2004 by L.G.Casado.

 ********************************************************************

 Purpose: Define a balance binary tree of vertexes sorted by their
 possitions and define the nodes of the balance binary tree.

 BTVNODE: Node of the balances binary tree of vertexes:
 Stores:
 pV: A vertex.
 BALANCETYPE: The balance of the subtree which has this node as root.
 pleft,pright: Pointers to the lest and right braches.

 BTV: Balanced Binary Tree of vertexes.
 Stores:
 NElem: Number of Elemets in the tree.
 MaxNElem: Maximum number of vertexes in the tree.
 pFirstBTVNode: Pointer to the fisrt node in the tree.

 Functions:
 NewBTV: Gets memory for a BTV.
 PrintBTV: Print the BTV.
 InsertBTV: Return in ppV the new inserted vertex if it does not exist,
 or the vertex if it exist.
 FreeVertexBTV: Delete the vertex with coordinates pCoor when it do not
 belongs to any simplex.

 *******************************************************************************/
#ifndef __BTV__
#define __BTV__

#define LEFT  -1
#define EQUAL  0
#define RIGHT  1

/*---------------------------------------------------------------------------*/
struct btvnode {
	CHAR Balance;
	PREAL pV;
	BOOL visited;
	struct btvnode * pleft;
	struct btvnode * pright;
};

typedef struct btvnode BTVNODE;
typedef BTVNODE * PBTVNODE;
typedef PBTVNODE * PPBTVNODE;

/*---------------------------------------------------------------------------*/
struct BalancedTreeV {
	int NElem;
	int MaxNElem;
	PBTVNODE pFirstBTVNode;
};

typedef struct BalancedTreeV BTV;
typedef BTV * PBTV;
typedef PBTV * PPBTV;

/*---------------------------------------------------------------------------*/
PBTV NewBTV(PBTV pBTV);
PBTV FreeBTV(PBTV pbtv);
VOID PrintBTV(PBTV pbtv, INT NDim);
VOID PREORDEN(PBTVNODE pNode, INT NDim);
BOOL VISITED(PBTVNODE pNode, INT NDim, PREAL pX);
INT Count(PBTVNODE pNode, INT NDim);
INT CountVisited(PBTVNODE pNode, INT NDim);
VOID InsertBTV(PBTV pbtv, INT NDim, PREAL pCoor, PBOOL pAdd);
VOID FreeVertexBTV(PBTV pbtv, INT NDim, PREAL pCoor);
VOID PrintStatBTV(FILE * FOut, PBTV pbtv, PCHAR String, INT PosIni, INT PosEnd);

#endif /*__BTV__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
