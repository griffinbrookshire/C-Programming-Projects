/**
    @file packet.c
    @author Griffin Brookshire (glbrook2)
    Tests if a packet if allowed or denied passage.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "packet.h"
#include "policy.h"
#include "command.h"

/**
    This function checks if @packet is matched by @match.
    It returns 1 if match and 0 if no match.
    @param match Packet to check
    @param packet Packet to check
    @return 0 if no match, 1 if match
*/
int packet_match(packet_match_t match, packet_t packet) {
  if ( match.protocol != packet.protocol ) {
    return 0;
  }
  if ( match.src_ip.a != packet.src_ip.a ) {
    return 0;
  }
  if ( match.src_ip.b != packet.src_ip.b ) {
    return 0;
  }
  if ( match.src_ip.c != packet.src_ip.c ) {
    return 0;
  }
  if ( match.src_ip.d != packet.src_ip.d ) {
    return 0;
  }
  if ( match.src_port != -1 ) {
    if ( match.src_port != packet.src_port ) {
      return 0;
    }
  }
  if ( match.dst_ip.a != packet.dst_ip.a ) {
    return 0;
  }
  if ( match.dst_ip.b != packet.dst_ip.b ) {
    return 0;
  }
  if ( match.dst_ip.c != packet.dst_ip.c ) {
    return 0;
  }
  if ( match.dst_ip.d != packet.dst_ip.d ) {
    return 0;
  }
  if ( match.dst_port != packet.dst_port ) {
    return 0;
  }
  return 1;
}
