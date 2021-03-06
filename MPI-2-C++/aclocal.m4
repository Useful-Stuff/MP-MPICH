dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl -*- shell-script -*-

# This file is part of the University of Notre Dame implementation of
# the MPI 2 C++ bindings package.  See the LICENSE file in the top
# level directory for license and copyright information.

define([LSC_CXX_TEMPLATE_REPOSITORY],[
#
# See if the compiler makes template repository directories
# Warning: this is a really screwy example! -JMS
#
# Sets TEMPLATE_REP to the directory name, or blank
# Does AC_SUBST on TEMPLATE_REP
#

AC_MSG_CHECKING([for template repository directory])
mkdir conf_tmp_$$
cd conf_tmp_$$
cat > conftest.h <<EOF
template <class T>
class foo {
public:
  foo(T yow) : data(yow) { yow.member(3); };
  void member(int i);
private:
  T data;
};

class bar {
public:
  bar(int i) { data = i; };
  void member(int j) { data = data * j; };

private:
  int data;
};
EOF

cat > conftest2.C <<EOF
#include "conftest.h"

void
some_other_function(void)
{
  foo<bar> var1(6);
  foo< foo<bar> > var2(var1);
}
EOF

cat > conftest1.C <<EOF
#include "conftest.h"

void some_other_function(void);

template <class T>
void
foo<T>::member(int i)
{
  i += 2;
}

int
main(int argc, char *argv[])
{
  foo<bar> var1(6);
  foo< foo<bar> > var2(var1);

  some_other_function();
  return 0;
}
EOF

echo configure:__oline__: $CXX $CXXFLAGS -c conftest1.C >&5
$CXX $CXXFLAGS -c conftest1.C >&5 2>&5
if test ! -f conftest1.o ; then
    AC_MSG_RESULT([templates not supported?])
    echo configure:__oline__: here is the program that failed: >&5
    cat conftest1.C >&5
    echo configure:__oline__: here is conftest.h: >&5
    cat conftest.h >&5
else
    echo configure:__oline__: $CXX $CXXFLAGS -c conftest2.C >&5
    $CXX $CXXFLAGS -c conftest2.C >&5 2>&5
    if test ! -f conftest2.o ; then
	AC_MSG_RESULT([unknown error])
	echo configure:__oline__: here is the program that failed: >&5
	cat conftest2.C >&5
	echo configure:__oline__: here is conftest.h: >&5
	cat conftest.h >&5
    else
	rm -rf conftest*

	for file in `ls`
	do
	    if test "$file" != "." -a "$file" != ".."; then
		# Is it a directory?
		if test -d "$file"; then
		    TEMPLATE_DIR="$file $TEMPLATE_DIR"
		    
		# Or is it a file?
		else
		    name="`echo $file | cut -d. -f1`"
		    
		    temp_mask=
		    if test "$name" = "main" -o "$name" = "other"; then
			temp_mask="`echo $file | cut -d. -f2`"
			if test "$TEMPLATE_FILEMASK" = ""; then
			TEMPLATE_FILEMASK="$temp_mask";
			elif test "`echo $TEMPLATE_FILEMASK | grep $temp_mask`" = ""; then
			TEMPLATE_FILEMASK="$TEMPLATE_FILEMASK $temp_mask"
			fi
		    fi
		fi
	    fi
	done
	if test "$TEMPLATE_FILEMASK" != ""; then
	    temp_mask=
	    for mask in $TEMPLATE_FILEMASK
	    do
		temp_mask="*.$mask $temp_mask"
	    done
	    TEMPLATE_FILEMASK=$temp_mask
	fi
    fi
fi
TEMPLATE_REP="$TEMPLATE_DIR $TEMPLATE_FILEMASK"

if test "$TEMPLATE_REP" != "" -a "$TEMPLATE_REP" != " "; then
    AC_MSG_RESULT([$TEMPLATE_REP])
else
    AC_MSG_RESULT([not used])
fi
cd ..
rm -rf conf_tmp_$$
AC_SUBST(TEMPLATE_REP)dnl
])dnl

define([LSC_CHECK_CXX_EXCEPTION_FLAGS],[
#
# Get the exception handling flags for the C++ compiler
#
# Sets LSC_EXCEPTION_CXXFLAGS and LSC_EXCEPTION_LDFLAGS as appropriate
# Calls AC_SUBST on both
#

AC_ARG_WITH(exflags,   [--with-exflags          Specify flags necessary to enable exceptions], FORCE_EXFLAGS=$withval)

LOCAL_CXXFLAGS_SAVE=$CXXFLAGS
AC_MSG_CHECKING([for compiler exception flags])
if test "$FORCE_EXFLAGS" != ""; then
    EXFLAGS=$FORCE_EXFLAGS
elif test "$GXX" = "yes"; then
    CXXFLAGS="$CXXFLAGS -fexceptions"

    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS
    AC_TRY_COMPILE(, [try { int i = 0; } catch(...) { int j = 2; }], HAPPY=1, HAPPY=0)

    if test "$HAPPY" = "1"; then
	EXFLAGS="-fexceptions";
    else
	CXXFLAGS="$LOCAL_CXXFLAGS_SAVE -fhandle-exceptions"
	AC_TRY_COMPILE(, [try { int i = 0; } catch(...) { int j = 2; }], HAPPY=1, HAPPY=0)
	if test "$HAPPY" = "1"; then
	    EXFLAGS="-fhandle-exceptions";
	fi
    fi
    AC_LANG_RESTORE
elif test "`basename $CXX`" = "KCC"; then
    EXFLAGS="-x"
elif test "$LAMHCP" = "1"; then
    if test "`basename $LAMHCP`" = "KCC"; then
	EXFLAGS="-x"
    fi
fi
CXXFLAGS="$LOCAL_CXXFLAGS_SAVE"

LSC_EXCEPTION_CXXFLAGS=$EXFLAGS
LSC_EXCEPTION_LDFLAGS="$EXFLAGS"
if test "$EXFLAGS" = ""; then
    AC_MSG_RESULT([none necessary])
else
    AC_MSG_RESULT([$EXFLAGS])
fi
AC_SUBST(LSC_EXCEPTION_CXXFLAGS)
AC_SUBST(LSC_EXCEPTION_LDFLAGS)])dnl


define([LSC_CHECK_MPI_H],[
if test "$TEMPLATE_REP" = ""; then
    LSC_CXX_TEMPLATE_REPOSITORY
fi
AC_MSG_CHECKING([for mpi.h])
touch conftest.o
rm -f conftest.o
cat > conftest.C <<EOF
#include <mpi.h>
int 
main(int argc, char* argv[])
{ return 0; }
EOF
echo configure:__oline__: $CXX $CXXFLAGS -c conftest.C >&5
$CXX $CXXFLAGS -c conftest.C >&2 2>&5
if test -f conftest.o ; then
    HAVE_MPI_H=yes
    AC_MSG_RESULT([yes])
else
    HAVE_MPI_H=no
    echo configure:__oline__: here is the program that failed: >&5
    cat conftest.C >&5
    AC_MSG_RESULT([no])
fi
rm -rf conftest* $TEMPLATE_REP
if test "$HAVE_MPI_H" = "no"; then
    AC_MSG_ERROR([cannot continue -- cannot find <mpi.h>])
fi])dnl


define([LSC_CHECK_CXX_BOOL],[
#
# Check to see if the C++ compiler has the bool type.  <sigh>
#
# Defines LSC_HAVE_BOOL to be 1 or 0
# Sets LSC_HAVE_BOOL to be 1 or 0
#

if test "$?CXXFLAGS" = 0; then
    CXXFLAGS=""
fi
AC_MSG_CHECKING([for type bool])
rm -f conftest.cc conftest.o
cat > conftest.cc <<EOF
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
  bool foo = (bool) 0;
  printf("so foo is used and the compiler wont complain: %d", (int) foo);
  return 0;
}
EOF
echo configure:__oline__: $CXX $CXXFLAGS conftest.cc -o conftest >&5 
$CXX $CXXFLAGS conftest.cc -o conftest >&5 2>&5
if test -f conftest; then
    LSC_HAVE_BOOL=1
    AC_DEFINE(LSC_HAVE_BOOL, 1)
    AC_MSG_RESULT([yes])
else
    echo configure:__oline__: here is the program that failed: >&5
    cat conftest.cc >&5
    LSC_HAVE_BOOL=0
    AC_DEFINE(LSC_HAVE_BOOL, 0)
    AC_MSG_RESULT([no])
fi
rm -rf conftest*])dnl


define([LSC_MPI_ERR_PENDING],[
#
# At least one MPI implementation has(had) this problem
# (no names mentioned...)
#
# Sets LSC_HAVE_PENDING to 1 if MPI_PENDING, 0 if MPI_ERR_PENDING
#

if test "$TEMPLATE_REP" = ""; then
    LSC_CXX_TEMPLATE_REPOSITORY
fi
AC_MSG_CHECKING([whether to use MPI(_ERR)_PENDING])
AC_TRY_COMPILE([#include <mpi.h>]
, int yow=MPI_PENDING;, use_pending=yes, use_pending=no)
if test "$use_pending" = "yes"; then
    LSC_HAVE_PENDING=1
    AC_MSG_RESULT([MPI_PENDING])
else
    LSC_HAVE_PENDING=0
    AC_MSG_RESULT([MPI_ERR_PENDING])
fi
rm -rf $TEMPLATE_REP])dnl


define([LSC_CHECK_LMPI],[
#
# Try to link a simple program, see if we can find libmpi.*
# 
# Aborts if we cannot -lmpi
#

if test "$TEMPLATE_REP" = ""; then
    LSC_CXX_TEMPLATE_REPOSITORY
fi
if test "`echo $MPI_LIBS`" != ""; then
    AC_MSG_CHECKING([for MPI_Initialized in "$MPI_LIBS"])
else
    AC_MSG_CHECKING([for MPI_Initialized])
fi
AC_TRY_LINK(#include <mpi.h>
, int i; MPI_Initialized(&i);, libmpi_found=yes, libmpi_found=no)
if test "$libmpi_found" = "yes"; then
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([cannot continue -- cannot link to "$MPI_LIBS"])
fi
rm -rf $TEMPLATE_REP])dnl


define(MPI2CPP_GET_SIZEOF,[
# Don't ask.  It's an AIX thing.  <sob>
if test "$?host" = "0"; then
    AC_CANONICAL_HOST
fi
# Determine datatype size. 
# First arg is type, 2nd (optional) arg is config var to define.
AC_MSG_CHECKING(size of of $1)
cat > conftest.C <<EOF
#include <stdio.h>
#ifdef _ALL_SOURCE
#undef _ALL_SOURCE
#endif
#include "confdefs.h"

int main(int argc, char* argv[])
{
    FILE *f=fopen("conftestval", "w");
    if (!f) return 1;
    fprintf(f, "%d\n", sizeof($1));
    return 0;
}
EOF
# AIX really, really sucks.  If you use the mpCC compiler (wrapper to
# the underlying C++ compiler), it expects that your program is
# parallel, so trying to run it with "./foo" won't work.  Hence, we
# have to switch compilers here.
REALCXX="$CXX"
if test "`basename $CXX`" = "mpCC"; then
    case "$host" in
    *aix*)
	REALCXX="xlC"
	;;
    esac
fi
$REALCXX $CXXFLAGS -o conftest conftest.C 1>&5 2>&1
if test -s conftest && (./conftest; exit) 2>/dev/null; then
    mpi2cpp_ac_size=`cat conftestval`
else
    mpi2cpp_ac_size=0
fi
AC_MSG_RESULT($mpi2cpp_ac_size)
if test -n "$2"; then
    AC_DEFINE_UNQUOTED($2,$mpi2cpp_ac_size)
fi
/bin/rm -f conftest*])dnl


# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AC_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])

# Define a conditional.

AC_DEFUN(AM_CONDITIONAL,
[AC_SUBST($1_TRUE)
AC_SUBST($1_FALSE)
if $2; then
  $1_TRUE=
  $1_FALSE='#'
else
  $1_TRUE='#'
  $1_FALSE=
fi])

