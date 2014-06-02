dnl $Id$
dnl config.m4 for extension zhtmltopdf

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(wkhtmltox-dir, for zhtmltopdf support,
Make sure that the comment is aligned:
[  --with-wkhtmltox-dir=[DIR]             Include zhtmltopdf support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(zhtmltopdf, whether to enable zhtmltopdf support,
dnl Make sure that the comment is aligned:
dnl [  --enable-zhtmltopdf           enable  libwkhtmltox support])

if test "$PHP_WKHTMLTOX_DIR" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-zhtmltopdf -> check with-path
  SEARCH_PATH="$PHP_WKHTMLTOX_DIR /usr/local /usr"     # you might want to change this
  SEARCH_FOR="include/wkhtmltox/pdf.h"  # you most likely want to change this
  dnl if test -r $PHP_ZHTMLTOPDF/$SEARCH_FOR; then # path given as parameter
  dnl   ZHTMLTOPDF_DIR=$PHP_ZHTMLTOPDF
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for zhtmltopdf files in default path])
  for i in $SEARCH_PATH ; do
    if test -f $i/$SEARCH_FOR; then
      ZHTMLTOPDF_DIR=$i
      AC_MSG_RESULT(found in $i)
    fi
  done
  dnl fi
  dnl
  if test -z "$ZHTMLTOPDF_DIR" -o -z "$ZHTMLTOPDF_DIR/include/wkhtmltox"; then
    AC_MSG_RESULT([$ZHTMLTOPDF_DIR not found])
    AC_MSG_ERROR([$SEARCH_PATH || $ZHTMLTOPDF_DIR ||  Please reinstall the zhtmltopdf distribution])
  fi

  dnl # --with-zhtmltopdf -> add include path
  PHP_ADD_INCLUDE($ZHTMLTOPDF_DIR/include/wkhtmltox)

  dnl # --with-zhtmltopdf -> check for lib and symbol presence
  dnl LIBNAME=zhtmltopdf # you may want to change this
  dnl LIBSYMBOL=zhtmltopdf # you most likely want to change this 

  PHP_CHECK_LIBRARY(wkhtmltox,wkhtmltopdf_init,
  [
    PHP_ADD_LIBRARY_WITH_PATH(wkhtmltox, $ZHTMLTOPDF_DIR/lib, ZHTMLTOPDF_SHARED_LIBADD)
    AC_DEFINE(HAVE_ZHTMLTOPDFLIB,1,[libwkhtmltox yes ])
  ],[
    AC_MSG_ERROR([$ZHTMLTOPDF_DIR/include no found])
  ],[
    -L$ZHTMLTOPDF_DIR/lib -lm
  ])
  
  PHP_SUBST(ZHTMLTOPDF_SHARED_LIBADD)

  PHP_NEW_EXTENSION(zhtmltopdf, zhtmltopdf.c, $ext_shared)
fi
