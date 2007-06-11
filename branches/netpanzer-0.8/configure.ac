#----------------------------------------------------------------------------
# Autoconf input script. Start the ./autgen.sh script for producing a
# configure script.
#----------------------------------------------------------------------------
AC_PREREQ([2.54])

AC_INIT([netpanzer], [0.8.2-rc5], [netpanzer-devel@lists.berlios.org])
AC_CONFIG_SRCDIR([mk/jam/build.jam])
AC_CONFIG_AUX_DIR([mk/autoconf])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

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

AC_C_BIGENDIAN()

# stupid autoconf is adding default -g -O2 flags when we don't want this :-/
test ".$CXXFLAGS" = "." && CXXFLAGS=" "

#----------------------------------------------------------------------------
# Check for build variant (debug, profile, optimize)
#----------------------------------------------------------------------------
VARIANT=optimize
AC_ARG_ENABLE([optimize], [AC_HELP_STRING([--enable-optimize],
    [build with optimizations enabled (default YES)])],
    [test "$enableval" = "yes" && VARIANT=optimize])
                                                                                
AC_ARG_ENABLE([debug], [AC_HELP_STRING([--enable-debug],
    [build with debugging information (default NO)])],
    [test "$enableval" = "yes" && VARIANT=debug])
                                                                                
AC_ARG_ENABLE([profile], [AC_HELP_STRING([--enable-profile],
    [build with profiling information (default NO)])],
    [test "$enableval" = "yes" && VARIANT=profile])
AC_SUBST([VARIANT])

#----------------------------------------------------------------------------
# find applications
#----------------------------------------------------------------------------
AC_PROG_CXX
AC_PROG_INSTALL

# check if our c++ compiler is gcc3.2 or newer
AC_MSG_CHECKING([whether g++ is new enough])
AC_LANG_SAVE
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE([], [
#ifndef __GNUC__
#error no gcc compiler found (__GNUC__ not defined)
#endif
#if __GNUC__ < 3 || (__GNUC__ == 3 && __GNUC_MINOR__ < 2)
#error gcc compiler too old. Need at least version 3.2.0
#endif
], [AC_MSG_RESULT([yes])], [AC_MSG_RESULT([no])
   AC_MSG_ERROR([
Error: You gcc version is too old. netpanzer needs at least gcc 3.2.0.
])])
AC_LANG_RESTORE
AC_CHECK_PROGS(DOXYGEN, [doxygen])

#----------------------------------------------------------------------------
# Check for operating system
#----------------------------------------------------------------------------
AC_MSG_CHECKING([for target host])
case $host_os in
    mingw*|cygwin*)
        AC_MSG_RESULT([WIN32])
        AC_DEFINE([USE_WINSOCK], [], [Define if winsock should be used])
        LIBS="$LIBS -lwsock32"
        ;;
    *)
        AC_MSG_RESULT([assume posix])
        ;;
esac

#----------------------------------------------------------------------------
# find libraries
#----------------------------------------------------------------------------
AX_TYPE_SOCKLEN_T

AM_PATH_SDL([1.2.5], , AC_MSG_ERROR([Please install libsdl >= 1.2.]))

NP_FINDLIB([SDLMIXER], [SDL_mixer], [SDL_mixer >= 1.2],
	NP_LANG_PROGRAM([#include <SDL_mixer.h>
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

dnl SDL_image disabled for now, when use in future reenable it.
dnl NP_FINDLIB([SDLIMAGE], [SDL_image], [SDL_image >= 1.2],
dnl	NP_LANG_PROGRAM([#include <SDL_image.h>], [IMG_Load("");]),
dnl	[], [-lSDL_image],
dnl	[],
dnl	[AC_MSG_ERROR([Please install SDLImage >= 1.2.1])],
dnl	[$SDL_CFLAGS], [$SDL_LIBS])

dnl disabled for now, reenable if you work further on the UI
dnl NP_FINDLIB([SDLTTF], [SDL_ttf], [SDL_ttf >= 2.0.0],
dnl 	NP_LANG_PROGRAM([#include <SDL_ttf.h>], [if(!TTF_WasInit()) TTF_Init();]),
dnl	[], [-lSDL_ttf],
dnl	[],
dnl	[AC_MSG_ERROR([Please install SDL_ttf >= 2.0.0])],
dnl	[$SDL_CFLAGS], [$SDL_LIBS])

NP_FINDLIB([PHYSFS], [physfs], [physfs >= 0.1.9],
	NP_LANG_PROGRAM([
#include <stdio.h>
#include <physfs.h>
#if PHYSFS_VER_MAJOR < 1
#if PHYSFS_VER_MINOR < 1
# error PHYSFS is too old
#else
# if PHYSFS_VER_MINOR == 1
#  if PHYSFS_VER_PATCH < 9
#   error PHYSFS is too old
#  endif
# endif
#endif
#endif]),
	[], [-lphysfs],
	[],
	[AC_MSG_ERROR([Please install physfs >= 0.1.9])])

AC_INIT_JAM
AC_CONFIG_FILES([Jamconfig])
AC_OUTPUT

AC_MSG_NOTICE([

Notice: This project uses jam (and not make) as build tool.
])
