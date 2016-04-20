/******************************************************************************
 listCDSimplexBySize.h  - description
 ----------------------
 begin		: April 2016
 copywirght	: (C) 2016 by J.M.G.Salmerón.

 ********************************************************************

 Purpose: Define a list of lists of simplexes ordered by simplex level.

 *******************************************************************************/

#ifndef __LISTCDSBYLEVEL__
#define __LISTCDSBYLEVEL__

#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#include "getmem.h"
#include "utils.h"
#include "btvertex.h"
#include "CDSimplex.h"
#include "listCDSimplex.h"

/*---------------------------------------------------------------------------*/

struct lcdsnodebylevel {
	PLISTCDS plcds;
	int level;
	struct lcdsnodebylevel * pnext;
};

typedef struct lcdsnodebylevel LCDSNODEBYLEVEL;
typedef LCDSNODEBYLEVEL * PLCDSNODEBYLEVEL;

/*---------------------------------------------------------------------------*/

struct ListCDSByLevel {
	int NElem;
	int MaxNElem;
	PLCDSNODEBYLEVEL pFirstLCDS;
};

typedef struct ListCDSByLevel LISTCDSBYLEVEL;
typedef LISTCDSBYLEVEL * PLISTCDSBYLEVEL;
typedef PLISTCDSBYLEVEL * PPLISTCDSBYLEVEL;

/*---------------------------------------------------------------------------*/

PLISTCDSBYLEVEL NewListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel);
PLISTCDSBYLEVEL FreeListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel);
PLCDSNODEBYLEVEL NewLCDSNodeByLevel(PLISTCDS plcds);
PLCDSNODEBYLEVEL FreeLCDSNodeByLevel(PLCDSNODEBYLEVEL plcdsnByLevel);
VOID InsertListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel, PCDSIMPLEX pCDS);
BOOL IsCDSByLevelCovered(PLISTCDSBYLEVEL plcdsbylevel, PCDSIMPLEX pCDS, INT NDim);
VOID PrintListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel);
PCDSIMPLEX ExtractListCDSByLevel(PLISTCDSBYLEVEL plcdsbylevel);

/*---------------------------------------------------------------------------*/

#endif /*__LISTCDSBYLEVEL__*/
