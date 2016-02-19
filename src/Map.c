/*
Map - Int,Int Map List implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include "Map.h"

PMap InitMap(){
  PMap map = (struct Map*)malloc(sizeof(struct Map));
  map->size = 0;
  map->first = NULL;
  map->last = NULL;
  return map;
}

void Insert(PMap map, int key, int value){
  if(map->size == 0){
    struct MapNode* newNode =	(struct MapNode*)malloc(sizeof(struct MapNode));
  	newNode->key = key;
    newNode->value = value;
  	newNode->next = NULL;

    map->first = newNode;
    map->last = newNode;
    map->size = 1;
  }else{
    struct MapNode* tempNode = map->first;
    BOOL found = False;

    while(tempNode != NULL){
      if(tempNode->key == key){
        tempNode->value = value;
        found = True;
        break;
      }
      tempNode = tempNode->next;
    }

    if(found == False){
      struct MapNode* newNode =	(struct MapNode*)malloc(sizeof(struct MapNode));
    	newNode->key = key;
      newNode->value = value;
    	newNode->next = NULL;

      map->last->next = newNode;

      map->last = newNode;

      map->size = map->size + 1;
    }
  }
}

void AddOne(PMap map, int key){
  if(map->size == 0){
    struct MapNode* newNode =	(struct MapNode*)malloc(sizeof(struct MapNode));
  	newNode->key = key;
    newNode->value = 1;
  	newNode->next = NULL;

    map->first = newNode;
    map->last = newNode;
    map->size = 1;
  }else{
    struct MapNode* tempNode = map->first;
    BOOL found = False;

    while(tempNode != NULL){
      if(tempNode->key == key){
        tempNode->value = tempNode->value + 1;
        found = True;
        break;
      }
      tempNode = tempNode->next;
    }

    if(found == False){
      struct MapNode* newNode =	(struct MapNode*)malloc(sizeof(struct MapNode));
    	newNode->key = key;
      newNode->value = 1;
    	newNode->next = NULL;

      map->last->next = newNode;

      map->last = newNode;

      map->size = map->size + 1;
    }
  }
}

int Get(PMap map, int key){
  if(map->size > 0){
    struct MapNode* tempNode = map->first;

    while(tempNode != NULL){
      if(tempNode->key == key)
        return tempNode->value;
      tempNode = tempNode->next;
    }
  }
  return -1;
}

void ShowMap(PMap map){
  if(map->size > 0){
    struct MapNode* tempNode = map->first;

    while(tempNode != NULL){
      fprintf(stderr, "%i -> %i\n", tempNode->key, tempNode->value);
      tempNode = tempNode->next;
    }
  }
}

void FreeMap(PMap map){
  if(map->size > 0){
    struct MapNode* tempNode = map->first;
    struct MapNode* tempNodeNext = tempNode->next;

    while(tempNodeNext != NULL){
      free((void*)tempNode);
      tempNode = tempNodeNext;
      tempNodeNext = tempNode->next;
    }
  }
  free((void*)map);
}
