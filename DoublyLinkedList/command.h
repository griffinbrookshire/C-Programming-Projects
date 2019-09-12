/**
    @file command.h
    @author Griffin Brookshire (glbrook2)
    Defines structs and methods used by command.c.
*/

#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>
#include "list.h"

// Short name for the command type.
typedef struct CommandStruct Command;

/** Structure for a Command, an edit to what's on the list. */
struct CommandStruct {
  /** Pointer to a function to apply (or redo) the current command, making a
      change to the given list.
      @param list The list to modify. */
  void (*apply)( Command *this, List *list );

  /** Pointer to a function to undo the current command, making a
      change to the given list.
      @param list The list to modify. */
  void (*undo)( Command *this, List *list );

  /** Free memory used to represent this command. */
  void (*destroy)( Command *this );

  /** Different instances of Command may need to remember some extra data
      to be able to apply or undo themselves.  They can use this pointer
      to keep up with any data they need. */
  void *data;
};

/** Apply the given edit command to the given list, and put it on
    the undo list.
    @param list The list to modify.
    @param cmd The command to apply.
*/
void applyCommand( Command *cmd, List *list );

/** Undoes the last command of the user.
    @param list A pointer to the current list.
    @return true if successfully undoes command, false if it cannot.
*/
bool undoCommand( List *list );

/** Redoes the last undo.
    @param list A pointer to the current list.
    @return true if successfully undoes command, false if it cannot.
*/
bool redoCommand( List *list );

/**
  Frees the history stacks.
*/
void freeHistory();

#endif
