/**
    Gives prototypes for buffer functionality.
  */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool cut( char *line, int start, int n );

bool copy( char *line, int start, int n );

bool paste( char *line, int start );
