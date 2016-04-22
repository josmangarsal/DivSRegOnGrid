/******************************************************************************
 CDSimplex.h  - description
 ----------------------
 begin		: August 2010
 copywirght	: (C) 2010 by L.G.Casado.

 ********************************************************************
 Purpose: Define the structure containing the center and distance from
 it to other vertices in regular simplices.
 It is mainly used to avoid already evaluated simplices.
 Given a simplex by its centre and distance is already in
 the btCDSvector, it is not eavluated.

 Stores:
 NSimplex: NSimplex*NDim+i. Als for graphical purposes.
 pCentre	: Simplex's Centroid
 L	: Sinplex's Length.
 R	: Distance form the centre to simplex's vertices.
 Up      : True = Normal Orientation. False= Up side down.
 Overlap : True = Overlapping with brothers.
 DivPhase:	: 0 NDim/NDim+1
 1 NDim-1/NDim.

 Functions:
 NewCDSimplex() : Creates a new simplex. Allocates memory an initialice
 the values.
 FreeCDSimplex() : Free the memory allocates by simplex.


 *******************************************************************************/
#ifndef __CDSIMPLEX__
#define __CDSIMPLEX__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "getmem.h"
#include "utils.h"
#include "btvertex.h"
#include "Queue.h"
#include "listInt.h"

/*---------------------------------------------------------------------------*/
struct typecdsimplex {
	ULLINT NSimplex;
	PREAL pCentre;
	REAL L;
	REAL R;
	BOOL Up;
	BOOL Overlap;
	UCHAR DivPhase;
	INT Level;
	PLISTINT over;
};

typedef struct typecdsimplex CDSIMPLEX;
typedef CDSIMPLEX * PCDSIMPLEX;
typedef PCDSIMPLEX * PPCDSIMPLEX;

/*---------------------------------------------------------------------------*/
VOID VToCDS(INT NDim, PPREAL ppCoor, PCDSIMPLEX pCDS);
VOID RegToV(INT NDim, PREAL pCentre, REAL R, BOOL Up, PPREAL ppVCoor, PPREAL ppCDSToVMat);
PCDSIMPLEX NewCDSimplex(INT NDim, PREAL pCentre, REAL L, REAL R, BOOL Up, BOOL Overlap, ULLINT NSimplex, UCHAR DivPhase, INT Level);
VOID UpdateCDSimplex(PCDSIMPLEX pCDS, INT NDim, PREAL pCentre, REAL L, REAL R, BOOL Up, BOOL Overlap, ULLINT NSimplex, UCHAR DivPhase);
PCDSIMPLEX FreeCDSimplex(PCDSIMPLEX pCDS);
VOID PrintCDSimplex(PCDSIMPLEX pCDS, INT NDim, PPREAL ppVCoorT, PPREAL ppCDSToVMat);
VOID StoreVertexCDSimplex(PCDSIMPLEX pCDS, PPREAL ppVCoorT, PPREAL ppCDSToVMat, PBTV pbtv, INT NDim, PBTV pbtvGridPoints);
VOID DrawCDSimplex(PCDSIMPLEX pCDS, PPREAL ppVCoorT, PPREAL ppCDSToVMat, UCHAR Draw, INT NDim, INT WWidth, PCHAR Color);
VOID DelCDSimplex(PCDSIMPLEX pCDS, UCHAR Draw);
VOID DrawGridPoints(PQueue gridPoints, INT NDim, INT WWidth, PCHAR Color);

#endif /*__CDSIMPLEX__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
