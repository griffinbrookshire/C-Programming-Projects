/**
    @file command.h
    @author Griffin Brookshire (glbrook2)
    Defines behavior and data for command.c
*/

#ifndef PARSE_H
#define PARSE_H

#include "packet.h"
#include "policy.h"

/** BITS bits */
#define BITS 8

/**
    Representation for a command that has been parsed.
    TYPES
    1 - help
    2 - default
    3 - insert
    4 - append
    5 - delete
    6 - test
    7 - print
    8 - quit
*/
typedef struct fw_cmd {
    int command_type;
    int default_pol; // 0 = allow, 1 = deny
    int trans_lay_prot; // 0 = tcp, 1 = udp
    unsigned int src_a:BITS;
    unsigned int src_b:BITS;
    unsigned int src_c:BITS;
    unsigned int src_d:BITS;
    int src_prt;
    unsigned int dst_a:BITS;
    unsigned int dst_b:BITS;
    unsigned int dst_c:BITS;
    unsigned int dst_d:BITS;
    int dst_prt;
    int pos;
    int all; // 0 = no, 1 = all
} fw_cmd_t;

/**
    Parses a command.
    @param line The string representation of a command
    @param cmd The command that is filled
    @return 1 if success, 0 if not
  */
int parse_command(char *line, fw_cmd_t *cmd);

#endif
