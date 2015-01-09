dnl
dnl $Id$
dnl

PHP_ARG_ENABLE(searcher, whether to enable SEARCHER support,
[  --disable-searcher          Disable SEARCHER support], yes)

if test "$PHP_SEARCHER" != "no"; then
  AC_DEFINE([HAVE_JSON],1 ,[whether to enable SEARCHER support])
  AC_HEADER_STDC

  PHP_NEW_EXTENSION(searcher, searcher.c searcher/searcher.c, $ext_shared)
  PHP_INSTALL_HEADERS([ext/searcher], [php_searcher.h])
  PHP_SUBST(SEARCHER_SHARED_LIBADD)
fi
