#----------------------------------------------------------------------------
# Autoconf input script. Start the ./autgen.sh script for producing a
# the configure script.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

AC_INIT([netpanzer], [0.1], [netpanzer-devel@nongnu.org])
AC_CONFIG_SRCDIR([src/Lib/Log.hpp])
AC_CONFIG_AUX_DIR([mk/autoconf])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE

#----------------------------------------------------------------------------
# Configuration header
#----------------------------------------------------------------------------
AC_CONFIG_HEADERS(config.h)
AH_TOP([
/* Disable gcc specific __attribute__ keyword on other compilers */
#ifndef __GNUC__
#define __attribute__(x)
#endif
])

#----------------------------------------------------------------------------
# Check for operating system
#----------------------------------------------------------------------------
AC_MSG_CHECKING([for target host])
case $host_os in 
    mingw*|cygwin*)
	AC_MSG_RESULT(WIN32)
	AC_DEFINE([WIN32], [], [define when compiling for a win32 box])
	;;
    *)
	AC_MSG_RESULT([assuming unix])
	AC_DEFINE([UNIX], [], [define when compiling for a unix/posix system])
	;;
esac

#----------------------------------------------------------------------------
# find applications
#----------------------------------------------------------------------------
AC_PROG_CXX
AC_PROG_INSTALL
AC_CHECK_PROG(MAKEDEPEND, [makedepend])

#----------------------------------------------------------------------------
# find libraries
#----------------------------------------------------------------------------
PKG_CHECK_MODULES(XML, [libxml-2.0])
    
AM_PATH_SDL([1.2.5], , AC_MSG_ERROR([Please install libsdl >= 1.2.]))

NP_FINDLIB([SDLNET], [SDL_net >= 1.2],
	AC_LANG_PROGRAM([#include <SDL_net.h>], [SDLNet_Init();]),
	[], [-lSDL_net],
	[],
	[AC_MSG_ERROR([Please install SDLNet >= 1.2.4])],
	[$SDL_CFLAGS], [$SDL_LIBS])

NP_FINDLIB([SDLMIXER], [SDL_mixer >= 1.2],
	AC_LANG_PROGRAM([#include <SDL_mixer.h>
#if MIX_MAJOR_VERSION < 1
# error SDLMix too old
#else
# if MIX_MAJOR_VERSION == 1
#  if MIX_MINOR_VERSION < 2
#   error SDLMix too old
#  endif
# endif
#endif], [Mix_OpenAudio(0, 0, 0, 0);]),
	[], [-lSDL_mixer],
	[],
	[AC_MSG_ERROR([Please install SDLMixer >=1.2.2])],
	[$SDL_CFLAGS], [$SDL_LIBS])

NP_FINDLIB([SDLIMAGE], [SDL_image >= 1.2],
	AC_LANG_PROGRAM([#include <SDL_image.h>], [IMG_Load("");]),
	[], [-lSDL_image],
	[],
	[AC_MSG_ERROR([Please install SDLImage >= 1.2.1])],
	[$SDL_CFLAGS], [$SDL_LIBS])

NP_FINDLIB([PHYSFS], [physfs >= 1.9],
	AC_LANG_PROGRAM([#include <physfs.h>
#if PHYSFS_VER_MINOR < 1
# error PHYSFS is too old
#else
# if PHYSFS_VER_MINOR == 1
#  if PHYSFS_VER_PATCH < 9
#   error PHYSFS is too old
#  endif
# endif
#endif]),
	[], [-lphysfs],
	[],
	[AC_MSG_ERROR([Please install physfs >= 0.1.9])])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

