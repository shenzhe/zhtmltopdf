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

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_zhtmltopdf.h"
#include "wkhtmltox/pdf.h"

///* If you declare any globals in php_zhtmltopdf.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(zhtmltopdf)
//*/

/* True global resources - no need for thread safety here */
//static int le_zhtmltopdf;

static void php_zhtmltopdf_init_globals(zend_zhtmltopdf_globals *zhtmltopdf_globals)
{
    zhtmltopdf_globals.zhtml2pdf_initialized = 0;
}



PHP_MINIT_FUNCTION(zhtmltopdf)
{
    ZEND_INIT_MODULE_GLOBALS(zhtmltopdf, php_zhtmltopdf_init_globals, NULL);
    return SUCCESS;

}
/* {{{ zhtmltopdf_functions[]
 *
 * Every user visible function must have an entry in zhtmltopdf_functions[].
 */
const zend_function_entry zhtmltopdf_functions[] = {
	PHP_FE(zhtml2pdf,	NULL)		/* For testing, remove later. */
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
/* Uncomment this function if you have INI entries
static void php_zhtmltopdf_init_globals(zend_zhtmltopdf_globals *zhtmltopdf_globals)
{
	zhtmltopdf_globals->global_value = 0;
	zhtmltopdf_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zhtmltopdf)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
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
	php_info_print_table_header(2, "wkhtmltopdf_version", wkhtmltopdf_version());
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
	int out_len, url_len, cookie_len, len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss|s", &our, &out_len, &url, &url_len, &cookie, &cookie_len) == FAILURE) {
		return;
	}

    if(!ZHTMLTOPDF_G(zhtml2pdf_initialized)) {
        ZHTMLTOPDF_G(zhtml2pdf_initialized)= wkhtmltopdf_init(false);
        
    }
    wkhtmltopdf_global_settings * gs;
    wkhtmltopdf_object_settings * os;
    wkhtmltopdf_converter * c;


    gs = wkhtmltopdf_create_global_settings();

    wkhtmltopdf_set_global_setting(gs, "out", out);

    if(cookie){
        wkhtmltopdf_set_global_setting(gs, "load.cookieJar", cookie);
    }

    os = wkhtmltopdf_create_object_settings();
    wkhtmltopdf_set_object_setting(os, "page", url);
    wkhtmltopdf_add_object(c, os, NULL);
    
    int ret;
    ret = wkhtmltopdf_convert(c);
    wkhtmltopdf_destroy_converter(c)
    RETVAL_BOOL(ret);
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
