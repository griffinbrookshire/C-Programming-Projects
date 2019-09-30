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

// Makes a list
RankList *makeList( int numArgs, char *names[] ) {
  RankList *list = ( RankList * )malloc( sizeof( RankList ) );
  for ( int i = 1; i < numArgs; i++ ) {
    Node *node = (Node *)malloc( sizeof( Node ) );
    node->str = names[ i ];
    if ( i == 1 ) { // things to do for just the first node
      node->prev = NULL;
      list->head = node;
    } else { // things to do for every other node
      list->tail->next = node;
      node->prev = list->tail;
    } // things to do for all nodes
      node->next = NULL;
      list->tail = node;
  }
  return list;
}

//Prints the list
void report( RankList *list ) {
  Node *temp = list->head;
  while( temp ) {
    printf( "%s\n", temp->str );
    temp = temp->next;
  }
}

// Frees the list
void freeList( RankList *list ) {
  Node *temp;
  while( list->head != NULL ) {
    temp = list->head;
    list->head = list->head->next;
    free( temp );
  }
}

// Flag for telling the server to stop running because of a sigint.
// This is safer than trying to print in the signal handler.
static int running = 1;

// Instance of the name list.
RankList *list;

// Alarm handler
void quitHandler( int signal ) {
  printf("\n");
  report( list );
  //running = 0;
  exit( 0 );
}

bool promote( char *name ) {
  Node *temp = list->head;
  while ( temp ) {
    if ( strcmp( name, temp->str ) == 0 ) {
      if ( temp->prev ) { // check if this is top of list
        char *tempBuf = temp->str;
        temp->str = temp->prev->str;
        temp->prev->str = tempBuf;
        return true;
      } else 
        break;
    }
    temp = temp->next;
  }
  return false;
}

bool demote( char *name ) {
  Node *temp = list->head;
  while ( temp ) {
    if ( strcmp( name, temp->str ) == 0 ) {
      if ( temp->next ) { // check if this is bottom of list
        char *tempBuf = temp->str;
        temp->str = temp->next->str;
        temp->next->str = tempBuf;
        return true;
      } else 
        break;
    }
    temp = temp->next;
  }
  return false;
}

int main( int argc, char *argv[] ) {

  int names = argc - 1;
  char namesChar[ 1 ] = { names + '0' };

  // check args
  if ( names < LIST_MIN || names > LIST_MAX )
    fail( "Please supply 2 to 6 names." );
  for ( int i = 1; i < argc; i++ ) {
    if ( ( int )( strlen( argv[ i ] ) ) > NAME_MAX )
      fail( "Names must be less than 12 characters." );
    if ( strpbrk( argv[ i ], " " ) != NULL )
      fail( "Names must not contain spaces." );
  }

  struct sigaction act;
  act.sa_handler = quitHandler;
  sigemptyset( &( act.sa_mask ) );
  act.sa_flags = 0;
  sigaction( SIGINT, &act, 0 );

  // Remove both queues, in case, last time, this program terminated
  // abnormally with some queued messages still queued.
  mq_unlink( SERVER_QUEUE );
  mq_unlink( CLIENT_QUEUE );

  // Prepare structure indicating maximum queue and message sizes.
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 1;
  attr.mq_msgsize = MESSAGE_LIMIT;

  // Make both the server and client message queues.
  mqd_t serverQueue = mq_open( SERVER_QUEUE, O_RDONLY | O_CREAT, 0600, &attr );
  mqd_t clientQueue = mq_open( CLIENT_QUEUE, O_WRONLY | O_CREAT, 0600, &attr );
  if ( serverQueue == -1 || clientQueue == -1 )
    fail( "Can't create the needed message queues" );

  // fill the list with names
  list = makeList( argc, argv );

  // Repeatedly read and process client messages.
  while ( running ) {

    // buffer for reading/writing messages
    char buffer[ MESSAGE_LIMIT ] = { 0 };

    int len = mq_receive( serverQueue, buffer, sizeof( buffer ), NULL );
    if ( len > 0 ) {
    } else
      fail( "Unable to receive message." );

    if ( strcmp( buffer, "report" ) == 0 ) {
      int errsv = mq_send( clientQueue, namesChar, sizeof( char ), 0 );
      if ( errsv < 0 )
        fail( "Send 'a' failed");
      Node *temp = list->head;
      for ( int i = 0; i < names; i++ ) {
        memset( buffer, 0, sizeof( buffer ) );
        strncpy( buffer, temp->str, strlen( temp->str ) );
        int errsv = mq_send( clientQueue, buffer, strlen( buffer ), 0 );
        if ( errsv < 0 )
          fail( "Send report failed");
        temp = temp->next;
      }
    } else if ( strcmp( buffer, "promote" ) == 0 ) {
      
      memset( buffer, 0, sizeof( buffer ) );
      int len = mq_receive( serverQueue, buffer, sizeof( buffer ), NULL );
      if ( len < 0 ) {
        fail( "Unable to receive message." );
      }
      bool prom = promote( buffer );
      char *resp;
      if ( prom ) {
        resp = "OK";
      } else {
        resp = "Invalid command";
      }
      int errsv = mq_send( clientQueue, resp, strlen( resp ), 0 );
      if ( errsv < 0 )
        fail( "Send report failed");

    } else if ( strcmp( buffer, "demote" ) == 0 ) {

      memset( buffer, 0, sizeof( buffer ) );
      int len = mq_receive( serverQueue, buffer, sizeof( buffer ), NULL );
      if ( len < 0 ) {
        fail( "Unable to receive message." );
      }
      bool dem = demote( buffer );
      char *resp;
      if ( dem ) {
        resp = "OK";
      } else {
        resp = "Invalid command";
      }
      int errsv = mq_send( clientQueue, resp, strlen( resp ), 0 );
      if ( errsv < 0 )
        fail( "Send report failed");

    }

  }

  // Free the list
  freeList( list );

  // Close our two message queues (and delete them).
  mq_close( clientQueue );
  mq_close( serverQueue );

  // Delete the message queues, this is for normal server termination.
  mq_unlink( SERVER_QUEUE );
  mq_unlink( CLIENT_QUEUE );

  return EXIT_SUCCESS;
}
