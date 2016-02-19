/******************************************************************************
			btvertex.c  - description
			----------------------
	begin		: May 2004
	copywirght	: (C) 2004 by L.G.Casado.

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#include "getmem.h"
#include "utils.h"
#include "btvertex.h"




/*---------------------------------------------------------------------------*/
PBTVNODE NewBTVNODE(INT NDim, PREAL pCoor)
{
 PBTVNODE pBTVNode;

 pBTVNode          = GetMem(1,sizeof(BTVNODE),"NewBTVNODE");
 pBTVNode->pV      = (PREAL)  GetMem((SIZE)NDim, (SIZE)sizeof(REAL),
                      "NewBTVNODE->pV");
 CopyVR(pBTVNode->pV,pCoor,NDim);
 pBTVNode->Balance = EQUAL;
 pBTVNode->pleft   = NULL;
 pBTVNode->pright  = NULL;
 pBTVNode->visited = False;
 return pBTVNode;
}

/*---------------------------------------------------------------------------*/
PBTVNODE FreeBTVNode(PBTVNODE pBTVNode)
{
 if (pBTVNode==NULL)
    {
     fprintf(stderr,"TRying to free a NULL BTVNODE pointer.\n");
     exit(1);
    }
 free( (PVOID) pBTVNode->pV);
 free((PVOID)pBTVNode);
 pBTVNode=NULL;
 return pBTVNode;
}

/*----------------------------------------------------------------------------*/
/*Swap the nodes Vertexes, but the structure of the BLTree does not change.   */
VOID SwapBTVNode(PBTVNODE pBTVNodeO, PBTVNODE pBTVNodeT)
{
 PREAL pV;

 pV            = pBTVNodeO->pV;
 pBTVNodeO->pV = pBTVNodeT->pV;
 pBTVNodeT->pV = pV;
}

/*----------------------------------------------------------------------------*/
/*Swap the data and copy the Balance and pointers of the BTVNodes.            */
/*Node2 will be removed with data of node1 after exchange.                    */
/*----------------------------------------------------------------------------*/
VOID ExchangeBTVNode(PBTVNODE node1, PBTVNODE node2)
{
 SwapBTVNode(node1,node2);
 node1->Balance = node2->Balance;
 node1->pleft   = node2->pleft;
 node1->pright  = node2->pright;

 node2->pleft   = NULL;
 node2->pright  = NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PBTV NewBTV(PBTV pBTV)
{
 pBTV                 = GetMem(1,sizeof(BTV),"NewBTV");
 pBTV->NElem          = 0;
 pBTV->MaxNElem       = 0;
 pBTV->pFirstBTVNode  = NULL;
 return pBTV;
}

/*----------------------------------------------------------------------------*/
void FreeBTVSubTree(PBTVNODE root)
{
 if (root!=NULL)
    {
     FreeBTVSubTree(root->pleft);
     FreeBTVSubTree(root->pright);
     root=FreeBTVNode(root);
    }
}

/*----------------------------------------------------------------------------*/
PBTV FreeBTV(PBTV pbtv)
{
 if (pbtv!=NULL)
    {
     FreeBTVSubTree(pbtv->pFirstBTVNode);
     pbtv->pFirstBTVNode=NULL;
     free((PVOID)pbtv);
     pbtv=NULL;
    }
 return NULL;
}


/*----------------------------------------------------------------------------*/
/*Rotate left a binary Tree                                                   */
/*----------------------------------------------------------------------------*/
PBTVNODE RotateBTVLeft(PBTVNODE root)
{
 PBTVNODE Temp;

 if (root==NULL)
    {
     fprintf(stderr,"BTVertex::RotateBTVLeft :");;
     fprintf(stderr," Can not rotate to left an empty tree \n");
     exit(1);
    }
 else
    if (root->pright==NULL)
       {
        fprintf(stderr,"BTVertex::RotateBTVLeft : Can not rotate to left\n");
        exit(1);
       }
    else
       {
        Temp         = root->pright;
        root->pright = Temp->pleft;
        Temp->pleft  = root;
        root=Temp;
       }
 return Temp;
}
/*----------------------------------------------------------------------------*/
/*Rotate right a binary Tree                                                  */
/*----------------------------------------------------------------------------*/
PBTVNODE RotateBTVRight(PBTVNODE root)
{
 PBTVNODE Temp;

 if (root==NULL)
    {
     fprintf(stderr,"BTVertex::RotateBTVRight :");
     fprintf(stderr," Can not rotate to right an empty tree \n");
     exit(1);
    }
 else
    if (root->pleft==NULL)
       {
        fprintf(stderr,"BTVertex::RotateBTVRight : Can not rotate to right\n");
        exit(1);
       }
    else
       {
        Temp         = root->pleft;
        root->pleft  = Temp->pright;
        Temp->pright = root;
        root=Temp;
       }
 return Temp;
}

/*----------------------------------------------------------------------------*/
/*Insert in the left branch maintaining the global balance. Taller noticed if */
/*the insertion has increased the heigh or not, so the balance factors can be */
/*changed appropiately and rotations may be done. Taller has efects only as   */
/* return value. We initiate it to false.                                     */
/*----------------------------------------------------------------------------*/
PBTVNODE LeftInsertBTVBalance(PBTVNODE root, PBOOL ptaller)
{
 PBTVNODE Left, LeftRight;

 Left=root->pleft;
 switch (Left->Balance)
        {
         case LEFT:
              root->Balance=EQUAL;
              Left->Balance=EQUAL;
              root=RotateBTVRight(root);
              *ptaller=False;
              break;
         case EQUAL:
              fprintf(stderr,"BTVertex:LeftInsertBalance :");
	      fprintf(stderr," The tree is already balanced\n");
              exit(1);
              break;
         case RIGHT:
              LeftRight=Left->pright;
              switch (LeftRight->Balance)
                     {
                      case LEFT:
                           root->Balance=RIGHT;
                           Left->Balance=EQUAL;
                           break;
                      case EQUAL:
                           root->Balance=EQUAL;
                           Left->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=EQUAL;
                           Left->Balance=LEFT;
                           break;
                     }
              LeftRight->Balance=EQUAL;
              root->pleft=RotateBTVLeft(root->pleft);
              root=RotateBTVRight(root);
              *ptaller=False;
              break;
        }
 return root;
}

/*---------------------------------------------------------------------------*/
/*Analogous to previous one but on the right.                                */
/*---------------------------------------------------------------------------*/
PBTVNODE RightInsertBTVBalance(PBTVNODE root, PBOOL ptaller)
{
 PBTVNODE Right, RightLeft;

 Right=root->pright;
 switch (Right->Balance)
        {
         case LEFT:
              RightLeft=Right->pleft;
              switch (RightLeft->Balance)
                     {
                      case LEFT:
                           root->Balance=EQUAL;
                           Right->Balance=RIGHT;
                           break;
                      case EQUAL:
                           root->Balance=EQUAL;
                           Right->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=LEFT;
                           Right->Balance=EQUAL;
                           break;
                     }
              RightLeft->Balance=EQUAL;
              root->pright=RotateBTVRight(root->pright);
              root=RotateBTVLeft(root);
              *ptaller=False;
              break;
         case EQUAL:
              fprintf(stderr,"BTVertex:RightInsertBTVBalance: ");
	      fprintf(stderr,"The tree is already balanced\n");
              exit(1);
              break;
         case RIGHT:
              root->Balance=EQUAL;
              Right->Balance=EQUAL;
              root=RotateBTVLeft(root);
              *ptaller=False;
              break;
        }
 return root;
}


/*---------------------------------------------------------------------------*/
/*Do the actual insertion in the BLVertex.                                   */
/*If exists, return the pointer to the vertex.                               */
/*---------------------------------------------------------------------------*/
PBTVNODE InsertBTV1 (PBTVNODE root, INT NDim, PREAL pCoor,
                     PBOOL ptaller, PBOOL pAdd)
{
 COMPARE CompCoor;

 if (root==NULL)
    {
     root     = NewBTVNODE(NDim,pCoor);
     *ptaller = True;
     *pAdd    = True;
    }
 else
    {
     CompCoor=CompareVR(pCoor,root->pV,NDim);
     if (CompCoor!=Equal)
        {
         if (CompCoor==Smaller)
            {
             root->pleft=InsertBTV1(root->pleft,NDim,pCoor,
	                            ptaller,pAdd);
             if (*ptaller)
                switch (root->Balance)
                       {
                        case LEFT:
                                 root=LeftInsertBTVBalance(root,ptaller);
                                 break;
                        case EQUAL:
                                 root->Balance=LEFT;
                                 break;
                        case RIGHT:
                                 root->Balance=EQUAL;
                                 *ptaller=False;
                                 break;
                       }
            }
         else
            {
             root->pright=InsertBTV1(root->pright,NDim,pCoor,
	                             ptaller,pAdd);
             if (*ptaller)
                switch (root->Balance)
                       {
                        case LEFT:
                                 root->Balance=EQUAL;
                                 *ptaller=False;
                                 break;
                        case EQUAL:
                                 root->Balance=RIGHT;
                                 break;
                        case RIGHT:
                                 root=RightInsertBTVBalance(root,ptaller);
                                 break;
                       }
            }
        }
    }
 return root;
}

/*---------------------------------------------------------------------------*/
VOID InsertBTV(PBTV pbtv, INT NDim, PREAL pCoor, PBOOL pAdd)
{
 BOOL taller = False;


 *pAdd=False;
 pbtv->pFirstBTVNode=InsertBTV1(pbtv->pFirstBTVNode,NDim,pCoor,
                                &taller,pAdd);
 if (*pAdd==True)
    {
     pbtv->NElem++;
     if (pbtv->NElem > pbtv->MaxNElem)
 	    pbtv->MaxNElem=pbtv->NElem;
    }
/* else
     fprintf(stderr,"Vertex already exists. Shared by %d Simplexes.\n",
            (*ppV)->NSimplex);
*/
}

/*---------------------------------------------------------------------------*/
/*Update the balance factors and do the necesary rotations when a left node  */
/*is removed.                                                                */
/*---------------------------------------------------------------------------*/
PBTVNODE LeftDelBTVBalance (PBTVNODE root, PBOOL pshorter)
{
 PBTVNODE Left, LeftRight;

 Left=root->pleft;
 switch (Left->Balance)
        {
         case LEFT:
              root->Balance=Left->Balance=EQUAL;
              root=RotateBTVRight(root);
              break;
         case EQUAL:
              root->Balance=LEFT;
              Left->Balance=RIGHT;
              root=RotateBTVRight(root);
              *pshorter=False;
              break;
         case RIGHT:
              LeftRight=Left->pright;
              switch (LeftRight->Balance)
                     {
                      case LEFT:
                           root->Balance=RIGHT;
                           Left->Balance=EQUAL;
                           break;
                      case EQUAL:
                           root->Balance=Left->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=EQUAL;
                           Left->Balance=LEFT;
                           break;
                     }
              LeftRight->Balance=EQUAL;
              root->pleft=RotateBTVLeft(root->pleft);
              root=RotateBTVRight(root);
              break;
        }
 return root;
}

/*---------------------------------------------------------------------------*/
/*Analogous to previous one but on the right.                                */
/*---------------------------------------------------------------------------*/
PBTVNODE RightDelBTVBalance (PBTVNODE root, PBOOL pshorter)
{
 PBTVNODE Right, RightLeft;

 Right=root->pright;
 switch (Right->Balance)
        {
         case LEFT:
              RightLeft=Right->pleft;
              switch (RightLeft->Balance)
                     {
                      case LEFT:
                           root->Balance=EQUAL;
                           Right->Balance=RIGHT;
                           break;
                      case EQUAL:
                           root->Balance=Right->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=LEFT;
                           Right->Balance=EQUAL;
                           break;
                     }
              RightLeft->Balance=EQUAL;
              root->pright=RotateBTVRight(root->pright);
              root=RotateBTVLeft(root);
              break;
         case EQUAL:
              root->Balance=RIGHT;
              Right->Balance=LEFT;
              root=RotateBTVLeft(root);
              *pshorter=False;
              break;
         case RIGHT:
              root->Balance=Right->Balance=EQUAL;
              root=RotateBTVLeft(root);
              break;
        }
 return root;
}

/*---------------------------------------------------------------------------*/
/*Delete the Vertex with the same Coordinates when it does not belong to a   */ /*Simplex.                                                                   */
/*The variable shorter is initially True. When it becomes False, no further  */
/*changes are needed. nDeleted return the number of deleted boxes.           */
/*---------------------------------------------------------------------------*/
PBTVNODE FreeBTV1(PBTVNODE root, INT NDim, PREAL pCoor, PBOOL pshorter)
{
 PBTVNODE Temp;
 COMPARE CompCoor;

 if (root==NULL)
    {
     fprintf(stderr,"DelNode: Empty BLTVertex\n");
     exit(1);
    }
 else
    {
     CompCoor=CompareVR(pCoor,root->pV,NDim);
     if (CompCoor==Equal)
	if (root->pleft==NULL && root->pright==NULL)
           {
	    root=FreeBTVNode(root);
            *pshorter=True;
           }
	else
           if (root->pleft==NULL)
              {
               Temp=root->pright;
               ExchangeBTVNode(root,Temp);
               *pshorter=True;
               Temp=FreeBTVNode(Temp);
              }
           else
              if (root->pright==NULL)
                 {
                  Temp=root->pleft;
                  ExchangeBTVNode(root,Temp);
                  *pshorter=True;
                  Temp=FreeBTVNode(Temp);
                 }
              else
                 {
                  for (Temp=root->pright;Temp->pleft!=NULL;
		       Temp=Temp->pleft);
                  SwapBTVNode(root,Temp);
                  root->pright=FreeBTV1(root->pright, NDim, pCoor, pshorter);
                  if (*pshorter)
                     switch (root->Balance)
                            {
                             case LEFT:
                                  root=LeftDelBTVBalance(root, pshorter);
                                  break;
                             case EQUAL:
                                  root->Balance=LEFT;
                                  *pshorter=False;
                                  break;
                             case RIGHT:
                                  root->Balance=EQUAL;
                                  break;
                            }
                 }
     else
        if (CompCoor==Smaller)
           {
            root->pleft=FreeBTV1(root->pleft, NDim, pCoor, pshorter);
            if (*pshorter)
               switch (root->Balance)
                      {
                       case LEFT:
                            root->Balance=EQUAL;
                            break;
                       case EQUAL:
                            root->Balance=RIGHT;
                            *pshorter=False;
                            break;
                       case RIGHT:
                            root=RightDelBTVBalance(root, pshorter);
                            break;
                      }
           }
        else
           {
            root->pright=FreeBTV1(root->pright, NDim, pCoor, pshorter );
            if (*pshorter)
                switch (root->Balance)
                      {
                       case LEFT:
                            root=LeftDelBTVBalance(root, pshorter);
                            break;
                       case EQUAL:
                            root->Balance=LEFT;
                            *pshorter=False;
                            break;
                       case RIGHT:
                            root->Balance=EQUAL;
                            break;
                      }
           }
    }
 return root;
}

/*---------------------------------------------------------------------------*/
VOID FreeVertexBTV(PBTV pbtv, INT NDim, PREAL pCoor)
{
 BOOL shorter=False;

 pbtv->pFirstBTVNode=FreeBTV1(pbtv->pFirstBTVNode, NDim, pCoor, &shorter);
 pbtv->NElem--;
}

/*---------------------------------------------------------------------------*/
VOID PrintStatBTV(FILE * FOut, PBTV pbtv, PCHAR String,
                   INT PosIni,INT PosEnd)
{
 if (pbtv!=NULL)
    {
     if (FOut==stderr)
        fprintf(FOut,"\e[%d;0H",PosIni);
     fprintf(FOut,
     "========== %s :\n",String);
     fprintf(FOut,"MaxEle___: ");
     fprintf(FOut,"%10d\n",pbtv->MaxNElem);

     fprintf(FOut,"NElem____: ");
     fprintf(FOut,"%10d \n",pbtv->NElem);

     if (FOut==stderr)
        fprintf(FOut,"\e[%d;0H",PosEnd);
    }
  else
     fprintf(FOut,"N ");
}

/*---------------------------------------------------------------------------*/
VOID PrintBTV(PBTV pbtv, INT NDim){
  PREORDEN(pbtv->pFirstBTVNode, NDim);
}

VOID PREORDEN(PBTVNODE pNode, INT NDim){
  if(pNode != NULL){
    int i;
    for(i = 0; i < NDim; i++){
      fprintf(stderr,"%f ",pNode->pV[i]);
    }
    fprintf(stderr,"(%d)\n", pNode->visited);
    PREORDEN(pNode->pleft, NDim);
    PREORDEN(pNode->pright, NDim);
  }
}

BOOL VISITED(PBTVNODE pNode, INT NDim, PREAL pX){
  if(pNode != NULL){
    int i;
    BOOL samePoint = True;
    for(i = 0; i < NDim; i++){
      if(!EQ(pNode->pV[i], pX[i])){
        samePoint = False;
        break;
      }
    }

    if(samePoint == True){
      pNode->visited = True;
      return True;
    }

    if(samePoint == False){
      BOOL left  = False;
      BOOL right = False;

      left  = VISITED(pNode->pleft, NDim, pX);
      right = VISITED(pNode->pright, NDim, pX);

      if(left == True || right == True){
        return True;
      }else{
        return False;
      }
    }

  }

  return False;
}

INT  Count	(PBTVNODE pNode, INT NDim){
  if(pNode != NULL){
    int left = Count(pNode->pleft, NDim);
    int right = Count(pNode->pright, NDim);

    return 1 + left + right;
  }

  return 0;
}

INT  CountVisited	(PBTVNODE pNode, INT NDim){
  if(pNode != NULL){
    int left = CountVisited(pNode->pleft, NDim);
    int right = CountVisited(pNode->pright, NDim);

    if(pNode->visited == True){
      return 1 + left + right;
    }else{
      return left + right;
    }
  }

  return 0;
}
/*---------------------------------------------------------------------------*/
