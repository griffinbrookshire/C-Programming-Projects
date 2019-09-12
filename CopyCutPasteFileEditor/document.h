/**
    Gives prototypes for functionality of document and defines variables.
  */

#include <string.h>
#include <stdio.h>

/** Maximum length of a line in the document. */
#define MAX_LENGTH 100

/** Maximum number of lines in the document. */
#define MAX_LINES 10000

int readDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH + 1 ] );

void printDocument( FILE *fp, char doc[ MAX_LINES ][ MAX_LENGTH + 1 ], int lines );
