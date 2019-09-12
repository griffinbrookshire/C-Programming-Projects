/**
    @file command.c
    @author Griffin Brookshire (glbrook2)
    Executes the commands of the user.
*/

#include <stdio.h>
#include <stdlib.h>
#include "command.h"

/** Maximum number of commands on the undo or redo lists. */
#define HIST_MAX 10

/** List of commands that have been applied, available for undo-ing. */
static Command *undoList[ HIST_MAX ];

/** Number of commands on the past list. */
static int undoLen = 0;

/** List of commands that have been undone, available for redo-ing. */
static Command *redoList[ HIST_MAX ];

/** Number of commands on the future list. */
static int redoLen = 0;

/** Executes the command of the user and adjusts the history stacks.
    @param cmd A pointer to the command to execute.
    @param list A pointer to the current list.
*/
void applyCommand( Command *cmd, List *list )
{
  cmd->apply( cmd, list );

  // If the undo list is full, slide everything down to make room (a circular
  // queue would be more efficient here.)
  if ( undoLen >= HIST_MAX ) {
    undoList[ 0 ]->destroy( undoList[ 0 ] );
    for ( int i = 0; i + 1 < undoLen; i++ )
      undoList[ i ] = undoList[ i + 1 ];
    undoLen -= 1;
  }

  // The redo-history goes away whenever we apply a new command (since we may
  // no longer be able to redo those commands).
  for ( int i = redoLen - 1; i >= 0; i-- )
    redoList[ i ]->destroy( redoList[ i ] );
  redoLen = 0;

  // Put the new command at the end of the undo list.
  undoList[ undoLen++ ] = cmd;
}

/** Undoes the last command of the user.
    @param list A pointer to the current list.
    @return true if successfully undoes command, false if it cannot.
*/
bool undoCommand( List *list ) {
  if ( undoLen <= 0 ) {
    fprintf( stdout, "Invalid command\n" );
    return false;
  }
  Command *cmd = undoList[ undoLen - 1 ];
  cmd->undo( cmd, list );
  redoList[ redoLen++ ] = cmd;
  undoLen--;
  return true;
}

/** Redoes the last undo.
    @param list A pointer to the current list.
    @return true if successfully undoes command, false if it cannot.
*/
bool redoCommand( List *list ) {
  if ( redoLen <= 0 ) {
    fprintf( stdout, "Invalid command\n" );
    return false;
  }
  Command *cmd = redoList[ redoLen - 1 ];
  cmd->apply( cmd, list );
  undoList[ undoLen++ ] = cmd;
  redoLen--;
  return true;
}

/**
  Frees the history stacks.
*/
void freeHistory() {
  for ( int i = undoLen - 1; i >= 0; i-- )
    undoList[ i ]->destroy( undoList[ i ] );
  undoLen = 0;
  for ( int i = redoLen - 1; i >= 0; i-- )
    redoList[ i ]->destroy( redoList[ i ] );
  redoLen = 0;
}
