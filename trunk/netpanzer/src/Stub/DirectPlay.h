#ifndef __DIRECTPLAY_H__
#define __DIRECTPLAY_H__

// very dummy stub for directplay stuff

typedef int DPID;
void SetPacketFunction( void ( * packet_func)( void *message, unsigned long message_size, DPID toID, DPID fromID) );

#endif

