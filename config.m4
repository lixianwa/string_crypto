dnl $Id$
dnl config.m4 for extension string_crypto


PHP_ARG_ENABLE(string_crypto, whether to enable string_crypto support,
[  --enable-string_crypto           Enable string_crypto support])

if test "$PHP_string_CRYPTO" != "no"; then

  PHP_SUBST(STRING_CRYPTO_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(crypto, /usr/lib64, STRING_CRYPTO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(string_crypto, string_crypto.c, $ext_shared) 
fi
