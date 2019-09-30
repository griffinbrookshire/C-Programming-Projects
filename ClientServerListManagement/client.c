#include "common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

int main( int argc, char *argv[] ) {

  // check args
  if ( argc < 2 || argc > 3 ) {
    printf( "Invalid command\n" );
    exit( 1 );
  }

  // Make both the server and client message queues.
  mqd_t serverQueue = mq_open( SERVER_QUEUE, O_WRONLY );
  mqd_t clientQueue = mq_open( CLIENT_QUEUE, O_RDONLY );
  if ( serverQueue == -1 || clientQueue == -1 ) {
    fail( "Can't open the needed message queues" );
  }
  
  // buffer for reading/writing messages
  char buffer[ MESSAGE_LIMIT ] = { 0 };

  // check command
  if ( strcmp( "report", argv[ 1 ] ) == 0 ) {

    int errsv = mq_send( serverQueue, argv[ 1 ], strlen( argv[ 1 ] ), 0 );
    if ( errsv < 0 )
      fail( "Send failed");
    memset( buffer, 0, sizeof( buffer ) );
    int len = mq_receive( clientQueue, buffer, sizeof( buffer ), NULL );
    int numNames;
    if ( len > 0 ) {
      numNames = buffer[ 0 ] - '0';
    } else {
      fail( "Unable to receive message." );
    }
    for ( int i = 0; i < numNames; i++ ) {
      memset( buffer, 0, sizeof( buffer ) );
      mq_receive( clientQueue, buffer, sizeof( buffer ), NULL );
      printf("%s\n", buffer);
    }

  } else if ( strcmp( "promote", argv[ 1 ] ) == 0 ) {

    int errsv = mq_send( serverQueue, argv[ 1 ], strlen( argv[ 1 ] ), 0 );
    if ( errsv < 0 )
      fail( "Send failed");
    errsv = mq_send( serverQueue, argv[ 2 ], strlen( argv[ 2 ] ), 0 );
    if ( errsv < 0 )
      fail( "Send failed");
    memset( buffer, 0, sizeof( buffer ) );
    mq_receive( clientQueue, buffer, sizeof( buffer ), NULL );
    printf("%s\n", buffer);

  } else if ( strcmp( "demote", argv[ 1 ] ) == 0 ) {

    int errsv = mq_send( serverQueue, argv[ 1 ], strlen( argv[ 1 ] ), 0 );
    if ( errsv < 0 )
      fail( "Send failed");
    errsv = mq_send( serverQueue, argv[ 2 ], strlen( argv[ 2 ] ), 0 );
    if ( errsv < 0 )
      fail( "Send failed");
    memset( buffer, 0, sizeof( buffer ) );
    mq_receive( clientQueue, buffer, sizeof( buffer ), NULL );
    printf("%s\n", buffer);


  } else {
    printf( "Invalid command\n" );
    exit( 1 );
  }

}
