#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include "argshand.h"
#include "utils.h"
#include "CDSimplex.h"
#include "listCDSimplexByLevel.h"

int main(int argc, char *argv[]) {
	PCDSIMPLEX simplex1, simplex2, simplex3;
	INT i;
	INT NDim = 4;
	REAL IniXi = 1.0;
	REAL IniLength = sqrt(2.0 * IniXi * IniXi);

	PREAL pCentreT1 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	for (i = 0; i < NDim; i++)
		pCentreT1[i] = IniXi / (REAL) NDim;

	PREAL pCentreT2 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	for (i = 0; i < NDim; i++)
		pCentreT2[i] = IniXi / (REAL) NDim;

	PREAL pCentreT3 = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "RegSDiv:pCentreT");
	for (i = 0; i < NDim; i++)
		pCentreT3[i] = IniXi / (REAL) NDim;

	//INT NDim, PREAL pCentre, REAL L, REAL R, BOOL Up, BOOL Overlap, ULLINT NSimplex, UCHAR DivPhase, INT Level
	simplex1 = NewCDSimplex(NDim, pCentreT1, IniLength, 1.0, True, False, 1, 0, 1);
	simplex2 = NewCDSimplex(NDim, pCentreT2, IniLength, 1.0, True, False, 2, 0, 2);
	simplex3 = NewCDSimplex(NDim, pCentreT3, IniLength, 1.0, True, False, 3, 0, 3);

	PLISTCDSBYLEVEL lista = NULL;
	lista = NewListCDSByLevel(lista);

	InsertListCDSByLevel(lista, simplex1);
	InsertListCDSByLevel(lista, simplex2);
	InsertListCDSByLevel(lista, simplex3);

	PrintListCDSByLevel(lista);

	FreeListCDSByLevel(lista);

	return 0;
}
