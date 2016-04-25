/******************************************************************************
 listInt.h  - description
 ----------------------
 begin		: April 2016
 copywirght	: (C) 2016 by J.M.G.Salmerón.

 ********************************************************************

 Purpose: Define a list of int to manage int values

 *******************************************************************************/

#ifndef __LISTINT__
#define __LISTINT__

#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#include "getmem.h"
#include "utils.h"

/*---------------------------------------------------------------------------*/

struct IntNode {
	ULLINT id;
	INT level;
	struct IntNode* pnext;
};

typedef struct IntNode INTNODE;
typedef INTNODE* PINTNODE;

/*---------------------------------------------------------------------------*/

struct ListInt {
	INT NElem;
	INT MaxNElem;
	PINTNODE pFirstInt;
};

typedef struct ListInt LISTINT;
typedef LISTINT* PLISTINT;

/*---------------------------------------------------------------------------*/

PLISTINT NewListInt(PLISTINT plint);
PLISTINT FreeListInt(PLISTINT plint);
PINTNODE NewIntNode(ULLINT id, INT level);
PINTNODE FreeIntNode(PINTNODE plnode);
VOID InsertListInt(PLISTINT plint, PINTNODE pIntNode);
VOID InsertListIntByLevel(PLISTINT plint, PINTNODE pnew);
VOID PrintListInt(PLISTINT plint);
PINTNODE ExtractListIntByID(PLISTINT plint, ULLINT id);
PINTNODE ExtractFirstListInt(PLISTINT plint);
PINTNODE GetNodeIntByIndex(PLISTINT plint, INT i);

/*---------------------------------------------------------------------------*/

#endif /*__LISTINT__*/
