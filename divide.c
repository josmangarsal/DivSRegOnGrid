/******************************************************************************
 			divide.c  - description
			----------------------
	begin		: August 2014
	copywirght	: (C) 2014 by L.G.Casado.
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "getmem.h"
#include "utils.h"
#include "btvertex.h"
#include "CDSimplex.h"
#include "listCDSimplex.h"
#include "btCDSimplex.h"
#include "divide.h"

/*---------------------------------------------------------------------------*/
/*2USC.									     */
PCDSIMPLEX TwoUSC (PCDSIMPLEX pCDS_O, PPREAL ppVCoor_O,
                   PPREAL ppVCoor_T, PREAL pCentreT, PPREAL ppCDSToVMat,
                   PULINT pCountersCDS, REAL Fraction, UCHAR Draw,
                   INT NDim, INT NSons, INT WWidth,
                   PLISTCDS plcds, PBTCDS pbtCDSEnd,
                   REAL FinalWidth,BOOL NoStoreFinalS,
		               PBTV pbtv, PBTV pbtvGridPoints)
{
 //ppVCoor_O and _T are Vertex matrix allocated in main for origin and target.
 //To reduce memory allocations
 //pCentre can be used as ppVCoor[0]

 INT k; /*Vertex Index*/
 INT j; /*Original Vertex Index*/
 ULLINT NSimplex;
 BOOL IsCovered=False;
 PCDSIMPLEX pCDS_T;     //New Simplex
 UCHAR DivPhase;
 BOOL Add; 		//Final simplex is added to the btCDS.
 BOOL Up;		//True = Normal Orientation


 DivPhase= (pCDS_O->DivPhase + 1) % 2;
 Up =  pCDS_O->Up;

/* if (Draw)
    {
     DelCDSimplex(pCDS_O,1);
     DrawCDSimplex (pCDS_O,ppVCoor_O,ppCDSToVMat,Draw,NDim,WWidth,"Green");
     DelCDSimplex(pCDS_O,1);
    }
*/

 for (j=0;j<NDim;j++) //Number of new simplices
     {
      //Generate the new center
      for (k=0;k<NDim;k++)
             {
              if (Up)
                 pCentreT[k]=pCDS_O->pCentre[k]-(1.0-Fraction)*pCDS_O->R/NDim;
              else
                 pCentreT[k]=pCDS_O->pCentre[k]+(1.0-Fraction)*pCDS_O->R/NDim;
             }
        if (Up)
	   pCentreT[j]+=pCDS_O->R*(1.0-Fraction);
        else
           pCentreT[j]-=pCDS_O->R*(1.0-Fraction);

      NSimplex=pCDS_O->NSimplex*NSons+1+j;

      if (IsCovered) //Previous simplex was covered. Let's use its memory.
         UpdateCDSimplex (pCDS_T, NDim, pCentreT, Fraction*pCDS_O->L,
                          Fraction*pCDS_O->R, Up, True, NSimplex, DivPhase);
      else
         pCDS_T= NewCDSimplex(NDim, pCentreT, Fraction*pCDS_O->L,
                              Fraction*pCDS_O->R, Up, True,
                              NSimplex, DivPhase);

     // PrintCDSimplex(pCDS_T,NDim);
     // PrintMR(stderr,ppVCoor_T,NDim,NDim);
     // fprintf(stderr,"\n\n");

      IsCovered= IsCDSCovered (plcds, pCDS_T, NDim);

      if (IsCovered)
         {
          if (Draw)
             {
              DrawCDSimplex (pCDS_T,ppVCoor_T,ppCDSToVMat,
                             Draw,NDim,WWidth,"Red");
              DelCDSimplex (pCDS_T, Draw);
             }
         }
      else
         {
          pCountersCDS[1]++; //Number of Evaluated
          //Store Vertices
	  StoreVertexCDSimplex(pCDS_T, ppVCoor_T, ppCDSToVMat, pbtv, NDim, pbtvGridPoints);

          if (LE(pCDS_T->L,FinalWidth))
             {
              pCountersCDS[2]++; //Number of Finals
              if (Draw)
                 DrawCDSimplex (pCDS_T,ppVCoor_T,ppCDSToVMat,
                                Draw,NDim,WWidth,"Black");
              if (!NoStoreFinalS)
                 {
                  InsertBTCDS(pbtCDSEnd, pCDS_T, NDim, &Add);
                  if (!Add)
                     pCountersCDS[3]++;
                  pCDS_T=NULL;
                 }
              else
                 pCDS_T=FreeCDSimplex(pCDS_T); //NEW CHECKKKKK
             }
          else
             {
              if (Draw)
                 DrawCDSimplex (pCDS_T,ppVCoor_T,ppCDSToVMat,
                                Draw,NDim,WWidth,"Black");
              InsertListCDS(plcds, pCDS_T);
              pCDS_T=NULL;
             }
         }
     }

 pCountersCDS[0]+=NDim; //Number of generated
 if (IsCovered)
    pCDS_T=FreeCDSimplex(pCDS_T);

 pCDS_O=FreeCDSimplex(pCDS_O);

 return NULL;
}

/*---------------------------------------------------------------------------*/
/*2NUSC.							             */
/*OnGrid = True for checking 2NUSC-Grid                                      */
PCDSIMPLEX TwoNUSC (PCDSIMPLEX pCDS_O, PPREAL ppVCoor_O,
                    PPREAL ppVCoor_T, PREAL pCentreT, PPREAL ppCDSToVMat,
                    PULINT pCountersCDS, REAL Fraction, UCHAR Draw,
                    INT NDim, INT WWidth,
                    PLISTCDS plcds, PBTCDS pbtCDSEnd,
                    REAL FinalWidth, REAL IniLXiRatio,
                    BOOL OnGrid, REAL GridSize, BOOL NoStoreFinalS,
                    PBTV pbtv, PBTV pbtvGridPoints)
{
 REAL UpDownRatio;
 PCDSIMPLEX pCDS_T;
 BOOL Up=True;
 ULLINT NSimplex;
 BOOL Add; 		//Final simplex is added to the btCDS.
 INT NSons;
 //INT i,j;

 if (pCDS_O->Up)
    Up=False;

 //First the smaller and centered one. 2USC free the Original Simplex.
 NSons=NDim+1;
 NSimplex=pCDS_O->NSimplex*NSons+NSons;


 if (LT ( Fraction, (REAL)(NDim-1)/(REAL) NDim) )
    {
     UpDownRatio=(NDim*(1.0-Fraction)-1.0);
     pCDS_T= NewCDSimplex(NDim, pCDS_O->pCentre,UpDownRatio*pCDS_O->L,
                	  UpDownRatio*pCDS_O->R, Up, False,
                	  NSimplex, pCDS_O->DivPhase);

    //Check on grid
/*    if (OnGrid)
       {
        CDSToV (NDim, pCDS_T, ppVCoor_T, ppCDSToVMat);
        for (i=0;i<NDim;i++)
            for (j=0;j<NDim;j++)
                 if ( !EQ (        ppVCoor_T[i][j]*IniLXiRatio/GridSize,
                           round ( ppVCoor_T[i][j]*IniLXiRatio/GridSize )
                          )
                    )
                    {
                     fprintf(stderr,"2NUSC-Grid: center simplex not on grid.\n");
                     PrintCDSimplex(pCDS_T,NDim);
                     exit(1);
                    }
                 else
                     fprintf(stderr,"2NUSC-Grid: center simplex on grid.\n");

       }
*/

     pCountersCDS[0]++; //Number of Generated
     pCountersCDS[1]++; //Number of Evaluated

     //Store Vertices
     StoreVertexCDSimplex(pCDS_T, ppVCoor_T, ppCDSToVMat, pbtv, NDim, pbtvGridPoints);

     if (LE(pCDS_T->L,FinalWidth))
	{
	 pCountersCDS[2]++; //Number of Finals
	 if (Draw)
            DrawCDSimplex (pCDS_T,ppVCoor_T,ppCDSToVMat,
                	   Draw,NDim,WWidth,"Yellow");

	 if (!NoStoreFinalS)
            {
             //Add = 0 when there exist a final with the same center
             InsertBTCDS(pbtCDSEnd, pCDS_T, NDim, &Add);
             if (!Add)
        	{
        	 pCountersCDS[3]++;
        	 if (Draw)
                    DrawCDSimplex (pCDS_T,ppVCoor_T,ppCDSToVMat,
                        	   Draw,NDim,WWidth,"Green");
        	}
             pCDS_T=NULL;
            }
         else
            pCDS_T=FreeCDSimplex(pCDS_T); //NEW CHECKKKKK
	 }
     else
	{
	 if (Draw)
            DrawCDSimplex (pCDS_T,ppVCoor_T,ppCDSToVMat,
                	   Draw,NDim,WWidth,"Yellow");

	 InsertListCDS(plcds, pCDS_T);
	 pCDS_T=NULL;
	}
    }

 pCDS_O= TwoUSC (pCDS_O, ppVCoor_O, ppVCoor_T, pCentreT, ppCDSToVMat,
                 pCountersCDS, Fraction, Draw, NDim, NSons, WWidth,
                 plcds, pbtCDSEnd, FinalWidth, NoStoreFinalS, pbtv, pbtvGridPoints);
 return NULL;
}

/*---------------------------------------------------------------------------*/
PCDSIMPLEX DivideCDSimplex (UCHAR Divide, PCDSIMPLEX pCDS_O, PPREAL ppVCoor_O,
                            PPREAL ppVCoor_T, PREAL pCentreT, PPREAL ppCDSToVMat,
                            PULINT pCountersCDS, REAL Fraction, UCHAR Draw,
                            INT NDim, INT WWidth,
                            PLISTCDS plcds, PBTCDS pbtCDSEnd,
                            REAL FinalWidth, REAL GridSize, REAL IniLXiRatio,
                            BOOL NoStoreFinalS, PBTV pbtv, PBTV pbtvGridPoints)
{
 REAL NewFraction;
 INT NGNSE; //Number of Grids for the New Simplex Edge

 switch (Divide)
        {
         case 1:  //2USC Grid
              if ( EQ( (Fraction*pCDS_O->L)/GridSize,
                       round((Fraction*pCDS_O->L)/GridSize)
                     )
                 )
                  NGNSE  = round((Fraction*pCDS_O->L)/GridSize);
              else
                  NGNSE = ceil((Fraction*pCDS_O->L)/GridSize);
              NewFraction=((REAL)NGNSE*GridSize)/pCDS_O->L;

              //fprintf(stderr,"Size simplex=%f.\n",pCDS_O->L);
              //fprintf(stderr,"NGNSE=%d, NewFraction=%f.\n\n", NGNSE,NewFraction);

              if (LT(NewFraction, Fraction))
                 {
                  fprintf(stderr," NewFraction=%f < Fraction=%f.\n",
                                   NewFraction, Fraction);
                  exit(1);
                 }

              if (GE(NewFraction, 1.0))
                 {
                  PrintCDSimplex(pCDS_O,NDim, ppVCoor_T, ppCDSToVMat);
                  fprintf(stderr,"Fraction   =%f \n",Fraction);
                  fprintf(stderr,"(Fraction*pCDS_O->L)/GridSize = %1.21f\n",
                                  (Fraction*pCDS_O->L)/GridSize);
                  fprintf(stderr,"NGNSE      =%d, \n", NGNSE);
                  fprintf(stderr,"NewFraction=%f >= 1.0\n",NewFraction);

                  fprintf(stderr,"GridSize   =alpha_f=%1.21f, \n", GridSize);
                  fprintf(stderr,"FinalWidth    =%1.21f, \n", FinalWidth);
                  exit(1);
                 }

              pCDS_O = TwoUSC (pCDS_O, ppVCoor_O,
                               ppVCoor_T, pCentreT, ppCDSToVMat,
                               pCountersCDS, NewFraction, Draw,
                               NDim, NDim, WWidth, plcds, pbtCDSEnd,
                               FinalWidth, NoStoreFinalS,pbtv,pbtvGridPoints);
              break;
         case 2:  //2NUSC Grid
              if (EQ(Fraction*pCDS_O->L, GridSize))
                 NGNSE=1;
              else
                 {
                  if ( EQ(      (Fraction*pCDS_O->L)/GridSize,
                           round((Fraction*pCDS_O->L)/GridSize)
                         )
                     )
                      NGNSE  = round((Fraction*pCDS_O->L)/GridSize);
                  else
                     NGNSE = ceil((Fraction*pCDS_O->L)/GridSize);
                 }

              NewFraction=((REAL)NGNSE*GridSize)/pCDS_O->L;

              //fprintf(stderr,"NGNSE=%d, NewFraction=%f.\n", NGNSE,NewFraction);

              if (LT(NewFraction, Fraction))
                 {
                  fprintf(stderr," NewFraction=%f < Fraction=%f.\n",
                                   NewFraction, Fraction);
                  exit(1);
                 }

              if (GE(NewFraction, 1.0))
                 {
                  PrintCDSimplex(pCDS_O,NDim,ppVCoor_T, ppCDSToVMat);
                  fprintf(stderr,"Fraction=%f \n",Fraction);
                  fprintf(stderr,"(Fraction*pCDS_O->L)/GridSize=%1.20f, \n",
                                  (Fraction*pCDS_O->L)/GridSize);
                  fprintf(stderr,"NGNSE=%d, \n", NGNSE);
                  fprintf(stderr,"NewFraction=%f >= 1.0\n",NewFraction);
                  fprintf(stderr,"GridSize=alhpa_f=%1.21f, \n", GridSize);
                  fprintf(stderr,"FinalWidth=%1.21f, \n", FinalWidth);


                  exit(1);
                 }

              pCDS_O = TwoNUSC (pCDS_O, ppVCoor_O,
                                ppVCoor_T, pCentreT, ppCDSToVMat,
                                pCountersCDS, NewFraction, Draw,
                                NDim, WWidth, plcds, pbtCDSEnd,
                                FinalWidth, IniLXiRatio,
                                True, GridSize, NoStoreFinalS,pbtv,pbtvGridPoints);
              break;

         default:
                 fprintf(stderr,"DivideSimplex, Divide=%d.\n",Divide);
	         exit(1);
       }
 return pCDS_O;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
