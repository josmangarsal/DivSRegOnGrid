/******************************************************************************
			Map.h  - description
			----------------------
	begin		: Nov 2015
	copywirght	: (C) 2015 by J.M.G.Ss

     ********************************************************************
     Purpose: Map

     Routines:
      ...

*******************************************************************************/
#ifndef __MAP__
#define __MAP__

#include "utils.h"

struct MapNode {
	int key;
	int value;
	struct MapNode* next;
};

struct Map {
	int size;
	struct MapNode* first;
	struct MapNode* last;
};

typedef struct Map* PMap;

/*----------------------------------------------------------------------------*/
PMap InitMap();
void Insert(PMap map, int key, int value);
void AddOne(PMap map, int key);
int Get(PMap map, int key);
void ShowMap(PMap map);
void FreeMap(PMap map);

#endif /*__QUEUE__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
