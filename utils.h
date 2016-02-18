/******************************************************************************
			utils.h  - description
			----------------------
	begin		: April 2004
	copywirght	: (C) 2004 by L.G.Casado.
	
     ********************************************************************	
     Purpose: Define new types of data and implement routines that will
     	be used by other modules. 
	
	OpenFile(): Open a file given by "name" in the mode given by "mode".
	CloseFile(): Check the correct close.
	ParametersError(): Shows the correct use of the comand line if some
		error has been detected.
	ReadProblem: Get from a file the problem name, number of dimensions,
		number of linear constraints, linear contraints,...	
	PrintVR(): Print the contents od a real vector.
	CopyVR(): Copy two real vectors.
	CompareVR(): Compare in order each component of two real vectors. 
		If there is a difference it returns if the first is smaller or 
		greater, otherwise both vectors are equal.
	NewSQMatrixR(): Get memory for a square matrix.
	FreeSQMatrixR(): Free the memory allocated by a sqare matrix.
	PrintSQMatrixR(): Print the square matrix values.
	CopySQMatrixR(): Copy a square matrix.
	RandomInt(): Generates a real random number between 0 an n with an
		integer value.
	XInWindow(): Given a point in the simplex space, calculates its X
		coordinate in the graphical window space.
	YInWindow(): Given a point in the simplex space, calculates its Y
		coordinate in the graphical window space.				
*******************************************************************************/
#ifndef __UTILS__
#define __UTILS__

/*---------------------------------------------------------------------------*/
typedef size_t		SIZE;
typedef	void		VOID;
typedef VOID 	*	PVOID;
typedef	PVOID	* 	PPVOID;
typedef PPVOID  *       PPPVOID;

typedef int		INT;
typedef INT 	*	PINT;
typedef	PINT	*	PPINT;

typedef unsigned int	UINT;
typedef UINT 	*	PUINT;
typedef	PUINT	*	PPUINT;

typedef unsigned long long int	ULLINT;

typedef unsigned long int	ULINT;
typedef ULINT 	*		PULINT;

typedef char		CHAR;
typedef CHAR	*	PCHAR;
typedef	PCHAR	* 	PPCHAR;

typedef	double		REAL;
typedef REAL	 *	PREAL;
typedef PREAL	 * 	PPREAL;
typedef PPREAL	 * 	PPPREAL;
typedef PPPREAL	 * 	PPPPREAL;

#define MAXVALUE	MAXDOUBLE
#define MINVALUE	MINDOUBLE
#define	REALPrec	10e-15  

typedef unsigned char 	UCHAR;

typedef UCHAR 		BOOL;
#define	False	0
#define	True	1
typedef BOOL * PBOOL;

typedef CHAR 		COMPARE;
#define	Greater	  1
#define Equal	  0
#define	Smaller	 -1

/*For binary trees*/
#define LEFT  -1
#define EQUAL  0
#define RIGHT  1


/*---------------------------------------------------------------------------*/
FILE 	*OpenFile       (char *name, char *mode);
void 	CloseFile       (FILE *f);
void 	ParametersError ();
VOID 	ReadProblem     (PCHAR FileName, PCHAR ProblemName, PINT pNDim, 
                         PINT pNLC, PPPREAL pppLC,PPREAL ppCostV, 
			 PPREAL ppCostP,PPREAL ppMaxFeasRad,
			 PINT pNQC, PPPPREAL ppppA, PPPREAL pppB, PPREAL ppD);
VOID    MinMaxEigenValue(PPREAL ppA, INT NDim, INT NNQC, 
                         PPREAL ppMinMaxEigenV);
void 	PrintVR 	(FILE * FOut, PREAL vr, INT NDim);
void    PrintMR         (FILE * FOut, PPREAL mr, INT Rows, INT Cols);
void    PrintBinary     (FILE * FOut, UINT Binary, INT NDim);
void    CopyVR 		(PREAL PD, PREAL PO, INT NDim);
COMPARE CompareVR 	(PREAL VR1, PREAL VR2, INT NDim);
REAL    RandomInt       (INT n);
REAL    XInWindow       (PREAL pX, INT WWidth);
REAL    YInWindow       (PREAL pX, INT WWidth);
VOID    DrawCircle      (PREAL pX, REAL Radius, INT NCircle, 
                         INT WWidth, PCHAR color, PCHAR dash);
VOID    DelCircle       (INT NCircle);
VOID    DrawEpsCircle   (PREAL pX, REAL Radius, INT NCircle, INT WWidth, 
                         PCHAR OutLineColor, PCHAR FillColor);
VOID    DelEpsCircle    (INT NCircle);
VOID    DrawPCover      (PREAL pX, INT NPC, INT WWidth, PCHAR color);
VOID    DelPCover       (INT NPC);
INT     Fact            (INT n); 
PPINT   CombNoRep       (INT n, INT k, PINT pNComb);
INT     CombN2ToIndex   (INT N, INT x, INT y);
VOID    IndexToCombN2   (INT N, INT Index, PINT px, PINT py);
REAL    DistPoints      (PREAL p1, PREAL p2, INT NDim);
BOOL    EQ              (REAL a, REAL b);
BOOL    LT              (REAL a, REAL b);
BOOL    GT              (REAL a, REAL b);
BOOL    LE              (REAL a, REAL b);
BOOL    GE              (REAL a, REAL b);
void    FractionPoint   (PREAL pA, PREAL pB, PREAL pNew, REAL Fraction, 
		                       INT NDim);
PCHAR   ColorConst      (INT NConst);
PCHAR   DashConst       (INT NConst);
PCHAR   ColorCover      (INT WhichCover);

#endif /*__UTILS__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
