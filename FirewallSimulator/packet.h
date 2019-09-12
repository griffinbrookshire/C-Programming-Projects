/**
    @file packet.h
    @author Griffin Brookshire (glbrook2)
    Defines data and behavior for a packet.
*/

#ifndef PACKET_H
#define PACKET_H

/** Protocol value indicating TCP */
#define PROTO_TCP      0

/** Protocol value indicating UDP */
#define PROTO_UDP      1

/** Maximum possible port number */
#define PORT_MAX 0xFFFF

/** Minimum possible port number */
#define PORT_MIN 0

/** Maximum possible octet value */
#define IP_OCTET_MAX 255

/** minimum possible octet value */
#define IP_OCTET_MIN 0

/** BITS bits */
#define BITS 8

/**
 * Structure to store an IP address as a.b.c.d
 * where each part may hold values 0-255
 */
typedef struct ipaddr {
    unsigned int a:BITS;
    unsigned int b:BITS;
    unsigned int c:BITS;
    unsigned int d:BITS;
} ipaddr_t;

typedef unsigned int protocol_t;
typedef unsigned short port_t;
typedef int port_match_t;

/**
 * Structure to store information of a packet
 * .protocol: the transport protocol (PROTO_TCP or PROTO_UDP)
 * .src_ip: the source IP address
 * .src_port: the source port address
 * .dst_ip: the destionation IP address
 * .dst_port: the destination port address
 */
typedef struct packet {
    protocol_t    protocol;
    ipaddr_t      src_ip;
    port_t        src_port; //unsigned short
    ipaddr_t      dst_ip;
    port_t        dst_port; //unsigned short
} packet_t;

/**
 * Used in packet_match_t for ports if any port should match
 */
#define MATCH_PORT_ANY -1


/**
 * Structure used to match packets (used in rules)
 * .protocol: the transport protocol (PROTO_TCP or PROTO_UDP)
 * .src_ip: the source IP address to match
 * .src_port: the source port address to match (may be MATCH_PORT_ANY)
 * .dst_ip: the destionation IP address to match
 * .dst_port: the destination port address to match (may be MATCH_PORT_ANY)
 */
typedef struct packet_match {
    protocol_t      protocol;
    ipaddr_t        src_ip;
    port_match_t    src_port; //int
    ipaddr_t        dst_ip;
    port_match_t    dst_port; //int
} packet_match_t;

/**
    This function checks if @packet is matched by @match.
    It returns 1 if match and 0 if no match.
    @param match Packet to check
    @param packet Packet to check
    @return 0 if no match, 1 if match
*/
int packet_match(packet_match_t match, packet_t packet);

#endif
