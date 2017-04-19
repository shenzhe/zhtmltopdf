/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <string.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_zhtmltopdf.h"
#include "wkhtmltox/pdf.h"
#include "wkhtmltox/image.h"


///* If you declare any globals in php_zhtmltopdf.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(zhtmltopdf)
//*/

/* True global resources - no need for thread safety here */
//static int le_zhtmltopdf;


/* {{{ zhtmltopdf_functions[]
 *
 * Every user visible function must have an entry in zhtmltopdf_functions[].
 */
const zend_function_entry zhtmltopdf_functions[] = {
	PHP_FE(zhtml2pdf,	NULL)		/* For testing, remove later. */
	PHP_FE(zhtml2img,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in zhtmltopdf_functions[] */
};
/* }}} */

/* {{{ zhtmltopdf_module_entry
 */
zend_module_entry zhtmltopdf_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"zhtmltopdf",
	zhtmltopdf_functions,
	PHP_MINIT(zhtmltopdf),
	PHP_MSHUTDOWN(zhtmltopdf),
	PHP_RINIT(zhtmltopdf),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(zhtmltopdf),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(zhtmltopdf),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_ZHTMLTOPDF_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ZHTMLTOPDF
ZEND_GET_MODULE(zhtmltopdf)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("zhtmltopdf.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_zhtmltopdf_globals, zhtmltopdf_globals)
    STD_PHP_INI_ENTRY("zhtmltopdf.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_zhtmltopdf_globals, zhtmltopdf_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_zhtmltopdf_init_globals
 */
/* Uncomment this function if you have INI entries*/
static void php_zhtmltopdf_init_globals(zend_zhtmltopdf_globals *zhtmltopdf_globals)
{
    zhtmltopdf_globals->zhtml2pdf_initialized = 0;
    zhtmltopdf_globals->zhtml2img_initialized = 0;
}

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zhtmltopdf)
{
    ZEND_INIT_MODULE_GLOBALS(zhtmltopdf, php_zhtmltopdf_init_globals, NULL);
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(zhtmltopdf)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
    if(ZHTMLTOPDF_G(zhtml2pdf_initialized)) {
        wkhtmltopdf_deinit();
    }
    if(ZHTMLTOPDF_G(zhtml2img_initialized)) {
        wkhtmltoimage_deinit();
    }
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(zhtmltopdf)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(zhtmltopdf)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(zhtmltopdf)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "zhtmltopdf support", "enabled");
	php_info_print_table_header(2, "version", "0.2");
	php_info_print_table_header(2, "Author", "shenzhe163@gmail.com");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_zhtmltopdf_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(zhtml2pdf)
{
	char *out = NULL;
	char *url = NULL;
	char *cookie = NULL;
	int out_len, url_len, cookie_len;
  long len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|ss", &url, &url_len, &out, &out_len, &cookie, &cookie_len) == FAILURE) {
		return;
	}

    if(!ZHTMLTOPDF_G(zhtml2pdf_initialized)) {
        ZHTMLTOPDF_G(zhtml2pdf_initialized) = wkhtmltopdf_init(0);
    }
    wkhtmltopdf_global_settings * gs;
    wkhtmltopdf_object_settings * os;
    wkhtmltopdf_converter * c;


    gs = wkhtmltopdf_create_global_settings();

    if(out) {
      wkhtmltopdf_set_global_setting(gs, "out", out);
    }
    
    if(cookie){
        wkhtmltopdf_set_global_setting(gs, "load.cookieJar", cookie);
    }

    os = wkhtmltopdf_create_object_settings();
    wkhtmltopdf_set_object_setting(os, "page", url);
    c = wkhtmltopdf_create_converter(gs);
    wkhtmltopdf_add_object(c, os, NULL);
    if(!wkhtmltopdf_convert(c)) {
      RETURN_FALSE;
      return;
    }
    
    if(out) {
      wkhtmltopdf_destroy_converter(c);
      RETURN_TRUE;
    } else {
      const unsigned char * data;
      len = wkhtmltopdf_get_output(c, &data);
      wkhtmltopdf_destroy_converter(c);
      if(len > 0) {
        //RETURN_LONG(len);
        RETURN_STRINGL(data, len, 1);
      } else {
        RETURN_FALSE;
      }
    }
    
}

PHP_FUNCTION(zhtml2img)
{
  char *url = NULL;
  char *fmt = NULL;
  char *out = NULL;
  long quality = 80;
  long screenWidth = 1000;
  long screenHeight = NULL;
  int url_len, fmt_len, out_len;

  long len;
  const unsigned char * data;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|sslll", &url, &url_len, &out, &out_len, &fmt, &fmt_len, &quality, &screenWidth, &screenHeight) == FAILURE) {
    return;
  }

    if(!ZHTMLTOPDF_G(zhtml2img_initialized)) {
        ZHTMLTOPDF_G(zhtml2img_initialized) = wkhtmltoimage_init(0);
    }

    wkhtmltoimage_global_settings * gs;
    wkhtmltoimage_converter * c;


    gs = wkhtmltoimage_create_global_settings();

    wkhtmltoimage_set_global_setting(gs, "in", url);

    if(fmt == NULL) {
      fmt = "jpeg";
    }

    wkhtmltoimage_set_global_setting(gs, "fmt", fmt);

    if(out) {
      wkhtmltoimage_set_global_setting(gs, "out", out);
    }

    if(quality < 1) {
      quality = 80;
    }
    //convert quality from long to char
    //to avoid segmentfault in libwkhtmltox.so.o
    const int n = snprintf(NULL,0,"%ld",quality);
    char buffer[n+1];
    int cnt = snprintf(buffer,n+1,"%ld",quality);
    buffer[n] = '\0';
    assert(cnt == n);
    wkhtmltoimage_set_global_setting(gs,"quality",buffer);

    // 设置宽度
    if (screenWidth < 1) {
      screenWidth = 1000;
    }
    const int k = snprintf(NULL,0,"%ld",screenWidth);
    char swBuffer[k+1];
    int swCnt = snprintf(swBuffer,k+1,"%ld",screenWidth);
    swBuffer[k] = '\0';
    assert(swCnt == k);
    wkhtmltoimage_set_global_setting(gs, "screenWidth", swBuffer);

    // 设置高度
    if (screenHeight) {
      if (screenHeight < 1) {
        screenHeight = 1000;
      }
      const int h = snprintf(NULL,0,"%ld",screenHeight);
      char shBuffer[h+1];
      int shCnt = snprintf(shBuffer,h+1,"%ld",screenHeight);
      shBuffer[h] = '\0';
      assert(shCnt == h);
      wkhtmltoimage_set_global_setting(gs, "screenHeight", shBuffer);
    }

    c = wkhtmltoimage_create_converter(gs, NULL);

    if (!wkhtmltoimage_convert(c)) {
      RETURN_FALSE;
      return;
    }

    if(out) {
      wkhtmltoimage_destroy_converter(c);
      RETURN_TRUE;
    } else {
      len = wkhtmltoimage_get_output(c, &data);
      wkhtmltoimage_destroy_converter(c);
      RETURN_STRINGL(data, len, 1);
    }
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
