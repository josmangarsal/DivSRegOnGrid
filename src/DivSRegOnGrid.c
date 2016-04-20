/******************************************************************************
 RegSDiv-1.3.c  - description
 ----------------------
 begin		: August 2014
 copywirght	: (C) 2014 by L.G.Casado.

 ********************************************************************
 Purpose: Partitioning of a regular n-simplex by regular simplices.
 All tree is built until termination criterion w(S)<=epsilon.
 Partition without overlapping is not possible for n>=3.
 A Depth-Search is done.
 A simplex is divided or stored in the final list if it is not
 already covered by other in the working list.
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include "argshand.h"
#include "getmem.h"
#include "utils.h"
#include "btvertex.h"
#include "CDSimplex.h"
#include "listCDSimplex.h"
#include "btCDSimplex.h"
#include "divide.h"
#include "gengridpoints.h"
#include "Queue.h"
#include "Map.h"

/*----------------------------------------------------------------------------*/
/*                               MAIN                                         */
/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
	INT i, j;						//Loops
	ULINT CountersCDS[4]; 			//[0]Generated, [1]Evaluated,
									//[2]Final, [3]Final Equal
	BOOL NoStoreFinalS;
	BOOL add;            			//To add vertices in InsertBTV
	CHAR Execution[256];
	FILE * FOut;
	BOOL OutStat;          			//OutPut statistics to a file
	UCHAR Divide;           		//Division method
	REAL IniXi; 		  			//Xi values of initial n-simplex

	REAL IniLength; 	 			//Initial Length
	REAL IniLXiRatio;      			//InitLenght / IniXi ratio.
	REAL Fraction;	  				//Percentage of edge length for new edge
	REAL Epsilon;          			//Epsilon to determine the grid
	REAL GridSize;	  				//Final grid size. alpha in BnbGrid article.
	REAL FinalWidth;	  			//Termination criterion w(S)<=FinalWidth.

	INT InitNGrid = 0;	  			//Number of divisions per edge.
	BOOL SmallInitNGrid = False;	//InitNGrid < 2*NDim-3 for -Div1
	INT NDim;	 	  				//Number of dimensions
	UCHAR Draw;		  				//Have we graphical output?
	INT WWidth;		  				//Window width. graphical purposes.

	PCDSIMPLEX pCDS;		  		//Pointer to a Center-Distance-Simplex
	PPREAL ppCDSToVMat;	  			//Matrix to move from CDS to VCoor
	PPREAL ppVCoorT1;	  			//Temporal Vertex Matrix.
	PPREAL ppVCoorT2;	  			//Temporal Vertex Matrix.
	PREAL pCentreT;	  				//Temporal centroid.

	PLISTCDS plcds = NULL;   	  	//list with working simplices.
	PBTCDS pbtCDSEnd = NULL;   		//AVL tree with final simplices.

	PBTV pbtv = NULL;	  			//AVL tree of vertices

	clock_t c1, c2;		  			//Timing
	struct tms t1, t2;            	//Timing

	/* Check the input parameters.-- -----------------------------------------*/
	if (ExistArg("--help", argc, argv))
		ParametersError();

	if (!ExistArg("-d", argc, argv))
		NDim = 3;
	else {
		NDim = atoi(GetArg("-d", argc, argv));
		if (NDim <= 0 || NDim > 16) {
			fprintf(stderr, "NDim must be > 0 and <=16\n");
			exit(1);
		}
	}

	//Initial coordinates.
	IniXi = 1.0;
	IniLength = sqrt(2.0 * IniXi * IniXi);
	IniLXiRatio = IniLength / IniXi;

	fprintf(stderr, "IniXi=%f, IniLength=%f, IniLXiRatio=%f.\n", IniXi, IniLength, IniLXiRatio);

	//Division rule.
	if (ExistArg("-Div", argc, argv)) {
		Divide = (UCHAR) atoi(GetArg("-Div", argc, argv));
		if (Divide < (UCHAR) 1 || Divide > (UCHAR) 2) {
			fprintf(stderr, "-d in {1,2} Divide=%d\n", Divide);
			exit(1);
		}
	} else
		Divide = (UCHAR) 1;

	//Minimum fraction factor
	if (Divide == 1) 	//2USC Grid
		Fraction = ((REAL) NDim - 1.0) / (REAL) NDim;

	if (Divide == 2)	//2NUSC Grid
			{
		Fraction = ((REAL) NDim - 2.0) / ((REAL) NDim - 1.0);
		//fprintf(stderr,"\n2NUSC not updated yet, Divide=%d.\n\n",Divide);
		//exit(1);
	}

	//Accuracy
	if ((!ExistArg("-ep", argc, argv) && !ExistArg("-g", argc, argv)) || (ExistArg("-ep", argc, argv) && ExistArg("-g", argc, argv))) {
		fputs("Parameter -ep  xor -g are neccesary.\n", stderr);
		ParametersError();
	}

	//Number of divisions of an edge.
	if (ExistArg("-g", argc, argv)) {
		InitNGrid = (INT) atoi(GetArg("-g", argc, argv));
		Epsilon = 1.0 / InitNGrid;
		if (InitNGrid <= 1) {
			fprintf(stderr, "-g  %d  <=1.\n", InitNGrid);
			exit(1);
		}
	}

	//Epsilon of the grid.
	if (ExistArg("-ep", argc, argv)) {
		Epsilon = (REAL) atof(GetArg("-ep", argc, argv));
		if (Epsilon <= 0 || Epsilon >= 1.0) {
			fprintf(stderr, "Epsilon not in (0,1). -gep %f\n", Epsilon);
			exit(1);
		}
	}

	//G in BnBGrid article
	if (InitNGrid == 0) //-g is not used.
		InitNGrid = (INT) ceil(1.0 / Epsilon);

	//InitNGrid >=2NDim-3 for -Div 1
	/*
	 if (Divide==1 && InitNGrid < 2*NDim-3)//2USC
	 {
	 SmallInitNGrid=True;
	 InitNGrid = 2*NDim-3;
	 }
	 */

	//Calculate grid size. alpha in BnB article
	GridSize = IniLength / InitNGrid;

	if (SmallInitNGrid) {
		Epsilon = 1.0 / InitNGrid; //Update epsilon and grid.

		fprintf(stderr, "---------------------------------------------\n");
		fprintf(stderr, "Warning: Init  Div/edge=%d < 2d-3=%d.\n", InitNGrid, 2 * NDim - 3);
		fprintf(stderr, "Using Epsilon=%f, GridSize=%f\n", Epsilon, GridSize);
		fprintf(stderr, "---------------------------------------------\n");
	}

	//Stopping  criterion: w(S)<=FinalWidth.
	if (Divide == 1)
		FinalWidth = GridSize * (REAL) (NDim - 1);
	if (Divide == 2)
		FinalWidth = GridSize * (REAL) (NDim - 2);

	//Just checking
	fprintf(stderr, "Init Length              = %f.\n", IniLength);
	fprintf(stderr, "Init N. of grid parts    = %d.\n", InitNGrid);
	fprintf(stderr, "Grid Size                = %f.\n", GridSize);
	fprintf(stderr, "FinalWidth               = %f.\n", FinalWidth);
	fprintf(stderr, "Final N. of grid parts   = %d.\n", (INT) ceil(FinalWidth / GridSize));
	fprintf(stderr, "Epsilon                  = %f.\n\n", Epsilon);

	//Graphical Output
	if (ExistArg("-tcl", argc, argv)) {
		//Only x1,x2,x3 neq 0 are used.
		if (ExistArg("-w", argc, argv))
			WWidth = atoi(GetArg("-w", argc, argv));
		else
			WWidth = 400;
		Draw = (UCHAR) atoi(GetArg("-tcl", argc, argv));
		printf("%d\n", WWidth);
		printf("%d\n", WWidth);
		printf("0.0\n");
		printf("IniLength\n"); //TODO: use IniLength, modify XInWindow, YInWindow
		printf("0.0\n");
		printf("IniLength\n");
		printf("%2.4f\n", Epsilon);
		printf("%s\n", "Regular partition");
	} else
		Draw = False;

	//Store Finals
	if (!ExistArg("-ns", argc, argv))
		NoStoreFinalS = False;
	else
		NoStoreFinalS = True;

	//Output file with statistics
	if (!ExistArg("-out", argc, argv))
		OutStat = False;
	else
		OutStat = True;

	//Output command on screen.
	if (ExistArg("-ep", argc, argv))
		sprintf(Execution, "DivSRegOnGrid-1.6_-d_%d_-ep_%f_-Div_%d", NDim, (REAL) atof(GetArg("-ep", argc, argv)), Divide);
	else
		sprintf(Execution, "DivSRegOnGrid-1.6_-d_%d_-g_%d_-Div_%d", NDim, (INT) atoi(GetArg("-g", argc, argv)), Divide);
	fprintf(stderr, "%s\n\n", Execution);

	//-------------------------------------------------------------------------------
	c1 = times(&t1);

	//Generate and store the grid points.------------------------------------------
	PQueue gridPoints = GenGrid(ceil(1 / Epsilon) + 1, NDim);

	PBTV pbtvGridPoints = NULL; //AVL tree of grid points
	pbtvGridPoints = NewBTV(pbtvGridPoints);

	do {
		double* point = Top(gridPoints);

		InsertBTV(pbtvGridPoints, NDim, point, &add);
		if (add == False) {
			fprintf(stderr, "Error, ya insertado\n\n");
			// No existiran dos grid points iguales
		}
		Next(gridPoints);
	} while (End(gridPoints) == 0);

	// Draw GRID POINTS
	if (Draw)
		DrawGridPoints(gridPoints, NDim, WWidth, "Yellow");

	FreeQueue(gridPoints);

	//Get memory for temporal matrices and vectors---------------------------------
	ppVCoorT1 = (PPREAL) GetMem2D((SIZE) NDim, (SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:ppVCorrT1");
	ppVCoorT2 = (PPREAL) GetMem2D((SIZE) NDim, (SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:ppVCorrT2");
	pCentreT = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");

	//Initiate the Data Structures-------------------------------------------------
	plcds = NewListCDS(plcds);
	if (!NoStoreFinalS)
		pbtCDSEnd = NewBTCDS(pbtCDSEnd);

	//AVL of vertices
	pbtv = NewBTV(pbtv);

	//Set the CDS to Vertices transformation matrix
	ppCDSToVMat = (PPREAL) GetMem2D((SIZE) NDim, (SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:ppCDStoVMat");

	//V= d * CDStoVMat + c
	//Vertor is not normalized to 1.
	for (i = 0; i < NDim; i++)
		for (j = 0; j < NDim; j++)
			if (i != j)
				ppCDSToVMat[i][j] = -1.0 / NDim;
			else
				ppCDSToVMat[i][j] = (REAL) (NDim - 1) / NDim;

	//PrintMR(stderr,ppCDSToVMat,NDim,NDim);

	//Set the initial simplex------------------------------------------------------
	for (i = 0; i < NDim; i++)
		pCentreT[i] = IniXi / (REAL) NDim;

	//Radious was not calculated. Let's set it as 1
	pCDS = NewCDSimplex(NDim, pCentreT, IniLength, 1.0, True, False, 1, 0, 1);

	StoreVertexCDSimplex(pCDS, ppVCoorT1, ppCDSToVMat, pbtv, NDim, pbtvGridPoints);

	/*
	 fprintf(stderr,"Initial Simplex:\n");
	 PrintCDSimplex(pCDS,NDim);
	 */

	CountersCDS[0] = 1;
	CountersCDS[1] = 1;
	CountersCDS[2] = 0;
	CountersCDS[3] = 0;

	if (Draw)
		DrawCDSimplex(pCDS, ppVCoorT1, ppCDSToVMat, Draw, NDim, WWidth, "Black");

	// PrintCDSimplex(pCDS,NDim);
	// PrintMR(stderr,ppVCoorT1,NDim,NDim);

	InsertListCDS(plcds, pCDS);

	// Type of USC
	PMap mapUSC = InitMap();
	PMap mapNUSC = InitMap();

	//Main loop--------------------------------------------------------------------
	while (plcds->NElem != 0) {
		pCDS = ExtractListCDS(plcds);

		if(pCDS->NSimplex == 5524)
			PrintCDSimplex(pCDS, NDim, ppVCoorT1, ppCDSToVMat);
/*
		fprintf(stderr, "NSimplex=%llu.\n", pCDS->NSimplex);
		PrintVR(stderr, pCDS->pCentre, NDim);
		fprintf(stderr, "Length=%f.\n", pCDS->L);
		fprintf(stderr, "Radius=%f.\n", pCDS->R);
*/
		pCDS = DivideCDSimplex(Divide, pCDS, ppVCoorT1, ppVCoorT2, pCentreT, ppCDSToVMat, CountersCDS, Fraction, Draw, NDim, WWidth, plcds, pbtCDSEnd,
				FinalWidth, GridSize, InitNGrid, IniLXiRatio, NoStoreFinalS, pbtv, pbtvGridPoints, mapUSC, mapNUSC);
	}

	c2 = times(&t2);
	//-------------------------------------------------------------------------------

	if (OutStat) {
		FOut = OpenFile(GetArg("-out", argc, argv), "a");
		//fprintf(FOut,"%f\t%ld\t%ld\n",Epsilon,CountersCDS[1],CountersCDS[0]-CountersCDS[1]);
		fprintf(FOut, "%f\t%d\n", GridSize, pbtv->MaxNElem);
		fflush(FOut);
		fclose(FOut);
	} else {
		fprintf(stderr, "Time=%f.\n", (REAL) (c2 - c1) / (REAL) sysconf(_SC_CLK_TCK));
		fprintf(stderr, "Number of generated   simplices= %lu.\n", CountersCDS[0]);
		fprintf(stderr, "Number of covered simplices    = %lu.\n", CountersCDS[0] - CountersCDS[1]);
		fprintf(stderr, "%% Cov./Eval.                  = %1.2f\n", (REAL) (CountersCDS[0] - CountersCDS[1]) / (REAL) CountersCDS[1]);
		fprintf(stderr, "Number of final simplices      = %lu.\n", CountersCDS[2]);
		fprintf(stderr, "Number of equal final simplices= %lu.\n", CountersCDS[3]);
		fprintf(stderr, "Number of evaluated simplices  = %lu.\n", CountersCDS[1]);
		fprintf(stderr, "NVertices                      = %d.\n", pbtv->MaxNElem);

		fprintf(stderr, "\nEXIT: -d %d -g %d -ep %f -Div %d -Grid %d -Formula %d -Visited %d -Vertices %d -Simplices %lu\n", NDim, InitNGrid, Epsilon,
				Divide, Count(pbtvGridPoints->pFirstBTVNode, NDim), numberGridPoints(ceil(1 / Epsilon) + 1, NDim),
				CountVisited(pbtvGridPoints->pFirstBTVNode, NDim), pbtv->MaxNElem, CountersCDS[1]);

		fprintf(stderr, "2USC\n");
		ShowMap(mapUSC);
		fprintf(stderr, "2NUSC\n");
		ShowMap(mapNUSC);

		FreeMap(mapUSC);
		FreeMap(mapNUSC);
	}

	if (!NoStoreFinalS)
		pbtCDSEnd = FreeBTCDS(pbtCDSEnd);

	plcds = FreeListCDS(plcds);

	Free2D((PPVOID) ppCDSToVMat, NDim);

	free((PVOID) pCentreT);
	Free2D((PPVOID) ppVCoorT1, NDim);
	Free2D((PPVOID) ppVCoorT2, NDim);

	return 0;
}
