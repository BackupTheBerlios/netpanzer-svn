AC_PREREQ([2.54])

AC_INIT([masterserver], 0.1.3)
AC_CONFIG_SRCDIR([src/MasterServer.hpp])
AM_INIT_AUTOMAKE

AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL

AC_CHECK_FUNCS([gethostbyname inet_ntoa memset socket strerror])
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_VOLATILE
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile src/Makefile exampleclient/Makefile])
AC_OUTPUT

