#-----------------------------------------------------------------------------
# installdirs.m4 (c) Matze Braun <matze@braunis.de>
# Macros for outputing the installation paths which autoconf gathers into the
# Jamconfig file.
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# CS_OUTPUT_INSTALLDIRS
#   Transforms the installation dirs which are gathered by autoconf and sets
#   properties in the Jamconfig file for them. We deal with stuff like
#   variable references inside the paths (often the paths contain ${prefix})
#   and with correct quoting here.
#   The script will set the INSTALLDIR.PREFIX, INSTALLDIR.EXEC_PREFIX,
#   INSTALLDIR.APPLICATION, INSTALLDIR.SBIN, INSTALLDIR.LIBEXEC,
#   INSTALLDIR.DATA, INSTALLDIR.MAP, INSTALLDIR.CONFIG, INSTALLDIR.SHAREDSTATE
#   INSTALLDIR.LOCALSTATE, INSTALLDIR.PLUGIN, INSTALLDIR.DOC
#   INSTALLDIR.LIBRARY, INSTALLDIR.INCLUDE, INSTALLDIR.OLDINCLUDE,
#   INSTALLDIR.INFO, INSTALLDIR.MAN
#-----------------------------------------------------------------------------
AC_DEFUN([CS_OUTPUT_INSTALLDIRS],[
# Handle the case when no prefix is given. And the special case when a path
# contains more than 2 slashes, these paths seem to be correct but jam fails
# on them.
AS_IF([test $prefix = NONE], [prefix="$ac_default_prefix"],
    [prefix=`echo "$prefix" | sed -e 's:///*:/:g'`])
AS_IF([test $exec_prefix = NONE],
    [exec_prefix="AS_ESCAPE([$(prefix)])"],
    [exec_prefix=`echo "$exec_prefix" | sed -e 's:///*:/:g'`])

prefix=CS_PREPARE_INSTALLPATH([$prefix])
exec_prefix=CS_PREPARE_INSTALLPATH([$exec_prefix])
bindir=CS_PREPARE_INSTALLPATH([$bindir])
sbindir=CS_PREPARE_INSTALLPATH([$sbindir])
libexecdir=CS_PREPARE_INSTALLPATH([$libexecdir])
datadir=CS_PREPARE_INSTALLPATH([$datadir])
sysconfdir=CS_PREPARE_INSTALLPATH([$sysconfdir])
sharedstatedir=CS_PREPARE_INSTALLPATH([$sharedstatedir])
localstatedir=CS_PREPARE_INSTALLPATH([$localstatedir])
libdir=CS_PREPARE_INSTALLPATH([$libdir])
includedir=CS_PREPARE_INSTALLPATH([$includedir])
oldincludedir=CS_PREPARE_INSTALLPATH([$oldincludedir])
infodir=CS_PREPARE_INSTALLPATH([$infodir])
mandir=CS_PREPARE_INSTALLPATH([$mandir])

#hack to get the order right :-/ (autoconf --trace reports wrong order...)
AC_SUBST(prefix)
AC_SUBST(exec_prefix)
AC_SUBST(bindir)
AC_SUBST(sbindir)
AC_SUBST(libexecdir)
AC_SUBST(datadir)
AC_SUBST(sysconfdir)
AC_SUBST(sharedstatedir)
AC_SUBST(localstatedir)
AC_SUBST(libdir)
AC_SUBST(includedir)
AC_SUBST(oldincludedir)
AC_SUBST(infodir)
AC_SUBST(mandir)
])

#-----------------------------------------------------------------------------
# CS_PREPARE_INSTALLPATH
#   Transform variables of the form ${bla} to $(bla) inside the string and
#   correctly quotes backslashes.
#   This is needed if you want to output some of the paths that autoconf
#   creates to the Jamconfig file.
#-----------------------------------------------------------------------------
AC_DEFUN([CS_PREPARE_INSTALLPATH],
dnl We need all the strange \\\\ quoting here, because the command will be
dnl inserted into a "" block and sed needs quoting as well
[`echo "$1" | sed -e 's/\${\([[a-zA-Z_][a-zA-Z_]]*\)}/$(\1)/g' -e 's/\\\\/\\\\\\\\/g'`])

