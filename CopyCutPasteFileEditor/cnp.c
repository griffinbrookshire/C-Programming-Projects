 /**
    @file cnp.c
    @author Griffin Brookshire (glbrook2)
    Performs a variation of specified cut, copy, and/or paste functions
    on some input, then outputs the results.
  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "buffer.h"
#include "document.h"

/** Holds the current characters in the document. */
static char doc [ MAX_LINES ][ MAX_LENGTH + 1 ];

/**
    Tells if a string is a number or not.
    @param arg The string to test
    @return true if is a number, false if not
  */
bool isNumber( char *arg ) {
  size_t length = strlen( arg );
  for ( int i = 0; i < length; i++ ) {
    if ( !isdigit( arg[ i ] ) ) {
      return false;
    }
  }
  return true;
}

/**
    Reads an input file, performs cut/copy/paste, prints result to file.
    @param argc The number of arguments passed in via command line
    @param argv The arguments passed in via command line
    @return 0 if success, 1 if error
  */
int main( int argc, char *argv[] ) {

  /** Input */
  FILE *fp = NULL;
  int lines;
  char ch;
  int lineCount = 0, lineLength = 0;
  FILE *temp = NULL;
  if ( strcmp( argv[ argc - 2 ], "-" ) == 0 ) {
    temp = fopen( "temp.txt", "w" );
    while ( (ch = getc( stdin ) ) != EOF ) {
      if ( ch == '\n' ) {
        fprintf( temp, "\n" );
        lineCount++;
        if ( lineLength > MAX_LENGTH ) {
          fprintf( stderr, "Line too long\n");
          exit( 1 );
        } else {
          lineLength = 0;
        }
      } else {
        fprintf( temp, "%c", ch );
        lineLength++;
      }
    }
    if ( lineLength > MAX_LENGTH ) {
      fprintf( stderr, "Line too long\n");
      exit( 1 );
    }
    lineCount++;
    if ( lineCount > MAX_LINES ) {
      fprintf( stderr, "Too many lines\n");
      exit( 1 );
    }
    fclose( temp );
    fp = fopen( "temp.txt", "r" );
    lines = readDocument( fp, doc );
    fclose( fp );
  } else {
    fp = fopen( argv[ argc - 2 ], "r" );
    if ( fp == NULL ) {
      fprintf( stderr, "Can't open file: %s\n", argv[ argc - 2 ] );
      exit( 1 );
    }
    lineCount = 0, lineLength = 0;
    while ( (ch = getc( fp ) ) != EOF ) {
      if ( ch == '\n' ) {
        lineCount++;
        if ( lineLength > MAX_LENGTH ) {
          fprintf( stderr, "Line too long\n");
          exit( 1 );
        } else {
          lineLength = 0;
        }
      } else {
        lineLength++;
      }
    }
    if ( lineLength > MAX_LENGTH ) {
      fprintf( stderr, "Line too long\n");
      exit( 1 );
    }
    lineCount++;
    if ( lineCount > MAX_LINES ) {
      fprintf( stderr, "Too many lines\n");
      exit( 1 );
    }
    fp = fopen( argv[ argc - 2 ], "r" );
    lines = readDocument( fp, doc );
    fclose( fp );
  }
  
  /** Copy n Paste */
  for ( int j = 0; j < lines; j++ ) {
    char *line = doc[ j ];
    bool pasteOK = false;
    for ( int i = 1; i < argc - 2; i++ ) {
      if ( strcmp( argv[ i ], "copy" ) == 0 ) {
        if ( !isNumber( argv[ i + 1 ] ) || !isNumber( argv[ i + 2 ] ) ) {
          fprintf( stderr,
          "Invalid command\nusage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n" );
          exit( 1 );
        }
        int start;
        sscanf( argv[ i + 1 ], "%d", &start );
        int n;
        sscanf( argv[ i + 2 ], "%d", &n );
        if ( !copy( line, start, n ) ) {
          fprintf( stderr,
          "Invalid command\nusage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n" );
          exit( 1 );
        }
        pasteOK = true;
        i+=2;
      } else if ( strcmp( argv[ i ], "cut" ) == 0 ) {
        if ( !isNumber( argv[ i + 1 ] ) || !isNumber( argv[ i + 2 ] ) ) {
          fprintf( stderr,
          "Invalid command\nusage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n" );
          exit( 1 );
        }
        int start;
        sscanf( argv[ i + 1 ], "%d", &start );
        int n;
        sscanf( argv[ i + 2 ], "%d", &n );
        if ( !cut( line, start, n ) ) {
          fprintf( stderr,
          "Invalid command\nusage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n" );
          exit( 1 );
        }
        pasteOK = true;
        i+=2;
      } else if ( strcmp( argv[ i ], "paste" ) == 0 && pasteOK ) {
        if ( !isNumber( argv[ i + 1 ] ) ) {
          fprintf( stderr,
          "Invalid command\nusage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n" );
          exit( 1 );
        }
        int start;
        sscanf( argv[ i + 1 ], "%d", &start );
        if ( !paste( line, start ) ) {
          fprintf( stderr,
          "Invalid command\nusage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n" );
          exit( 1 );
        }
        i++;
      } else {
        fprintf( stderr,
        "Invalid command\nusage: ((cut s n)|(copy s n)|(paste s))* (infile|-) (outfile|-)\n" );
        exit( 1 );
      }
    }
  }
  
  /** Output */
  FILE *output = NULL;
  if ( strcmp( argv[ argc - 1 ], "-" ) == 0 ) {
    printDocument( stdout, doc, lines );
  } else {
    output = fopen( argv[ argc - 1 ], "w" );
    printDocument( output, doc, lines );
    fclose( output );
  }
  return 0;
}
