/**
    @file command.c
    @author Griffin Brookshire (glbrook2)
    Parses a string describing a command into a command struct.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "command.h"

/** Help cmd type */
#define HELP 1

/** Default cmd type */
#define DEFAULT 2

/** Insert cmd type */
#define INSERT 3

/** Append cmd type */
#define APPEND 4

/** Delete cmd type */
#define DELETE 5

/** Test cmd type */
#define TEST 6

/** Print cmd type */
#define PRINT 7

/** Quit cmd type */
#define QUIT 8

/** BITS bits */
#define BITS 8

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
    Parses a command.
    @param line The string representation of a command
    @param cmd The command that is filled
    @return 1 if success, 0 if not
  */
int parse_command(char *line, fw_cmd_t *cmd) {
  char *word = strtok( line, " " );


  if ( strcmp( word, "help" ) == 0 ) {
    cmd->command_type = HELP;
    return 0;


  } else if ( strcmp( word, "default" ) == 0 ) {
    cmd->command_type = DEFAULT;
    word = strtok( NULL, " " );
    if ( strcmp( word, "allow" ) == 0 ) {
      cmd->default_pol = ACTION_ALLOW;
      return 0;
    } else if ( strcmp( word, "deny" ) == 0 ) {
      cmd->default_pol = ACTION_DENY;
      return 0;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }


  } else if ( strcmp( word, "insert" ) == 0 ) {
    cmd->command_type = INSERT;
    word = strtok( NULL, " " );
    if ( isNumber( word ) ) {
      int pos = atoi( word );
      cmd->pos = pos;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    if ( strcmp( word, "allow" ) == 0 ) {
      cmd->default_pol = ACTION_ALLOW;
    } else if ( strcmp( word, "deny" ) == 0 ) {
      cmd->default_pol = ACTION_DENY;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    if ( strcmp( word, "tcp" ) == 0 ) {
      cmd->trans_lay_prot = PROTO_TCP;
    } else if ( strcmp( word, "udp" ) == 0 ) {
      cmd->trans_lay_prot = PROTO_UDP;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int a = atoi( word );
      if ( a < IP_OCTET_MIN || a > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_a = a;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int b = atoi( word );
      if ( b < IP_OCTET_MIN || b > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_b = b;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int c = atoi( word );
      if ( c < IP_OCTET_MIN || c > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_c = c;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, ":" );
    if ( isNumber( word ) ) {
      int d = atoi( word );
      if ( d < IP_OCTET_MIN || d > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_d = d;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    int port;
    if ( strcmp( word, "*" ) == 0 ) {
      cmd->src_prt = -1;
    } else if ( isNumber( word ) ) {
      port = atoi( word );
      if ( port < PORT_MIN || port > PORT_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_prt = port;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int a = atoi( word );
      if ( a < IP_OCTET_MIN || a > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_a = a;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int b = atoi( word );
      if ( b < IP_OCTET_MIN || b > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_b = b;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int c = atoi( word );
      if ( c < IP_OCTET_MIN || c > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_c = c;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, ":" );
    if ( isNumber( word ) ) {
      int d = atoi( word );
      if ( d < IP_OCTET_MIN || d > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_d = d;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    if ( strcmp( word, "*" ) == 0 ) {
      cmd->src_prt = -1;
    } else if ( isNumber( word ) ) {
      port = atoi( word );
      if ( port < PORT_MIN || port > PORT_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_prt = port;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    return 0;


  } else if ( strcmp( word, "append" ) == 0 ) {
    cmd->command_type = APPEND;
    word = strtok( NULL, " " );
    if ( strcmp( word, "allow" ) == 0 ) {
      cmd->default_pol = ACTION_ALLOW;
    } else if ( strcmp( word, "deny" ) == 0 ) {
      cmd->default_pol = ACTION_DENY;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    if ( strcmp( word, "tcp" ) == 0 ) {
      cmd->trans_lay_prot = PROTO_TCP;
    } else if ( strcmp( word, "udp" ) == 0 ) {
      cmd->trans_lay_prot = PROTO_UDP;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      unsigned int a = atoi( word );
      if ( a < IP_OCTET_MIN || a > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_a = a;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      unsigned int b = atoi( word );
      if ( b < IP_OCTET_MIN || b > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_b = b;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      unsigned int c = atoi( word );
      if ( c < IP_OCTET_MIN || c > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_c = c;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, ":" );
    if ( isNumber( word ) ) {
      unsigned int d = atoi( word );
      if ( d < IP_OCTET_MIN || d > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_d = d;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    int port;
    if ( strcmp( word, "*" ) == 0 ) {
      cmd->src_prt = -1;
    } else if ( isNumber( word ) ) {
      port = atoi( word );
      if ( port < PORT_MIN || port > PORT_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_prt = port;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int a = atoi( word );
      if ( a < IP_OCTET_MIN || a > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_a = a;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int b = atoi( word );
      if ( b < IP_OCTET_MIN || b > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_b = b;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int c = atoi( word );
      if ( c < IP_OCTET_MIN || c > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_c = c;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, ":" );
    if ( isNumber( word ) ) {
      int d = atoi( word );
      if ( d < IP_OCTET_MIN || d > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_d = d;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    if ( strcmp( word, "*" ) == 0 ) {
      cmd->src_prt = -1;
    } else if ( isNumber( word ) ) {
      port = atoi( word );
      if ( port < PORT_MIN || port > PORT_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_prt = port;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    return 0;


  } else if ( strcmp( word, "delete" ) == 0 ) {
    cmd->command_type = DELETE;
    word = strtok( NULL, ":" );
    if ( isNumber( word ) ) {
      cmd->pos = atoi( word );
    }
    return 0;


  } else if ( strcmp( word, "test" ) == 0 ) {
    cmd->command_type = TEST;
    word = strtok( NULL, " " );
    if ( strcmp( word, "tcp" ) == 0 ) {
      cmd->trans_lay_prot = PROTO_TCP;
    } else if ( strcmp( word, "udp" ) == 0 ) {
      cmd->trans_lay_prot = PROTO_UDP;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      unsigned int a = atoi( word );
      if ( a < IP_OCTET_MIN || a > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_a = a;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      unsigned int b = atoi( word );
      if ( b < IP_OCTET_MIN || b > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_b = b;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      unsigned int c = atoi( word );
      if ( c < IP_OCTET_MIN || c > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_c = c;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, ":" );
    if ( isNumber( word ) ) {
      unsigned int d = atoi( word );
      if ( d < IP_OCTET_MIN || d > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_d = d;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    int port;
    if ( isNumber( word ) ) {
      port = atoi( word );
      if ( port < PORT_MIN || port > PORT_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->src_prt = port;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int a = atoi( word );
      if ( a < IP_OCTET_MIN || a > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_a = a;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int b = atoi( word );
      if ( b < IP_OCTET_MIN || b > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_b = b;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, "." );
    if ( isNumber( word ) ) {
      int c = atoi( word );
      if ( c < IP_OCTET_MIN || c > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_c = c;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, ":" );
    if ( isNumber( word ) ) {
      int d = atoi( word );
      if ( d < IP_OCTET_MIN || d > IP_OCTET_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_d = d;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    word = strtok( NULL, " " );
    if ( isNumber( word ) ) {
      port = atoi( word );
      if ( port < PORT_MIN || port > PORT_MAX ) {
        fprintf( stdout, "Error: Could not parse command.\n" );
        return -1;
      }
      cmd->dst_prt = port;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    return 0;


  } else if ( strcmp( word, "print" ) == 0 ) {
    cmd->command_type = PRINT;
    word = strtok( NULL, " " );
    if ( strcmp( word, "all" ) == 0 ) {
      cmd->all = 1;
      return 0;
    } else if ( isNumber( word ) ) {
      int pos = atoi( word );
      cmd->pos = pos;
      return 0;
    } else {
      fprintf( stdout, "Error: Could not parse command.\n" );
      return -1;
    }
    return 0;


  } else if ( strcmp( word, "quit" ) == 0 ){
    cmd->command_type = QUIT;
    return 0;
  }
  fprintf( stdout, "Error: Could not parse command.\n" );
  return -1;
}
