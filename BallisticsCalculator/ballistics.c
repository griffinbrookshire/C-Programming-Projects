/**
    @file ballistics.c
    @author Griffin Brookshire (glbrook2)
    This program prints a ballistics table that describes the flight pattern of
    a projectile launched with a user specified initial velocity.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** Double representing the gravitational constant. */
#define GRAVITY 9.81

/** Indicates a program success */
#define EXIT_SUCCESS 0

/** Maximum angle to calculate */
#define MAX_ANGLE 90

/** Increase in angle per calculation */
#define ANGLE_STEP 5

/**
    Calculates the flight time of a projectile given an angle and an initial velocity.
    @param angle the angle at which the projectile is launched
    @param v0 the initial velocity at which the projectile is launched
    @return time the time it takes for the projectile to land
  */
double flightTime( int angle, double v0 )
{
    double rads = angle * ( M_PI / 180 );
    double time = sin( rads );
    time = time * ( 2 * v0 );
    time = time / ( GRAVITY );
    return time;
}

/**
    Calculates the horizontal distance traveled by a projectile with a given
    angle and initial velocity.
    @param angle the angle at which the projectile is launched
    @param v0 the initial velocity at which the projectile is launched
    @return dist the horizontal distance traveled by the projectile
  */
double calcDistance( int angle, double v0, double time )
{
    double rads = angle * ( M_PI / 180 );
    double dist = cos ( rads );
    dist = dist * v0;
    dist = dist * time;
    return dist;
}

/**
    Prints a row of the ballistics table, calls helper methods to calculate data.
    @param angle the angle at which the projectile is launched
    @param v0 the initial velocity at which the projectile is launched
    @param t the time the projectile is airborne
  */
void tableRow( int angle, double v0, double t )
{
    printf( "%10d |", angle );
    printf( " %10.3f |", v0 );
    printf( " %10.3f |", t );
    double distance = calcDistance( angle , v0 , t );
    printf( " %10.3f\n", distance );
}

/**
    Prints the header of the table.
  */
void printHeader()
{
    printf( "\n     angle |         v0 |       time |   distance\n" );
    printf( "-----------+------------+------------+-----------\n" );
}

/**
    Asks the user for an initial velocity. Uses the inputted velocity to print
    a ballistics table which includes angle, velocity, flight time,
    and horizontal distance traveled.
    @return exit status
  */
int main()
{
    double iniV = 0.0;
    printf( "V0: " );
    scanf( "%lf", &iniV );
    printHeader();
    int ang;
    for ( ang = 0; ang <= MAX_ANGLE; ang += ANGLE_STEP ) {
        double time = flightTime( ang, iniV );
        tableRow( ang, iniV, time );
    }
    return EXIT_SUCCESS;
}