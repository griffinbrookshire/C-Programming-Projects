/**
    @file textbox.c
    @author Griffin Brookshire (glbrook2)
    This program adds a textbox around an inputted text file.
  */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Width of each line of text in the box. */
#define LINE_WIDTH 60

/** Symbol used to draw the border around the box. */
#define BORDER '*'

/** Symbol used to draw the border around the box. */
#define FILLER ' '

/** Indicates a program success */
#define EXIT_SUCCESS 0

/**
    Reads and prints a single line of text inside the border.
    @return true if a border was added to the line, false if there are no more lines.
  */
bool paddedLine()
{
    int count = LINE_WIDTH;
    int ch = getchar();
    if ( ch == EOF ) {
        return false;
    }
    putchar( BORDER );
    while ( true ) {
        if ( count == 0 ) { // if you get to 60 characters...
            while ( ch != '\n' ) { // then consume the rest of the 
                ch = getchar(); // line until you get a newline char
            }
            break; // then break
        }
        if ( ch == '\n' ) {
            break;
        }
        putchar( ch );
        count--;
        ch = getchar();
    }
    for ( int i = 0; i < count; i++ ) {
        putchar( FILLER );
    }
    putchar( BORDER );
    putchar( '\n' );
    return true;
}

/**
    This function prints out multiple copies of the given character,
    followed by a newline. The number of copies is determined by the 'count' parameter.
    @param ch the character to print copies of
    @param count the number of times to print the specified character
  */
void lineOfChars( char ch, int count )
{
    for ( int i = 0; i < count; i++ ) {
        putchar( ch );
    }
    putchar( '\n' );
}

/**
    The main function prints the top portion of the border, 
    adds border to the body of the input file, then prints the bottom border.
    @return exit status
  */
int main()
{
    lineOfChars( BORDER, LINE_WIDTH + 2 );
    bool success = true;
    while ( success ) {
        success = paddedLine();
    }
    lineOfChars( BORDER, LINE_WIDTH + 2 );
    return EXIT_SUCCESS;
}