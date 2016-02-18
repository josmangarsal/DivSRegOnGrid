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
                   PULINT pCountersCDS, REAL RedFact, UCHAR Draw,
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
                 pCentreT[k]=pCDS_O->pCentre[k]-(1.0-RedFact)*pCDS_O->R/NDim;
              else
                 pCentreT[k]=pCDS_O->pCentre[k]+(1.0-RedFact)*pCDS_O->R/NDim;
             }
        if (Up)
	   pCentreT[j]+=pCDS_O->R*(1.0-RedFact);
        else
           pCentreT[j]-=pCDS_O->R*(1.0-RedFact);

      NSimplex=pCDS_O->NSimplex*NSons+1+j;

      if (IsCovered) //Previous simplex was covered. Let's use its memory.
         UpdateCDSimplex (pCDS_T, NDim, pCentreT, RedFact*pCDS_O->L,
                          RedFact*pCDS_O->R, Up, True, NSimplex, DivPhase);
      else
         pCDS_T= NewCDSimplex(NDim, pCentreT, RedFact*pCDS_O->L,
                              RedFact*pCDS_O->R, Up, True,
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
	  StoreVertexCDSimplex(pCDS_T, ppVCoor_T, ppCDSToVMat, pbtv, NDim,
	                       pbtvGridPoints);

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
                    PULINT pCountersCDS, REAL RedFact, UCHAR Draw,
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


 if (LT ( RedFact, (REAL)(NDim-1)/(REAL) NDim) )
    {
     UpDownRatio=(NDim*(1.0-RedFact)-1.0);
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
     StoreVertexCDSimplex(pCDS_T, ppVCoor_T, ppCDSToVMat, pbtv, NDim,  
                          pbtvGridPoints);

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
                 pCountersCDS, RedFact, Draw, NDim, NSons, WWidth,
                 plcds, pbtCDSEnd, FinalWidth, NoStoreFinalS, pbtv, 
                 pbtvGridPoints);
 return NULL;
}




/*---------------------------------------------------------------------------*/
void CheckIncFact(REAL IncFact, REAL NewIncFact, INT CurrNGrid, INT NewNGrid)
{
 if (GT(NewIncFact, IncFact))
    {
     fprintf(stderr," NewIncFact=%f < IncFact=%f.\n",
             NewIncFact, IncFact);
             exit(1);
    }

 if (LE(NewIncFact, 1.0))
    {
     fprintf(stderr,"IncFact           = %f \n"   ,IncFact);
     fprintf(stderr,"IncFact*CurrNGrid = %1.21f\n",IncFact*CurrNGrid);
     fprintf(stderr,"NewNGrid          = %d \n"   ,NewNGrid);
     fprintf(stderr,"NewIncFact<= 1.0  = %f\n"    ,NewIncFact);
     exit(1);
    }
}

/*---------------------------------------------------------------------------*/
/*Next increasing factor*/
void NextIF (INT CurrNGrid, REAL IncFact, 
             PINT pNewNGrid, PREAL pNewIncFact)
{
 /*Requires:
 CurrNGrid : Current number of grids in the width
 IncFact   : maximum increasing factor*/
 /*returns
 pNewNGrid : number of grids in the new width  
 NewIncFact: Increasing factor used to get the new width. */
 
 INT  NewNGrid; 	//Number of Grids for the new Width
 REAL NewIncFact;	//New Increasing Factor
 
     
 //New number of grids    
  if ( EQ (IncFact*(REAL)CurrNGrid, round(IncFact*(REAL)CurrNGrid) ) )
     NewNGrid  = round(IncFact*(REAL)CurrNGrid);
 else
     NewNGrid  = floor(IncFact*(REAL)CurrNGrid);
              
 NewIncFact=(REAL)NewNGrid/(REAL)CurrNGrid;

// fprintf(stderr,"CurrNGrid=%d, NewNGrid=%d, NewRedFact=%f.\n\n", 
//                 CurrNGrid, NewNGrid,NewRedFact);

 CheckIncFact(IncFact, NewIncFact, CurrNGrid, NewNGrid);
    
 *pNewNGrid    = NewNGrid;
 *pNewIncFact = NewIncFact;    
}

/*---------------------------------------------------------------------------*/
/*Calculate the Reduction factors backwards and determine the first RedFact  */
REAL FirstRF2USC(INT NDim, INT InitNGrid)
{
 REAL IncFact; 		//Incrementing factor.
 INT  NIterIF;		//Number of iterations using increasing factor.
 INT  CurrNGrid;	//Current number of grid parts.
 INT  PrevNGrid;	//Previous number of grid parts.
 INT  NewNGrid; 	//Number of Grids for the new Width.
 REAL NewIncFact; 	//IncFact to get NewNGrid from CurrNGrid.
 
 IncFact   = (REAL)NDim / ((REAL) NDim - 1.0);
 CurrNGrid = PrevNGrid= NDim-1; //Number of grid parts in a final simplex.
 NIterIF   = 1;
 
 while (CurrNGrid<InitNGrid)
       {
        NextIF (CurrNGrid, IncFact, &NewNGrid, &NewIncFact);
        //fprintf(stderr,"NIterIF=%3d, CurrNGrid =%3d, NewIncFact=%f.\n",
        //        NIterIF,CurrNGrid,NewIncFact);
        NIterIF++;
        PrevNGrid=CurrNGrid;
        CurrNGrid=NewNGrid;               
       }  
 //fprintf(stderr,"------------------------------------------------\n");          
 //fprintf(stderr,"NIterIF=%3d, CurrNGrid =%3d.\n\n\n",NIterIF-1,CurrNGrid); 
 
 fprintf(stderr,"Initial Reduction Factor = %d /%d.\n", PrevNGrid, InitNGrid);
 return (REAL)PrevNGrid/(REAL)InitNGrid;       
}


/*---------------------------------------------------------------------------*/
void CheckRedFact(REAL RedFact, REAL NewRedFact, INT CurrNGrid, INT NewNGrid)
{
 if (LT(NewRedFact, RedFact))
    {
     fprintf(stderr," NewRedFact=%f < RedFact=%f.\n",
             NewRedFact, RedFact);
             exit(1);
    }

 if (GE(NewRedFact, 1.0))
    {
     fprintf(stderr,"RedFact           = %f \n",   RedFact);
     fprintf(stderr,"RedFact*CurrNGrid = %1.21f\n",RedFact*CurrNGrid);
     fprintf(stderr,"NewNGrid           = %d \n"  ,NewNGrid);
     fprintf(stderr,"NewRedFact>= 1.0  = %f\n"    ,NewRedFact);
     exit(1);
    }
}

/*---------------------------------------------------------------------------*/
/*Next reduction factor*/
void NextRF (PCDSIMPLEX pCDS, REAL GridSize, REAL RedFact, 
             PINT pNewNGrid, PREAL pNewRedFact)
{
 /*Requires:
 pCDS = A regular simplex.
 GridSize,
 RedFact=minimum reduction RedFact
 GridSize: 1/g */
 /*returns
 pNewNGrid: number of grids in the new width  
 NewRedFact: RedFact used to get the new width. */
 
 INT  CurrNGrid; 	//Current number of grids in the width
 INT  NewNGrid; 	//Number of Grids for the new Width
 REAL NewRedFact;	//New Reduction Factor.
 
 //Current number of grids.
 if (EQ(pCDS->L/GridSize,round(pCDS->L/GridSize)))
     CurrNGrid  = round(pCDS->L/GridSize);
 else
     CurrNGrid  = ceil(pCDS->L/GridSize);
 
 //New number of grids
 if (EQ(RedFact*(REAL)CurrNGrid,round(RedFact*(REAL)CurrNGrid)))
     NewNGrid  = round(RedFact*(REAL)CurrNGrid);
 else
     NewNGrid  = ceil(RedFact*(REAL)CurrNGrid);
 
 //New reduction facot             
 NewRedFact=(REAL)NewNGrid/(REAL)CurrNGrid;

// fprintf(stderr,"CurrNGrid=%d, NewNGrid=%d, NewRedFact=%f.\n\n", 
//                 CurrNGrid, NewNGrid, NewRedFact);

 CheckRedFact(RedFact, NewRedFact, CurrNGrid, NewNGrid);
    
 *pNewNGrid    = NewNGrid;
 *pNewRedFact = NewRedFact;    
}

/*---------------------------------------------------------------------------*/
PCDSIMPLEX DivideCDSimplex (UCHAR Divide, PCDSIMPLEX pCDS_O, PPREAL ppVCoor_O,
                            PPREAL ppVCoor_T, PREAL pCentreT, PPREAL ppCDSToVMat,
                            PULINT pCountersCDS, REAL RedFact, UCHAR Draw,
                            INT NDim, INT WWidth,
                            PLISTCDS plcds, PBTCDS pbtCDSEnd,
                            REAL FinalWidth, REAL GridSize, INT InitNGrid, 
                            REAL IniLXiRatio, BOOL NoStoreFinalS, PBTV pbtv, 
                            PBTV pbtvGridPoints)
{
 INT  NewNGrid;		//Number of Grids for the new Width
 REAL NewRedFact;	//New reduction factor

 switch (Divide)
        {
         case 1:  //2USC Grid
/*
              if (pCDS_O->NSimplex==1) //The same result.
                 NewRedFact=FirstRF2USC(NDim,InitNGrid); 
              else
*/
                 NextRF(pCDS_O, GridSize, RedFact, &NewNGrid, &NewRedFact);    

              pCDS_O = TwoUSC (pCDS_O, ppVCoor_O,
                               ppVCoor_T, pCentreT, ppCDSToVMat,
                               pCountersCDS, NewRedFact, Draw,
                               NDim, NDim, WWidth, plcds, pbtCDSEnd,
                               FinalWidth, NoStoreFinalS,pbtv,pbtvGridPoints);
              break;
         case 2:  //2NUSC Grid
              if (EQ(RedFact*pCDS_O->L, GridSize))
                 NewNGrid=1;
              else
                 NextRF(pCDS_O, GridSize, RedFact, &NewNGrid, &NewRedFact);

              pCDS_O = TwoNUSC (pCDS_O, ppVCoor_O,
                                ppVCoor_T, pCentreT, ppCDSToVMat,
                                pCountersCDS, NewRedFact, Draw,
                                NDim, WWidth, plcds, pbtCDSEnd,
                                FinalWidth, IniLXiRatio,
                                True, GridSize, NoStoreFinalS,pbtv,
                                pbtvGridPoints);
              break;

         default:
                 fprintf(stderr,"DivideSimplex, Divide=%d.\n",Divide);
	         exit(1);
       }
 return pCDS_O;
}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
