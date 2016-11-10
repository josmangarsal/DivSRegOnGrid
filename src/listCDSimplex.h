/******************************************************************************
 listCDSimplex.h  - description
 ----------------------
 begin		: August 2010
 copywirght	: (C) 2010 by L.G.Casado.

 ********************************************************************

 Purpose: Define a list of simplexes which will have the same sort
 key value.

 LDDSNODE: Node of the list:
 Stores:
 A CDSimplex.
 A pointer to the next node in the list.
 Functions:
 NewLCDSNode(): generates a new node of the list of  simplexes.
 FreeLCSSNode(): Free a node of the list of simplexes.

 LISTS: List of CDSimplexes.
 Stores:
 NElem: Number of Elemets in the list..
 pFirstLCDS: First pointer to the list.

 Functions:
 NewListCDS()   : Get memory and initiates the list.
 FreeListCDS()  : Free al the nodes of the list and the memory allocated by
 the list.
 InsertListCDS(): Insert the simplex in the list. Last first (FIFO).
 ExtractListCDS : Extract the last intserted (FIFO)
 PrintListCDS() : Print the contents of the List.

 *******************************************************************************/
#ifndef __LISTCDS__
#define __LISTCDS__

/*---------------------------------------------------------------------------*/
struct lcdsnode {
	PCDSIMPLEX pCDS;
	struct lcdsnode * pnext;
};

typedef struct lcdsnode LCDSNODE;
typedef LCDSNODE * PLCDSNODE;

/*---------------------------------------------------------------------------*/
struct ListCDS {
	int NElem;
	int MaxNElem;
	PLCDSNODE pFirstLCDS;
};

typedef struct ListCDS LISTCDS;
typedef LISTCDS * PLISTCDS;
typedef PLISTCDS * PPLISTCDS;

/*---------------------------------------------------------------------------*/
PLISTCDS NewListCDS(PLISTCDS plcds);
PLISTCDS FreeListCDS(PLISTCDS plcds);
VOID InsertListCDS(PLISTCDS plcds, PCDSIMPLEX pCDS);
PCDSIMPLEX ExtractListCDS(PLISTCDS plcds);
VOID PrintListCDS(PLISTCDS plcds, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat);
BOOL IsCDSCovered(PLISTCDS pLCDS, PCDSIMPLEX pCDS, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat);

#endif /*__LISTCDS__*/		     
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
