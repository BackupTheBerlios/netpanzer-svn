#ifndef __DIRECTPLAY_H__
#define __DIRECTPLAY_H__

#include <stdio.h>

// very dummy stub for directplay stuff

typedef int DPID;
static inline void SetPacketFunction( void ( * packet_func)( void *message, unsigned long message_size, DPID toID, DPID fromID) )
{ 
	printf ("SetPacketFunction NOT IMPLEMENTED!\n");
}

#endif

