/**
    @file deque.c
    @author Griffin Brookshire (glbrook2)
    Executes a doubly-linked list implementation.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "command.h"

/** Max length of input string */
#define BUFFER 32

/** Apply method for the PushBack command. */
static void pushBack( Command *this, List *list ) {
  // Get our node we've already made.
  Node *n = (Node *) this->data;

  // Link this node into the tail of the list.
  n->next = NULL;
  if ( list->tail ) {
    // There's already a node on the list.
    n->prev = list->tail;
    list->tail->next = n;
  } else {
    // The list was previously empty.
    n->prev = NULL;
    list->head = n;
  }

  list->tail = n;

  // The node for this command is currently on the list.
  this->data = NULL;
}

/** Undo method for the PopBack command. */
static void popBack( Command *this, List *list ) {
  // Remove the last node from the list.
  Node *n = list->tail;
  if ( list->tail->prev ) {
    // List will still be non-empty after the remove.
    list->tail = list->tail->prev;
    list->tail->next = NULL;
  } else {
    // The list is now empty.
    list->tail = NULL;
    list->head = NULL;
  }

  // Remember the node we just removed, in case we want to re-insert it later.
  this->data = n;
}
/** Undo method for the PushFront command. */
static void pushFront( Command *this, List *list ) {
  Node *n = (Node *) this->data;

  // Link this node into the head of the list.
  n->prev = NULL;
  if ( list->head ) {
    // There's already a node on the list.
    n->next = list->head;
    list->head->prev = n;
  } else {
    // The list was previously empty.
    n->next = NULL;
    list->tail = n;
  }

  list->head = n;

  // The node for this command is currently on the list.
  this->data = NULL;
}

/** Undo method for the PopFront command. */
static void popFront( Command *this, List *list ) {
  Node *n = list->head;
  if ( list->head->next ) {
    list->head = list->head->next;
    list->head->prev = NULL;
  } else {
    list->tail = NULL;
    list->head = NULL;
  }
  this->data = n;
}

/** Destroyfor a pushBack command */
static void destroyCommand( Command *this ) {
  if ( this->data ) {
    // If this command has a non-null data pointer, then it's a
    // pointer to a node is not part of the list, so we need to free
    // it when we free the command.
    Node *n = (Node *) this->data;
    free( n->str );
    free( n );
  }
  free( this );
}

/** Make a Command object that knows how to push a new string on
    the back of a list.
    @param str The new string to put on the list.
    @return a pointer to the new command object.
*/
static Command *makePushBack( char *str ) {
  // Make the command object and fill in its method pointers.
  Command *cmd = (Command *) malloc( sizeof( Command ) );
  cmd->apply = pushBack;
  cmd->undo = popBack;
  cmd->destroy = destroyCommand;

  // Go ahead and make the node we're going to append in the
  // apply operation, and store it in the command's data field.
  Node *n = (Node *) malloc( sizeof( Node ) );
  n->str = (char *) malloc( strlen( str ) + 1 );
  strcpy( n->str, str );
  cmd->data = n;

  return cmd;
}

/** Make a Command object that knows how to push a new string on
    the front of a list.
    @param str The new string to put on the list.
    @return a pointer to the new command object.
*/
static Command *makePushFront( char *str ) {
  Command *cmd = (Command *) malloc( sizeof( Command ) );
  cmd->apply = pushFront;
  cmd->undo = popFront;
  cmd->destroy = destroyCommand;
  Node *n = (Node *) malloc( sizeof( Node ) );
  n->str = (char *) malloc( strlen( str ) + 1 );
  strcpy( n->str, str );
  cmd->data = n;
  return cmd;
}


/** Make a Command object that knows how to pop a string off
    the back of a list.
    @return a pointer to the new command object.
*/
static Command *makePopBack() {
  Command *cmd = (Command *) malloc( sizeof( Command ) );
  cmd->apply = popBack;
  cmd->undo = pushBack;
  cmd->destroy = destroyCommand;
  return cmd;
}

/** Make a Command object that knows how to pop a string off
    the front of a list.
    @return a pointer to the new command object.
*/
static Command *makePopFront() {
  Command *cmd = (Command *) malloc( sizeof( Command ) );
  cmd->apply = popFront;
  cmd->undo = pushFront;
  cmd->destroy = destroyCommand;
  return cmd;
}

/** Make a Command object that pertains to the command
    inputted by the user.
    @param line The command inputted by the user.
    @return a pointer to the new command object.
*/
static Command *parseCommand( char *line ) {
  char *str;
  char *word = strtok( line, " " );
  if ( strcmp( word, "push-front" ) == 0 ) {
    str = strtok( NULL, " " );
    return makePushFront( str );
  } else if ( strcmp( word, "push-back" ) == 0 ) {
    str = strtok( NULL, " " );
    return makePushBack( str );
  } else if ( strcmp( word, "pop-front" ) == 0 ) {
    return makePopFront();
  } else if ( strcmp( word, "pop-back" ) == 0 ) {
    return makePopBack();
  } else {
    fprintf( stdout, "Invalid command\n" );
  }
  return NULL;
}

/** Make a Command object that knows how to push a new string on
    the front of a list.
    @return 0 if successfully runs
*/
int main() {
  List *list = makeList();
  char line[ BUFFER ];
  while ( true ) {
    fprintf( stdout, "> " );
    char *test = fgets( line, sizeof( line ), stdin );
    if ( test == NULL ) {
      break;
    }
    fprintf( stdout, "%s", line );
    int size = strlen( line );
    line[ size - 1 ] = '\0';
    Command *cmd = NULL;
    if ( strcmp( line, "report" ) == 0 ) {
      report( list );
      continue;
    } else if ( strcmp( line, "quit" ) == 0 ) {
      exit( 0 );
    } else if ( strcmp( line, "undo" ) == 0 ) {
      undoCommand( list );
      continue;
    } else if ( strcmp( line, "redo" ) == 0 ) {
      redoCommand( list );
      continue;
    } else {
      cmd = parseCommand( line );
    }
    if ( cmd == NULL ) {
      continue;
    }
    applyCommand( cmd, list  );
  }
  freeHistory();
  freeList( list );
  return 0;
}
