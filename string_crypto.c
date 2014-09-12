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

#include <openssl/des.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_string_crypto.h"


#define BLOCK_SIZE 8
#define ALIGN(n) ((n + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE)

static int le_string_crypto;
static unsigned char key1[8] = {0x2C, 0x7A, 0x0E, 0x98, 0xF1, 0xE0, 0x76, 0x49};
static unsigned char key2[8] = {0x73, 0x15, 0xCD, 0x25, 0xE0, 0xB5, 0x43, 0xCB};
static unsigned char key3[8] = {0x0E, 0x80, 0x76, 0x01, 0x7F, 0x23, 0x8A, 0x46};


PHP_MINIT_FUNCTION(string_crypto)
{
	
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(string_crypto)
{
	
	return SUCCESS;
}

PHP_RINIT_FUNCTION(string_crypto)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(string_crypto)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(string_crypto)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "string_crypto support", "enabled");
	php_info_print_table_end();
}

static int do_encrypt(const unsigned char *in, unsigned char *out, int len,
		int action)
{
	DES_key_schedule ks1, ks2, ks3;
	DES_cblock cblock = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	int i;

	DES_set_odd_parity(&cblock);

	if (DES_set_key_checked(&key1, &ks1) || DES_set_key_checked(&key2, &ks2)
			|| DES_set_key_checked(&key3, &ks3)) {
		return 1;
	}

	for (i = 0; i < len / BLOCK_SIZE; i++) {
		DES_ede3_cbc_encrypt(in + i * BLOCK_SIZE, out + i * BLOCK_SIZE,
				BLOCK_SIZE, &ks1, &ks2, &ks3, &cblock, action);
	}

	return 0;
}


PHP_FUNCTION(string_cookie_encrypt)
{
	char *str = NULL;
	int argc = ZEND_NUM_ARGS();
	int str_len, dst_len, src_len;
	char *dst = NULL;
	char *src = NULL;


	if (zend_parse_parameters(argc TSRMLS_CC, "s", &str, &str_len) == FAILURE)
		RETURN_FALSE;

	if(str_len <= 0)
		RETURN_FALSE;

	//php_printf(">>>>>>>encrypt string :%s len:%d\n", str, str_len);

	src_len = ALIGN(str_len);
	src = emalloc(src_len * sizeof(char));
	memset(src , 0, src_len);
	memcpy(src, str, str_len);

	dst_len = src_len;
	dst = emalloc((dst_len + 1) * sizeof(char));
	memset(dst, 0, dst_len + 1);

	if (do_encrypt((const unsigned char *) src, (unsigned char *) dst, dst_len,
			DES_ENCRYPT))
		RETURN_FALSE;

	dst[dst_len] = '\0';

	efree(src);

	RETURN_STRINGL(dst, dst_len, 0);
}


PHP_FUNCTION(string_cookie_decrypt)
{
	char *str = NULL;
	int argc = ZEND_NUM_ARGS();
	int str_len, dst_len;
	char *dst = NULL;


	if (zend_parse_parameters(argc TSRMLS_CC, "s", &str, &str_len) == FAILURE)
		RETURN_FALSE;

	if(str_len <= 0)
		RETURN_FALSE;

//	php_printf(">>>>>>>decrypt string :%s len:%d\n", s, str_len);

	dst_len = ALIGN(str_len);
	dst = emalloc((dst_len + 1) * sizeof(char));
	memset(dst, 0, dst_len + 1);

	if (do_encrypt((const unsigned char *) str, (unsigned char *) dst, str_len,
			DES_DECRYPT))
		RETURN_FALSE;

	dst[dst_len] = '\0';

	RETURN_STRINGL(dst, strlen(dst), 0);
}

const zend_function_entry string_crypto_functions[] = {
	PHP_FE(string_cookie_encrypt,	NULL)
	PHP_FE(string_cookie_decrypt,	NULL)
	PHP_FE_END
};


zend_module_entry string_crypto_module_entry = {
	STANDARD_MODULE_HEADER,
	"string_crypto",
	string_crypto_functions,
	PHP_MINIT(string_crypto),
	PHP_MSHUTDOWN(string_crypto),
	PHP_RINIT(string_crypto),
	PHP_RSHUTDOWN(string_crypto),
	PHP_MINFO(string_crypto),
	PHP_STRING_CRYPTO_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_STRING_CRYPTO
ZEND_GET_MODULE(string_crypto)
#endif


