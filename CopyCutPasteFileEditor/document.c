 /**
    @file document.c
    @author Griffin Brookshire (glbrook2)
    Defines functionality for input and output.
  */

#include <string.h>
#include <stdio.h>

/** Maximum length of a line in the document. */
#define MAX_LENGTH 100

/** Maximum number of lines in the document. */
#define MAX_LINES 10000

/**
    Reads an input file and stores it in a character array.
    @param fp Pointer to the file to read
    @param doc The array to store characters in
    @return lineNumber The number of lines that were read
  */
int readDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH + 1 ] ) {
  int ch = 1, lineLength = 0, lineNumber = 0;
  while ( ( ch = fgetc( fp ) ) != EOF ) {
    if ( ch != '\n' ) {
      doc[ lineNumber ][ lineLength ] = (char)ch;
      lineLength++;
    } else {
      lineNumber++;
      lineLength = 0;
    }
  }
  return lineNumber;
}

/**
    Prints the current character array to file.
    @param fp Pointer to the file to write
    @param doc The array to write to file
  */
void printDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH + 1 ], int lines ) {
  char ch = doc[ 0 ][ 0 ];
  int lineLength = 0;
  for ( int i = 0; i < lines; i++ ) {
    while ( ch != '\0' ) {
      ch = doc[ i ][ lineLength ];
      if ( ch != '\0' ) {
        fprintf( fp, "%c", ch );
        lineLength++;
      }
    }
    fprintf( fp, "\n" );
    lineLength = 0;
    ch = doc[ i + 1 ][ lineLength ]; //set to first char in new line
  }
}
