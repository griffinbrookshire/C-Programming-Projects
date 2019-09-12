/**
    @file policy.c
    @author Griffin Brookshire (glbrook2)
    Holds the firewall policy, a list of rules.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "policy.h"

/**
 * The initial allocation size of the policy
 */
#define POLICY_INIT_SIZE 10

/**
 * The global firewall policy, internally managed
 */
static rule_t **policy = NULL;

/**
 * The current number of policy in the policy
 */
static int policy_len = 0;

/**
 * The current capacity for storing policy policy
 */
static int policy_cap = 0;

/**
 * The default policy action
 */
static int policy_default = ACTION_DENY;

/** Holds the rules */
static rule_t *rules[ POLICY_INIT_SIZE ] = { NULL };

static void grow_array() {
  //printf("got to grow\n");
  //policy = ( rule_t ** )realloc( policy[ 0 ], ( policy_len + policy_len ) * sizeof( rule_t * ) );
  //int new_size = policy_len + policy_len;
  //rule_t *new_rules[ new_size ] = { NULL };
  //for ( int i = 0; i < new_size; i++ ) {
    //new_rules[ i ] = ( rule_t * )malloc( sizeof( rule_t * ) );
  //}
  //rules = &new_rules[ 0 ];
  //policy = rules;
  //printf("grew to size: %d\n", new_size);
  //policy_cap = new_size;
}

/**
    This function will initialize the dynamically allocated policy structure.
    It returns 0 if successful, -1 if unsuccessful.
    @return 0 if success, -1 if fail
*/
int policy_init() {
  for ( int i = 0; i < POLICY_INIT_SIZE; i++ ) {
    rules[ i ] = ( rule_t * )malloc( sizeof( rule_t * ) );
  }
  //policy = ( rule_t ** )malloc( POLICY_INIT_SIZE * sizeof( rule_t * ) );
  //policy = &rules[ 0 ];
  policy = rules;
  if ( policy == NULL ) {
    return -1;
  }
  policy_cap = POLICY_INIT_SIZE;
  return 0;
}

/**
    This function will free the dynamically allocated policy
    structure and re-initialize values as appropriate.
*/
void policy_free() {
  for ( int i = 0; i < policy_len; i++ ) {
    free( rules[ i ] );
  }
  for ( int j = 0; j < policy_len; j++ ) {
    //free( policy[ j ][ 0 ] );
  }
}

/**
    This function will set the default policy to the specified action.
    The starter files includes #define's for ACTION_ALLOW and ACTION_DENY.
    It returns 0 if successful, -1 if unsuccessful.
    @param action The default to set to
    @return 0 if success, -1 if fail
*/
int policy_set_default(int action) {
  policy_default = action;
  return 0;
}

/**
    This function will append a rule to the policy.
    It returns 0 if successful, -1 if unsuccessful.
    @param rule The rule to add
    @return 0 if success, -1 if fail
*/
int policy_append(rule_t rule) {
  if ( policy_len == policy_cap - 1 ) {
    //printf("About to grow\n");
    grow_array();
  }
  //policy_print_rule( stdout, 9 );
  *( policy[ policy_len ] ) = rule;
  //printf("After one append\n");
  policy_len++;
  return 0;
}

/**
    This function will insert a rule at position @pos.
    It returns 0 if successful, -1 if unsuccessful.
    @param rule Rule to insert
    @param pos The position to insert at
    @return 0 if success, -1 if fail
*/
int policy_insert(rule_t rule, int pos) {
  if ( pos <= 0 ) {
    fprintf( stdout, "Error: Could not add rule.\n" );
    return -1;
  }
  if ( policy_len == policy_cap - 1 ) {
    grow_array();
  }
  if ( pos > policy_len ) {
    policy_append( rule );
    return 0;
  }
  pos = pos - 1;
  for ( int i = policy_len - 1; i >= pos; i-- ) {
    rule_t rulei = policy[ i ][ 0 ];
    //policy[ i + 1 ] = &( policy[ i ][ 0 ] );
    *( policy[ i + 1 ] ) = rulei;
  }
  *( policy[ pos ] ) = rule;
  policy_len++;
  return 0;
}

/**
    This function will delete the rule at position @pos.
    It returns 0 if successful, -1 if unsuccessful.
    @param pos The position to delete
    @return 0 if success, -1 if fail
*/
int policy_delete(int pos) {
  if ( pos <= 0 || pos > policy_len ) {
    fprintf( stdout, "Error: Could not delete rule.\n" );
    return -1;
  }
  pos = pos - 1;
  for ( int i = pos; i < policy_len; i++ ) {
    policy[ i ] = &( policy[ i + 1 ][ 0 ] );
  }
  policy_len--;
  return 0;
}

/**
    This function will test if @pkt is allowed or denied by the policy.
    It returns ACTION_ALLOW or ACTION_DENY.
    Additionally the value pointed to by @pos will be updated
    with the position number of the rule that is matched.
    If no rule is matched, the value will be set to -1.
    @param pkt The packet to test
    @param pos The position to test
    @return ACTION_ALLOW if success, ACTION_DENY if fail
*/
int policy_test(packet_t pkt, int *pos) {
  for ( int i = 0; i < policy_len; i++ ) {
    int test = packet_match( policy[ i ]->match, pkt );
    if ( test == 1 ) { // if packet is matched with a rule
      if ( policy[ i ]->action == ACTION_ALLOW ) { // if rule says allowed
        fprintf( stdout, "Allowed via " );
        policy_print_rule( stdout, i + 1 );
        return ACTION_ALLOW; // then return allow
      } else {
        fprintf( stdout, "Denied via " );
        policy_print_rule( stdout, i + 1 );
        return ACTION_DENY; // if not, then return deny
      }
    } // if
  }
  if ( policy_default == ACTION_DENY ) {
    fprintf( stdout, "Denied via default policy.\n" );
  } else {
    fprintf( stdout, "Allowed via default policy.\n" );
  }
  return 0;
}

/**
    This function will print to @stream the rule at position @pos.
    It returns 0 if successful and -1 if unsuccessful
    @param stream Stream to print to
    @param pos The position to print
    @return 0 if success, -1 if fail
*/
int policy_print_rule(FILE *stream, int pos) {
  fprintf( stream, "[%d] ", ( pos ) );
  if ( pos <= 0 || pos > policy_len ) {
    fprintf( stream, "\nError: Rule %d does not exist.\n", ( pos ) );
    return -1;
  }
  pos = pos - 1;
  //fprintf( stream, "The port of rule %d is: %d \n", pos, policy[ pos ]->match.dst_port );
  if ( policy[ pos ]->action == 0 ) {
    fprintf( stream, "allow " );
  } else {
    fprintf( stream, "deny " );
  }
  if ( policy[ pos ]->match.protocol == 0 ) {
    fprintf( stream, "tcp " );
  } else {
    fprintf( stream, "udp " );
  }
  fprintf( stream, "%d.", policy[ pos ]->match.src_ip.a );
  fprintf( stream, "%d.", policy[ pos ]->match.src_ip.b );
  fprintf( stream, "%d.", policy[ pos ]->match.src_ip.c );
  fprintf( stream, "%d:", policy[ pos ]->match.src_ip.d );
  if ( policy[ pos ]->match.src_port < 0 ) {
    fprintf( stream, "* " );
  } else {
    fprintf( stream, "%d ", policy[ pos ]->match.src_port );
  }
  fprintf( stream, "%d.", policy[ pos ]->match.dst_ip.a );
  fprintf( stream, "%d.", policy[ pos ]->match.dst_ip.b );
  fprintf( stream, "%d.", policy[ pos ]->match.dst_ip.c );
  fprintf( stream, "%d:", policy[ pos ]->match.dst_ip.d );
  if ( policy[ pos ]->match.dst_port < 0 ) {
    fprintf( stream, "* \n" );
  } else {
    fprintf( stream, "%d \n", policy[ pos ]->match.dst_port );
  }
  return 0;
}

/**
    This function will print the default policy
    followed by the policy rules in order to @stream.
    @param stream Stream to print to
*/
void policy_print(FILE *stream) {
  fprintf( stream, "default " );
  if ( policy_default == ACTION_DENY ) {
    fprintf( stream, "deny\n" );
  } else {
    fprintf( stream, "allow\n" );
  }
  for ( int i = 1; i <= policy_len; i++ ) {
    policy_print_rule( stream, i );
  }
}
