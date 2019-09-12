 /**
    @file buffer.c
    @author Griffin Brookshire (glbrook2)
    Defines functionality for cut, copy, and paste.
  */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "document.h"

/** Holds the current characters in the buffer. */
static char buffer [ MAX_LENGTH + 1 ];

/**
    Cuts characters from the line and stores them in the buffer.
    @param line The line to cut
    @param start Index from which to start cutting
    @param n The number of characters to cut
    @return true if successful, false if error
  */
bool cut( char *line, int start, int n ) {
  size_t length = strlen( line );
  if ( start < 0 || ( start + n ) > ( length + 1 ) ) {
    return false;
  }
  /** Clear buffer */
  for ( int i = 0; i < MAX_LENGTH + 1; i++ ) {
    buffer[ i ] = '\0';
  }
  /** Copy to buffer */
  int bufIdx = 0;
  for ( int i = start - 1; i < start + n - 1; i++ ) {
    buffer[ bufIdx ] = line[ i ];
    bufIdx++;
  }
  /** Copy ending */
  char *ending = malloc( MAX_LENGTH + 1 );
  int endIdx = 0;
  for ( int i = start + n - 1; i < length; i++ ) {
    ending[ endIdx ] = line[ i ];
    endIdx++;
  }
  ending[ endIdx ] = '\0';
  
  /** Insert ending */
  size_t endLength = strlen( ending );
  endIdx = 0;
  for ( int i = start - 1; i < start - 1 + endLength; i++ ) {
    line[ i ] = ending[ endIdx ];
    endIdx++;
  }
  /** Mark new terminator */
  line[ start + endLength - 1 ] = '\0';
  free( ending );
  return true;
}

/**
    Copy characters from the line and stores them in the buffer.
    @param line The line to copy
    @param start Index from which to start copying
    @param n The number of characters to copy
    @return true if successful, false if error
  */
bool copy( char *line, int start, int n ) {
  size_t length = strlen( line );
  if ( start < 0 || ( start + n ) > ( length + 1 ) ) {
    return false;
  }
  /** Clear buffer */
  for ( int i = 0; i < MAX_LENGTH + 2; i++ ) {
    buffer[ i ] = '\0';
  }
  /** Copy to buffer */
  int bufIdx = 0;
  for ( int i = start - 1; i < start + n - 1; i++ ) {
    buffer[ bufIdx ] = line[ i ];
    bufIdx++;
  }
  return true;
}

/**
    Pastes characters from the buffer to the line.
    @param line The line to paste to
    @param start Index from which to start pasting
    @return true if successful, false if error
  */
bool paste( char *line, int start ) {
  size_t length = strlen( line );
  size_t buffLength = strlen( buffer );
  if ( start < 0 || length + buffLength > MAX_LENGTH + 1 ) {
    return false;
  }
  /** Copy ending */
  
  char *ending = malloc( MAX_LENGTH + 1 );
  ending[ 0 ] = '\0';
  int endIdx = 0;
  for ( int i = start - 1; i < length; i++ ) {
    ending[ endIdx ] = line[ i ];
    endIdx++;
  }
  /** Insert buffer */
  int bufIdx = 0;
  for ( int i = start - 1; i < start + buffLength; i++ ) {
    line[ i ] = buffer[ bufIdx ];
    bufIdx++;
  }
  /** Insert ending */
  size_t endLength = strlen( ending );
  endIdx = 0;
  for ( int i = start + buffLength - 1; i < start + buffLength - 1 + endLength; i++ ) {
    line[ i ] = ending[ endIdx ];
    endIdx++;
  }
  free( ending );
  return true;
}
