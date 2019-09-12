/**
    @file list.h
    @author Griffin Brookshire (glbrook2)
    Defines the structs needed for a list
    and gives prototypes of needed functions.
*/

#ifndef LIST_H
#define LIST_H

// It's OK to make an alias for a type before you've defined the type.
typedef struct NodeStruct Node;

/** Representation for a node on the linked list, containing a string. */
struct NodeStruct {
  /** Dynamically allocated string stored in this node. */
  char *str;

  /** Pointer to the next node. */
  Node *next;

  /** Pointer to the previous node. */
  Node *prev;
};

/** Structure for the whole list, including head and tail pointers. */
typedef struct {
  /** Pointer to the first node on the list (or NULL ). */
  Node *head;

  /** Pointer to the first node on the list (or NULL ). */
  Node *tail;
} List;

/** Makes a new list.
    @return A pointer to the new list.
*/
List *makeList();

/** Prints the contents of the list.
    @param A pointer to the current list.
*/
void report( List *list );

/** Frees the list.
    @param A pointer to the current list.
*/
void freeList( List *list );

#endif
