/**
    @file policy.h
    @author Griffin Brookshire (glbrook2)
    Defines behavior and data of the firewall policy.
*/

#ifndef POLICY_H
#define POLICY_H

#include "packet.h"

/** Used to indicate an allow rule. */
#define ACTION_ALLOW   0

/** Used to indicate a deny rule. */
#define ACTION_DENY    1

/**
 * Representation of a firewall rule
 * .action: the rule action (ACTION_ALLOW or ACTION_DENY)
 * .match: the packet match
 */
typedef struct rule {
    unsigned int    action;
    packet_match_t  match;
} rule_t;

/**
    This function will set the default policy to the specified action.
    The starter files includes #define's for ACTION_ALLOW and ACTION_DENY.
    It returns 0 if successful, -1 if unsuccessful.
    @param action The default to set to
    @return 0 if success, -1 if fail
*/
int policy_set_default(int action);

/**
    This function will append a rule to the policy.
    It returns 0 if successful, -1 if unsuccessful.
    @param rule The rule to add
    @return 0 if success, -1 if fail
*/
int policy_append(rule_t rule);

/**
    This function will initialize the dynamically allocated policy structure.
    It returns 0 if successful, -1 if unsuccessful.
    @return 0 if success, -1 if fail
*/
int policy_init();


/**
    This function will free the dynamically allocated policy
    structure and re-initialize values as appropriate.
*/
void policy_free();

/**
    This function will insert a rule at position @pos.
    It returns 0 if successful, -1 if unsuccessful.
    @param rule Rule to insert
    @param pos The position to insert at
    @return 0 if success, -1 if fail
*/
int policy_insert(rule_t rule, int pos);

/**
    This function will delete the rule at position @pos.
    It returns 0 if successful, -1 if unsuccessful.
    @param pos The position to delete
    @return 0 if success, -1 if fail
*/
int policy_delete(int pos);

/**
    This function will test if @pkt is allowed or denied by the policy.
    It returns ACTION_ALLOW or ACTION_DENY.
    Additionally the value pointed to by @pos will be
    updated with the position number of the rule that is matched.
    If no rule is matched, the value will be set to -1.
    @param pkt The packet to test
    @param pos The position to test
    @return ACTION_ALLOW if success, ACTION_DENY if fail
*/
int policy_test(packet_t pkt, int *pos);

/**
    This function will print to @stream the rule at position @pos.
    It returns 0 if successful and -1 if unsuccessful
    @param stream Stream to print to
    @param pos The position to print
    @return 0 if success, -1 if fail
*/
int policy_print_rule(FILE *stream, int pos);

/**
    This function will print the default policy
    followed by the policy rules in order to @stream.
    @param stream Stream to print to
*/
void policy_print(FILE *stream);

#endif
