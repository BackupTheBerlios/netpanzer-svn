#! /bin/sh

# Correct working directory?
if test ! -f configure.ac ; then
  echo "*** Please invoke this script from directory containing configure.ac."
  exit 1
fi

MACRODIR=mk/autoconf

aclocal -I $MACRODIR
autoheader
automake -a
autoconf
