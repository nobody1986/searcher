/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
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

#include<searcher.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_searcher.h"

/* If you declare any globals in php_searcher.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(searcher)
*/

/* True global resources - no need for thread safety here */
static int le_searcher;

/* {{{ searcher_functions[]
 *
 * Every user visible function must have an entry in searcher_functions[].
 */
const zend_function_entry searcher_methods[] = {
	//PHP_FE(confirm_searcher_compiled,	NULL)		/* For testing, remove later. */
	PHP_ME(Searcher,__construct,NULL,ZEND_ACC_PUBLIC|ZEND_ACC_CTOR),
	PHP_ME(Searcher,__destruct,NULL,ZEND_ACC_PUBLIC|ZEND_ACC_DTOR),
	PHP_ME(Searcher,saveToFile,saveToFile_args,ZEND_ACC_PUBLIC),
	PHP_ME(Searcher,search,search_args,ZEND_ACC_PUBLIC),
	PHP_FE_END	/* Must be the last line in searcher_functions[] */
};
/* }}} */

/* {{{ searcher_module_entry
 */
zend_module_entry searcher_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"searcher",
	searcher_functions,
	PHP_MINIT(searcher),
	PHP_MSHUTDOWN(searcher),
	PHP_RINIT(searcher),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(searcher),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(searcher),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SEARCHER
ZEND_GET_MODULE(searcher)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("searcher.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_searcher_globals, searcher_globals)
    STD_PHP_INI_ENTRY("searcher.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_searcher_globals, searcher_globals)
PHP_INI_END()
*/
/* }}} */ 

/* {{{ php_searcher_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_searcher_init_globals(zend_searcher_globals *searcher_globals)
{
	searcher_globals->global_value = 0;
	searcher_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(searcher)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	zend_class_entry searcher;  
    INIT_CLASS_ENTRY(searcher,"Searcher",searcher_methods);//初始化  
    searcher_ce=zend_register_internal_class_ex(&searcher,NULL,NULL TSRMLS_DC);  
    searcher_ce->ce_flags=ZEND_ACC_IMPLICIT_PUBLIC;  
	return SUCCESS;
}
/* }}} */ 

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(searcher)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(searcher)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(searcher)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(searcher)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "searcher support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_searcher_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_METHOD(Searcher,__construct)
{
	char *rulefile = NULL,*imgfile = NULL;
	int rulefile_len,imgfile_len;
	zval *rulefile_zval,*imgfile_zval,*cvalue,*nnvalue, *self;
	void* c = NULL;
	int nn = 0;
	long ret = 0;
 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "!zz", &rulefile_zval, &imgfile_zval) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
 	
 	if(Z_TYPE_P(rulefile_zval) != IS_NULL){
 		c = parseRuleFile(Z_STRVAL_P(rulefile_zval),&nn);
 	}else{
 		if(Z_TYPE_P(imgfile_zval) != IS_NULL){
 			ret = releaseFromFile(Z_STRVAL_P(imgfile_zval),&c);
 		}else{
 			WRONG_PARAM_COUNT;
 		}
 	}

 	

	self = getThis();
 
	MAKE_STD_ZVAL(cvalue);
	ZVAL_LONG(cvalue, (int)c);

	SEPARATE_ZVAL_TO_MAKE_IS_REF(&cvalue);
	zend_update_property(Z_OBJCE_P(self), self, ZEND_STRL("_c"), cvalue TSRMLS_CC);

	MAKE_STD_ZVAL(nnvalue);
	ZVAL_LONG(nnvalue, nn);

	SEPARATE_ZVAL_TO_MAKE_IS_REF(&nnvalue);
	zend_update_property(Z_OBJCE_P(self), self, ZEND_STRL("_nn"), nnvalue TSRMLS_CC);
 
	RETURN_TRUE;
}
/* }}} */ 
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_METHOD(Searcher,__destruct)
{
	
 
	RETURN_TRUE;
}

PHP_METHOD(Searcher,search)
{
	char *str = NULL;
	int str_len,level;
	zval *cvalue, *self,*callback;
	void* c = NULL;
	long ret = 0;
 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl|f", &str,&str_len, &level,&callback) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
 	

 	

	self = getThis();
 
	cvalue = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_c"), 0 TSRMLS_CC);
	c = (void*)Z_LVAL_P(cvalue);

 	ret = search(c,str,str_len,level,NULL);

	RETURN_LONG(ret);
}

PHP_METHOD(Searcher,saveToFile)
{
	char *imgfile = NULL;
	int imgfile_len;
	zval *cvalue,*nnvalue, *self;
	void* c = NULL;
	int nn = 0;
	long ret = 0;
 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &imgfile, &imgfile_len) == FAILURE) {
		WRONG_PARAM_COUNT;
	}
 	
	self = getThis();
	nnvalue = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_nn"), 0 TSRMLS_CC);
	nn = Z_LVAL_P(nnvalue);
 	if(nn == 0){
 		WRONG_PARAM_COUNT;
 	}
	
	cvalue = zend_read_property(Z_OBJCE_P(self), self, ZEND_STRL("_c"), 0 TSRMLS_CC);
	c = (void*)Z_LVAL_P(cvalue);

	ret = saveToFile(c,nn,imgfile);

	RETURN_LONG(ret);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
