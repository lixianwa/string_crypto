/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: lixian <lix0806 at gmail dot com>                                                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_STRING_CRYPTO_H
#define PHP_STRING_CRYPTO_H

extern zend_module_entry string_crypto_module_entry;
#define phpext_string_crypto_ptr &string_crypto_module_entry

#define PHP_STRING_CRYPTO_VERSION "0.1.0"

#ifdef PHP_WIN32
#	define PHP_STRING_CRYPTO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_STRING_CRYPTO_API __attribute__ ((visibility("default")))
#else
#	define PHP_STRING_CRYPTO_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


#ifdef ZTS
#define STRING_CRYPTO_G(v) TSRMG(string_crypto_globals_id, zend_string_crypto_globals *, v)
#else
#define STRING_CRYPTO_G(v) (string_crypto_globals.v)
#endif

PHP_FUNCTION(string_cookie_encrypt);
PHP_FUNCTION(string_cookie_decrypt);

#endif	/* PHP_STRING_CRYPTO_H */


