/******************************************************************************
			Queue.h  - description
			----------------------
	begin		: Nov 2015
	copywirght	: (C) 2015 by J.M.G.Ss

     ********************************************************************
     Purpose: Queue

     Routines:
      ...

*******************************************************************************/
#ifndef __QUEUE__
#define __QUEUE__

struct Node {
	double* data;
	struct Node* next;
};

struct Queue {
	// Two global variables to store address of front and rear nodes.
	struct Node* front;
	struct Node* rear;
	int size;
	struct Node* top; // For iterate
};

typedef struct Queue* PQueue;

/*----------------------------------------------------------------------------*/
PQueue InitQueue();
void Push(PQueue queue, double* x);
void Pop(PQueue queue);
double* Front(PQueue queue);
void FreeQueue(PQueue queue);
void Next(PQueue queue);
double* Top(PQueue queue);
int End(PQueue queue);

#endif /*__QUEUE__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
