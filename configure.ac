AC_INIT([Micetrap], [0.0.1], [josep.m.bach AT gmail.com])

# unique source file --- primitive safety check 
AC_CONFIG_SRCDIR([src/main.c])

# fairly severe build strictness
# change foreign to gnu or gnits to comply with gnu standards
AM_INIT_AUTOMAKE([-Wall -Werror gnu 1.9.6])

PKG_CHECK_MODULES(CHECK,[check >= 0.9.8],:,[
ifdef([AM_PATH_CHECK],
[AM_PATH_CHECK],
[AC_MSG_RESULT([no, testing is disabled])])
])

AM_CONDITIONAL([HAVE_CHECK],[test "x$CHECK_CFLAGS" != "x"])


# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for library functions.
AC_FUNC_MALLOC

# Output files
#AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tests/Makefile])

AC_OUTPUT
