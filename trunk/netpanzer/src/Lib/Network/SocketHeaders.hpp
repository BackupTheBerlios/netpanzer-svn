#ifndef __SOCKETHEADERS_HPP__
#define __SOCKETHEADERS_HPP__

#ifdef WINSOCK
#include <winsock2.h>
#else
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif

#endif

