/**
    @file fwsim.c
    @author Griffin Brookshire (glbrook2)
    Main code for a firewall system. Allows user to allow or deny
    specific connections and test the outcome.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "packet.h"
#include "policy.h"
#include "command.h"

/** Command prompt shown to the user. */
#define PROMPT "> "

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

/** Line size */
#define BUFFER 64

/** Max number of command line args */
#define MAX_ARGS 3

/* Print out a usage message. */
static void usage()
{
  fprintf(stderr, "Usage: fwsim [-h] [-r <rule_file>]\n");
}

/**
    Executes a command.
    @param cmd The command to execute
    @return 1 if success, 0 if not
  */
static int execute_command( fw_cmd_t *cmd ) {
  if ( cmd->command_type == HELP ) { //help
    fprintf( stdout, "Firewall Command Language:\n\ndefault (allow|deny)\ninsert " );
    fprintf( stdout, "<pos> (allow|deny) (tcp|udp) <src_ip>:(*|<src_port>) <dst_ip>:" );
    fprintf( stdout, "(*|<dst_port>)\nappend (allow|deny) (tcp|udp) <src_ip>:" );
    fprintf( stdout, "(*|<src_port>) <dst_ip>:(*|<dst_port>)\ndelete <pos>\ntest " );
    fprintf( stdout, "(tcp|udp) <src_ip>:<src_port> <dst_ip>:<dst_port>\nprint " );
    fprintf( stdout, "(all|<pos>)\nhelp\nquit\n" );
    return 0;
  } else if ( cmd->command_type == DEFAULT ) { //default
    policy_set_default( cmd->default_pol );
    return 0;
  } else if ( cmd->command_type == INSERT ) { //insert
    /** Create rule */
    rule_t *rule = ( rule_t * )malloc( sizeof( rule_t ) );
    rule->action = cmd->default_pol;
    /** Create packet for rule */
    packet_match_t pack;
    pack.protocol = cmd->trans_lay_prot; // 0 = tcp, 1 = udp
    pack.src_ip.a = cmd->src_a;
    pack.src_ip.b = cmd->src_b;
    pack.src_ip.c = cmd->src_c;
    pack.src_ip.d = cmd->src_d;
    pack.src_port = cmd->src_prt;
    pack.dst_ip.a = cmd->dst_a;
    pack.dst_ip.b = cmd->dst_b;
    pack.dst_ip.c = cmd->dst_c;
    pack.dst_ip.d = cmd->dst_d;
    pack.dst_port = cmd->dst_prt;
    /** Add packet to rule and append */
    rule->match = pack;
    policy_insert( *rule, cmd->pos );
    return 0;
  } else if ( cmd->command_type == APPEND ) { //append
    /** Create rule */
    rule_t *rule = ( rule_t * )malloc( sizeof( rule_t ) );
    rule->action = cmd->default_pol;
    /** Create packet for rule */
    packet_match_t pack;
    pack.protocol = cmd->trans_lay_prot; // 0 = tcp, 1 = udp
    pack.src_ip.a = cmd->src_a;
    pack.src_ip.b = cmd->src_b;
    pack.src_ip.c = cmd->src_c;
    pack.src_ip.d = cmd->src_d;
    pack.src_port = cmd->src_prt;
    pack.dst_ip.a = cmd->dst_a;
    pack.dst_ip.b = cmd->dst_b;
    pack.dst_ip.c = cmd->dst_c;
    pack.dst_ip.d = cmd->dst_d;
    pack.dst_port = cmd->dst_prt;
    /** Add packet to rule and append */
    rule->match = pack;
    policy_append( *rule );
    //free( rule );
    return 0;
  } else if ( cmd->command_type == DELETE ) { //delete
    policy_delete( cmd->pos );
    return 0;
  } else if ( cmd->command_type == TEST ) { //test
    /** Create packet to test */
    packet_t pack;
    pack.protocol = cmd->trans_lay_prot; // 0 = tcp, 1 = udp
    pack.src_ip.a = cmd->src_a;
    pack.src_ip.b = cmd->src_b;
    pack.src_ip.c = cmd->src_c;
    pack.src_ip.d = cmd->src_d;
    pack.src_port = cmd->src_prt;
    pack.dst_ip.a = cmd->dst_a;
    pack.dst_ip.b = cmd->dst_b;
    pack.dst_ip.c = cmd->dst_c;
    pack.dst_ip.d = cmd->dst_d;
    pack.dst_port = cmd->dst_prt;
    int pos = -1;
    int *posp = &pos;
    policy_test( pack, posp );
    return 0;
  } else if ( cmd->command_type == PRINT ) { //print
    if ( cmd->all == 1 ) { // all
      policy_print( stdout );
    } else { // pos
      policy_print_rule( stdout, cmd->pos );
    }
    return 0;
  } else { //quit
    return -1;
  }
  return -1;
}

/* Load firewall rules from a file.
   @param filename name of a file from which to load the rules.
*/
static void load_rules(char *filename)
{
  FILE *file = fopen( filename, "r" );
  if ( file == NULL ) {
    fprintf( stdout, "Could not open file.\n" );
    exit( 1 );
  }
  char line[ BUFFER ];
  while ( fgets( line, sizeof( line ), file ) ) {
    int size = strlen( line );
    line[ size - 1 ] = '\0';
    fw_cmd_t cmd;
    fw_cmd_t *cmd_ptr = &cmd;
    int par = parse_command( line, cmd_ptr );
    if ( par == -1 ) {
      continue;
    }
    int exe = execute_command( cmd_ptr );
    if ( exe == -1 ) {
      fclose( file );
      exit( 0 );
    }
  }
  fclose( file );
  return;
}

/* Starting point for the program.  Process command-line arguments then
   read and execute user commands.
   @param argc number of command-line arguments.
   @param argv list of command-line arguments.
   @return program exit status
*/
int main(int argc, char *argv[])
{

  if ( argc != 1 && argc != MAX_ARGS ) { //either ./fwsim or ./fwsim -r <filename>
    usage();
    exit( 1 );
  }

  policy_init();

  if ( argc == MAX_ARGS ) { // ./fwsim -r <filename>
    if ( strcmp( argv[ 1 ], "-r" ) == 0 ) {
      load_rules( argv[ 2 ] );
    } else {
      usage();
      exit( 1 );
    }
  }

  char line[ BUFFER ];
  while ( true ) {
    fprintf( stdout, PROMPT );
    char *test = fgets( line, sizeof( line ), stdin );
    if ( test == NULL ) {
      break;
    }
    int size = strlen( line );
    line[ size - 1 ] = '\0';
    fw_cmd_t cmd;
    fw_cmd_t *cmd_ptr = &cmd;
    int par = parse_command( line, cmd_ptr );
    if ( par == -1 ) {
      continue;
    }
    int exe = execute_command( cmd_ptr );
    if ( exe == -1 ) {
      exit( 0 );
    }
  }

  policy_free();
  return EXIT_SUCCESS;
}
