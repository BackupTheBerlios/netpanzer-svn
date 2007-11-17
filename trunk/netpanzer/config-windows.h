/* Disable gcc specific __attribute__ keyword on other compilers */
#ifndef __GNUC__
#define __attribute__(x)
#endif

#define PACKAGE_NAME "netpanzer"
#define PACKAGE_STRING "netpanzer svnwin"
#define PACKAGE_VERSION "svnwin"
#define USE_WINSOCK
#undef WORDS_BIGENDIAN
#define socklen_t int
