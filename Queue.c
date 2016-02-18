/*
Queue - Linked List implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

PQueue InitQueue(){
  PQueue queue = (struct Queue*)malloc(sizeof(struct Queue));
  queue->size = 0;
  queue->front = NULL;
  queue->rear = NULL;
  queue->top = NULL;
  return queue;
}

// To Enqueue a double*
void Push(PQueue queue, double* x){
	struct Node* temp =	(struct Node*)malloc(sizeof(struct Node));
	temp->data = x;
	temp->next = NULL;
	if(queue->front == NULL && queue->rear == NULL){
    queue->front = queue->rear = queue->top = temp;
	}else{
    queue->rear->next = temp;
    queue->rear = temp;
  }
  queue->size = queue->size + 1;
}

// To Dequeue a double*
void Pop(PQueue queue){
	struct Node* temp = queue->front;
	if(queue->front != NULL){
  	if(queue->front == queue->rear){
      queue->front = queue->rear = queue->top = NULL;
  	}else{
      queue->front = queue->top = queue->front->next;
  	}
    queue->size = queue->size - 1;
  	free(temp);
  }
}

double* Front(PQueue queue){
	if(queue->front == NULL){
		return NULL;
	}
	return queue->front->data;
}

void FreeQueue(PQueue queue){
  while(Front(queue) != NULL){
    free((void*)Front(queue));
    Pop(queue);
  }
  free((void*)queue);
}

void Next(PQueue queue){
  if(queue->top == queue->rear){
    queue->top = queue->front;
  }else{
    queue->top = queue->top->next;
  }
}

double* Top(PQueue queue){
  if(queue->top == NULL){
    return NULL;
  }
  return queue->top->data;
}

int End(PQueue queue){
  // IMPORTANTE: Recorrer con un do-while para no detectar el primer front
  if(queue->top == queue->front){
    return 1;
  }
  return 0;
}
