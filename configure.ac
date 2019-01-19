#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE(CaiJiQi,1.0)
# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_PROG_RANLIB
# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h string.h sys/vfs.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT32_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([mkdir])

AC_CONFIG_FILES([Pipeline/Makefile ThreadMsg/Makefile Server/Makefile Thread/Makefile DataBase/Makefile LLog/Makefile
                 Makefile
                 Timer/Makefile])

AC_OUTPUT
