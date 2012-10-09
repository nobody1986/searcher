dnl $Id$
dnl config.m4 for extension searcher

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(searcher, for searcher support,
dnl Make sure that the comment is aligned:
dnl [  --with-searcher             Include searcher support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(searcher, whether to enable searcher support,
dnl Make sure that the comment is aligned:
 [  --enable-searcher           Enable searcher support])

if test "$PHP_SEARCHER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-searcher -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/searcher.h"  # you most likely want to change this
  dnl if test -r $PHP_SEARCHER/$SEARCH_FOR; then # path given as parameter
  dnl   SEARCHER_DIR=$PHP_SEARCHER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for searcher files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SEARCHER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SEARCHER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the searcher distribution])
  dnl fi

  dnl # --with-searcher -> add include path
  dnl PHP_ADD_INCLUDE($SEARCHER_DIR/include)

  dnl # --with-searcher -> check for lib and symbol presence
  dnl LIBNAME=searcher # you may want to change this
  dnl LIBSYMBOL=searcher # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SEARCHER_DIR/lib, SEARCHER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SEARCHERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong searcher lib version or lib not found])
  dnl ],[
  dnl   -L$SEARCHER_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SEARCHER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(searcher, searcher.c, $ext_shared)
fi
