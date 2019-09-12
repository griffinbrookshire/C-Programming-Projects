/**
    @file list.c
    @author Griffin Brookshire (glbrook2)
    Implements the doubly-linked list.
*/

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/** Makes a new list.
    @return A pointer to the new list.
*/
List *makeList() {
  List *list = ( List * )malloc( sizeof( List ) );
  list->head = NULL;
  list->tail = NULL;
  return list;
}

/** Prints the contents of the list.
    @param A pointer to the current list.
*/
void report( List *list ) {
  Node *temp = list->head;
  while ( temp ) {
    fprintf( stdout, "%s\n", temp->str );
    temp = temp->next;
  }
}

/** Frees the list.
    @param A pointer to the current list.
*/
void freeList( List *list ) {
  Node *temp;
  while ( list->head != NULL ) {
    temp = list->head;
    list->head = list->head->next;
    free( temp );
  }
  free( list );
}
