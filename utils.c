#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "getmem.h"
#include "utils.h"

/*----------------------------------------------------------------------------*/
FILE * OpenFile (char *name, char *mode)
{
 FILE *f;

 f=fopen(name,mode);
 if (f==NULL)
    {
     fprintf(stderr,"Can not open the file %s\n",name);
     exit(1);
    }
 return f;
}

/*----------------------------------------------------------------------------*/
void CloseFile (FILE *f)
{
  if (fclose(f))
      fprintf(stderr,"Unable to close file.\n");
}

/*----------------------------------------------------------------------------*/
void ParametersError()
{
 fputs("Options are:\n",stderr);
 fputs("\t[ --help To show this help]\n",stderr);
 fputs("\t[ -d            <int>: dimension, default=3]\n",stderr);
 fputs("\t  -ep          <real>: Accuracy. w(s)<=ep*w(S1) \n",stderr);
 fputs("\t[ -ns                : No store final simplices]\n",stderr);
 fputs("\t[ -out               : Generates statistic output file]\n",stderr);
 fputs("\t[ -Div         <int> : Division method]\n",stderr);
 fputs("\t                     : [1]-> 2USC Grid\n",stderr);
 fputs("\t                     :  2 -> 2NUSC Grid\n",stderr);
 fputs("\t[ -w            <int>: Graphic window width (400)]\n",stderr);
 fputs("\t[ -tcl          <int>: Graphical:\"| escala.tcl\"]\n",stderr);
 fputs("\t                       1: normal draw, 2:press key]\n\n", stderr);       

 exit(0);
}       


/*---------------------------------------------------------------------------*/
void PrintVR (FILE * FOut, PREAL vr, INT NDim)
{
 int i;

 for (i=0;i<NDim;i++)
     fprintf(FOut,"%10.9f\n",vr[i]);
}

/*---------------------------------------------------------------------------*/
void PrintMR (FILE * FOut, PPREAL mr, INT Rows, INT Cols)
{
 int i;

 for (i=0;i<Rows;i++)
     {
      fprintf(FOut,"Row %d:\n",i);
      PrintVR(FOut,mr[i],Cols);
     }
 fprintf(FOut,"\n");     
}


/*---------------------------------------------------------------------------*/
void PrintBinary (FILE * FOut, UINT Binary, INT NDim)
{
 int i;
 UINT l;

 l=1;
 for (i=0;i<NDim;i++)
  {
   if (Binary&l)
      fprintf(stderr,"1");
   else
      fprintf(stderr,"0");   
   l=l<<1;
  }    
 fprintf(stderr,"\n"); 
}


/*---------------------------------------------------------------------------*/
void PrintVINT (FILE * FOut, PINT vr, INT NDim)
{
 int i;

 for (i=0;i<NDim;i++)
     fprintf(FOut,"%d,",vr[i]);
 fprintf(stderr,"\n");    
}
/*---------------------------------------------------------------------------*/
void CopyVR (PREAL PD, PREAL PO, INT NDim)
{
 INT i;
 
 for (i=0;i<NDim;i++) 
     PD[i]=PO[i];
}


/*---------------------------------------------------------------------------*/
void CopyVINT (PINT PD, PINT PO, INT NDim)
{
 INT i;
 
 for (i=0;i<NDim;i++) 
     PD[i]=PO[i];
}


/*---------------------------------------------------------------------------*/
COMPARE CompareVR (PREAL VR1, PREAL VR2, INT NDim)
{
 INT i;
 COMPARE Result;
 
 Result=Equal;
 
 for (i=0;i<NDim;i++)
     if (!EQ(VR1[i],VR2[i]))
        {
	 if (LT(VR1[i],VR2[i]))
	     Result=Smaller;
	 else
	     Result=Greater;    
	 break;
	}
     
 return Result;    
}

/*---------------------------------------------------------------------------*/
REAL RandomInt(INT n)
{
 return ((int)(drand48()*(REAL)n));
}

/*---------------------------------------------------------------------------*/
REAL XInWindow(PREAL pX, INT WWidth)
{
 REAL Xw;
 INT HalfWWidth;
 
 HalfWWidth=WWidth/2;
 Xw =(REAL)WWidth*pX[0]+(REAL)HalfWWidth*pX[2];
 
 return Xw;
}

/*---------------------------------------------------------------------------*/
REAL YInWindow(PREAL pX, INT WWidth)
{
 REAL Yw;
 REAL EqualTRiangleHigh;
 INT HalfWWidth;
 
 HalfWWidth=WWidth/2;
 EqualTRiangleHigh=(REAL)sin(3.1415926535/3.0)*(REAL)WWidth;
 Yw = (REAL)sin(3.1415926535/3.0)*(REAL)WWidth*pX[2];
 Yw = (REAL)HalfWWidth + (HalfWWidth) - Yw -
 	(REAL)((WWidth-(INT)EqualTRiangleHigh)/2.0); 
  
 return Yw;
}

/*---------------------------------------------------------------------------*/
VOID DrawCircle(PREAL pX, REAL Radius, INT NCircle, INT WWidth, 
		PCHAR color, PCHAR dash)
{
 REAL WRadius, Wx,Wy;
 
 WRadius = Radius/sqrt(2.0)*WWidth;
 Wx      = XInWindow(pX,WWidth);
 Wy      = YInWindow(pX,WWidth);

 printf("DrawCircle\n");
     
 printf("%f\n",Wx-WRadius);
 printf("%f\n",Wy-WRadius);
 printf("%f\n",Wx+WRadius);
 printf("%f\n",Wy+WRadius);
 printf("%s\n",color);
 printf("%s\n",dash);
 printf("%d\n",NCircle);  

 fflush(stdout);
}

/*---------------------------------------------------------------------------*/
VOID DelCircle(INT NCircle)
{
 printf("DelCircle\n");
 printf("%d\n",NCircle);
 fflush(stdout);
}

/*---------------------------------------------------------------------------*/
VOID DrawEpsCircle(PREAL pX, REAL Radius, INT NCircle, INT WWidth, 
                   PCHAR OutLineColor, PCHAR FillColor)
{
 REAL WRadius, Wx,Wy;
 
 WRadius = Radius/sqrt(2.0)*WWidth;
 Wx      = XInWindow(pX,WWidth);
 Wy      = YInWindow(pX,WWidth);

 printf("DrawEpsCircle\n");
     
 printf("%f\n",Wx-WRadius);
 printf("%f\n",Wy-WRadius);
 printf("%f\n",Wx+WRadius);
 printf("%f\n",Wy+WRadius);
 printf("%s\n",OutLineColor);
 printf("%s\n",FillColor);
 printf("%d\n",NCircle);  

 fflush(stdout);
}

/*---------------------------------------------------------------------------*/
VOID DelEpsCircle(INT NCircle)
{
 printf("DelEpsCircle\n");
 printf("%d\n",NCircle);
 fflush(stdout);
}


/*---------------------------------------------------------------------------*/
VOID DrawPCover(PREAL pX, INT NPC, INT WWidth, PCHAR color)
{
 printf("DrawPCover\n");    
 printf("%f\n",XInWindow(pX,WWidth));
 printf("%f\n",YInWindow(pX,WWidth));
 printf("%s\n",color);
 printf("%d\n",NPC);  

 fflush(stdout);
}

/*---------------------------------------------------------------------------*/
VOID DelPCover(INT NPC)
{
 printf("DelPCover\n");
 printf("%d\n",NPC);
 fflush(stdout);
}

/*---------------------------------------------------------------------------*/
/*INT NoverK(INT n, INT k)
{
 INT i;
 INT nk;
 INT Num;
 
 nk=n-k;
 
 if (nk > k)
    {
     for (i=1;i<k;i++)
         k=1; 
    }
}
*/
/*---------------------------------------------------------------------------*/
INT Fact(INT n)
{
 INT i;
 INT Result =1;
 
 for (i=2;i<=n;i++)
     Result*=i;
 return Result;    
}

/*---------------------------------------------------------------------------*/
PPINT CombNoRep(INT n, INT k, PINT pNComb)
{
 INT NComb,nk;
 INT i,j;
 PPINT S;
 
 NComb=0;
 nk=n-k;
 
 (*pNComb)=Fact(n)/(Fact(k)*Fact(nk));
  
 S=(PPINT) GetMem2D(*pNComb,k,(SIZE)sizeof(INT),"CombNoRep");
 
/* fprintf(stderr,"Convinaciones=%d.\n",Fact(n)/(Fact(k)*Fact(nk)) );
 fprintf(stderr,"Fact(%d)=%20d, Fact(%d)=%20d, Fact(%d)=%20d.\n",
                n,Fact(n),k,Fact(k),nk,Fact(nk));
 fprintf(stderr,"n=%d, k=%d.\n", n, k);
 fprintf(stderr,"NComb=%d, pNComb=%d.\n",NComb, (*pNComb));
*/
 
 for (i=0;i<k;i++)
     S[NComb][i]=i;
 
 
 /*PrintVINT(stderr,S[NComb],k);*/ 
 while (S[NComb][0] < nk)
       {	
        NComb++;
	CopyVINT(S[NComb],S[NComb-1],k);    
	
        if (S[NComb][k-1]<n-1)
	    S[NComb][k-1]++;
	else
	   {
	    j=1;
	    while (S[NComb][k-1-j]==n-1-j)
	          j++;
	    S[NComb][k-1-j]++;
	    for (i=k-j;i<k;i++) 
	       S[NComb][i]=S[NComb][i-1]+1;
	   } 
/*	fprintf(stderr,"NComb=%d = ",NComb);
	PrintVINT(stderr,S[NComb],k);       
*/	
       }
 return S;      
}


/*---------------------------------------------------------------------------*/
INT CombN2ToIndex(INT N, INT x, INT y)
{
 int Sum[17]={0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120,136};
 /*Sum(n)=n*(n-1)/2+n*/

 INT Tmp;
 if (x>y)
    {
     Tmp=x;
     x=y;
     y=Tmp;
    }

 return x*N-Sum[x]+(y-x)-1;
}

/*---------------------------------------------------------------------------*/
VOID IndexToCombN2(INT N, INT Index, PINT px, PINT py)
{
 int Sum[17]={0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120,136};
 /*Sum(n)=n*(n-1)/2+n*/
 INT x=N-2;
 INT y;
  
 while ( x*N - Sum[x] > Index )
       x--;
       
 y=x+Index+1-(x*N - Sum[x]);
 
 *px=x;
 *py=y;
}

/*---------------------------------------------------------------------------*/
REAL DistPoints(PREAL p1, PREAL p2, INT NDim)
{
 INT i;
 REAL Distance,Temp;
 
 Temp=p1[0] - p2[0];
 Distance=Temp*Temp;
 for (i=1;i<NDim;i++)
     {
      Temp      = p1[i] - p2[i];
      Distance += Temp*Temp;
     } 
     
 return sqrt(Distance);     
}

/*---------------------------------------------------------------------------*/
BOOL EQ(REAL a, REAL b)
{    
 if (a <= b && b-a < REALPrec)
    return True;
    
 if (a >= b && a-b < REALPrec)
    return True;
    
 return False;   
}

/*---------------------------------------------------------------------------*/
BOOL LT(REAL a, REAL b)
{
 if ( EQ(a,b) )
    return False;
    
 if ( a < b )
    return True;
    
 return False;   
}

/*---------------------------------------------------------------------------*/
BOOL GT(REAL a, REAL b)
{
 if ( EQ(a,b) )
    return False;
    
 if ( a > b )
    return True;
    
 return False;   
}

/*---------------------------------------------------------------------------*/
BOOL LE(REAL a, REAL b)
{
 if ( EQ(a,b) || a < b)
    return True;

 return False;   
}

/*---------------------------------------------------------------------------*/
BOOL GE(REAL a, REAL b)
{
 if ( EQ(a,b) || a > b)
    return True;

 return False;   
}

/*---------------------------------------------------------------------------*/
/*Return the fraction point between pA and pB*/
/*If Fraction=0.5, it return the middle point*/
void FractionPoint (PREAL pA, PREAL pB, PREAL pNew, REAL Fraction, INT NDim)
{
 int i;

 for (i=0;i<NDim;i++)
      pNew[i] = pA[i] + ( pB[i] - pA[i] ) * Fraction; 
}


/*---------------------------------------------------------------------------*/
PCHAR ColorCover(INT WhichCover)
{
 switch (WhichCover)
        {
	 case 1:
	      return "#4040ff";	      
	      break; 
	 case 2:
	      return "#5ca6ff";	      
	      break; 
	 case 3:
	      return "#5cc0ff";	      
	      break; 
	 default:
	      fprintf(stderr,"ColorCover: NConst =%d.\n",WhichCover);
	      exit(1);
	      break;  
	}
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
