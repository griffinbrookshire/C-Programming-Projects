/**
    @file style.c
    @author Griffin Brookshire (glbrook2)
    This program prints a paragraph of random words
    and prints a count of the number of words within it.
*/

#include <stdio.h>
#include <stdlib.h>

/** Maximum number of words in a line. */
#define SEVENTY 72

/** Indicates a program success */
#define EXIT_SUCCESS 0

/**
    Prints a word composed a random letters with a length of input 'x'.
    @param x the length of the word
*/
void printWord( int x )
{
    for ( int i = 0; i < x; i++ ) {
        // Print a random lower-case letter.
        printf( "%c", 97 + rand() % 26 );
    }
}

/**
    Prints a line of random words. There is a max of 72 words in a line.
    @return count the number of words in the line
*/
int printLine()
{
    int count = 0, pos = 0, space = 0;
    int len = 1 + rand() % 10;
    // Print a line of words up to a limited length.
    while ( pos + len + space < SEVENTY ) {
        if ( space > 0 ) {
            printf( " " );
        }
        printWord( len );
        pos += len + space;
        len = 1 + rand() % 10;
        space = 1;
        count += 1;
    }
    printf( "\n" );
    return count;
}

/**
    Prints a paragraph composed of 'n' lines. Counts the total number of words printed.
    @param n the number of lines to print
    @return total the total number of words printed
*/
int printParagraph( int n )
{
    int total = 0;
    for ( int i = 0; i < n; i++ ) {
        total += printLine();
    }
    return total;
}

/**
    Executes the printing of a paragraph containing random words.
    @return exit status
*/
int main()
{
    // Total number of words printed in the program.
    int w = printParagraph( 10 );
    
    // Print the total number of words.
    printf( "Words: %d\n", w );
    return EXIT_SUCCESS;
}