/******************************************************************************
			divide.h  - description
			----------------------
	begin		: August 2014
	copywirght	: (C) 2014 by L.G.Casado.

     ********************************************************************

     Purpose: Define the division methods for a regular simplex by regular simplices.

    Functions:
    	NewListCDS()   : Get memory and initiates the list.
	FreeListCDS()  : Free al the nodes of the list and the memory allocated by
		         the list.
	InsertListCDS(): Insert the simplex in the list. Last first (FIFO).
	ExtractListCDS : Extract the last intserted (FIFO)
        PrintListCDS() : Print the contents of the List.

*******************************************************************************/
#ifndef __DIVIDE__
#define __DIVIDE__

/*---------------------------------------------------------------------------*/
PCDSIMPLEX DivideCDSimplex(UCHAR Divide, PCDSIMPLEX pCDS_O,
                           PPREAL ppVCoor_O, PPREAL ppVCoor_T, PREAL pCentreT,
		           PPREAL ppCDSToVMat,PULINT pCountersCDS,
                           REAL Fraction, UCHAR Draw,
                           INT NDim, INT WWidth,
                           PLISTCDS plcds, PBTCDS pbtCDSEnd,
                           REAL Epsilon, REAL GridSize, INT InitNGrid, 
                           REAL IniLXiRatio,
                           BOOL NoStoreFinalS, PBTV pbtv, PBTV pbtvGridPoints);
#endif /*__DIVIDE__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
