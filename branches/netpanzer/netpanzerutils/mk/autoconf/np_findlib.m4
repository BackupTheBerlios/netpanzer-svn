#  NP_FINDLIB(NAME, STRING, TESTAPP, CFLAGS, LIBS, ACTION_IF_FOUND,
#             ACTION_IF_NOT_FOUND, EXTRACFLAGS, EXTRALIBS)
AC_DEFUN([NP_FINDLIB], [
    save_CFLAGS="$CFLAGS"
    save_CPPFLAGS="$CPPFLAGS"
    save_LIBS="$LIBS"

    CFLAGS="$CFLAGS $4 $8"
    CPPFLAGS="$CPPFLAGS $4 $8"
    LIBS="$LIBS $5 $9"

    AC_MSG_CHECKING([for $2])

    AC_LINK_IFELSE([$3], [buildok=yes], [buildok=no])

    LIBS=$save_LIBS
    CPPFLAGS=$save_CPPFLAGS
    CFLAGS=$save_CFLAGS

    AS_IF([test $buildok = yes],
	[AC_MSG_RESULT([found])
	 $1_AVAILABLE=yes
	 $1_CFLAGS="$4"
	 $1_LIBS="$5"],
	[AC_MSG_RESULT([not found])
	 $1_AVAILABLE=no
	 $1_CFLAGS=""
	 $1_LIBS=""])
    
    AC_SUBST([$1_AVAILABLE])
    AC_SUBST([$1_CFLAGS])
    AC_SUBST([$1_LIBS])

    AS_IF([test $buildok = yes],
 	[ifelse([$6], , :, [$6])],
 	[ifelse([$7], , :, [$7])])
])
