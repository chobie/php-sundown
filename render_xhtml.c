/*
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2011 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Shuhei Tanuma <chobieee@gmail.com>                          |
   +----------------------------------------------------------------------+
 */


#include "php_sundown.h"
extern zend_class_entry *sundown_render_html_class_entry;
zend_class_entry *sundown_render_xhtml_class_entry;

static void php_sundown_render_xhtml_free_storage(php_sundown_render_xhtml_t *obj TSRMLS_DC)
{
	zend_object_std_dtor(&obj->zo TSRMLS_CC);
	efree(obj);
}

zend_object_value php_sundown_render_xhtml_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	php_sundown_render_xhtml_t *obj;
	zval *tmp;

	obj = ecalloc(1, sizeof(*obj));
	zend_object_std_init( &obj->zo, ce TSRMLS_CC);
#if ZEND_MODULE_API_NO >= 20100525
	object_properties_init(&(obj->zo), ce);
#else
	zend_hash_copy(obj->zo.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
#endif

	retval.handle = zend_objects_store_put(obj, 
		(zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t)php_sundown_render_xhtml_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render___construct,0,0,1)
	ZEND_ARG_INFO(0, render_flags)
ZEND_END_ARG_INFO()

/* {{{ proto Sundown\Render\XHTML::__construct([array render_flags])
*/
PHP_METHOD(sundown_render_xhtml, __construct)
{
	zval *render_flags = NULL, *c_flags = NULL;
	HashTable *hash;
	char *key = "xhtml";
	zval **data;
	long length = sizeof("xhtml");
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"|z", &render_flags) == FAILURE){
		return;
	}

	if (render_flags != NULL && Z_TYPE_P(render_flags) == IS_ARRAY) {
		ALLOC_INIT_ZVAL(c_flags);
		ZVAL_ZVAL(c_flags, render_flags, 1, 1);
	} else {
		zval *t;
		MAKE_STD_ZVAL(t);
		ZVAL_TRUE(t);
		MAKE_STD_ZVAL(c_flags);
		array_init(c_flags);
		hash = Z_ARRVAL_P(c_flags);
		zend_hash_add(hash, key, length,&t,sizeof(zval *),NULL);
		Z_ADDREF_P(t);
		zval_ptr_dtor(&t);
		t = NULL;
	}

	hash = Z_ARRVAL_P(c_flags);
	if (zend_hash_find(hash, key, length, (void **)&data) != SUCCESS) {
		zval *t;
		MAKE_STD_ZVAL(t);
		ZVAL_TRUE(t);
		zend_hash_add(hash, key, length,&t,sizeof(zval *),NULL);
		Z_ADDREF_P(t);
	} else {
		if (!Z_BVAL_P(*data)) {
			zval *t;
			MAKE_STD_ZVAL(t);
			ZVAL_TRUE(t);
			zend_hash_update(hash, key, length,&t,sizeof(zval *),NULL);
			Z_ADDREF_P(t);
		}
	}
	
	add_property_zval_ex(getThis(),"render_flags",sizeof("render_flags"),c_flags TSRMLS_CC);
}
/* }}} */

/* {{{ proto Sundown\Render\XHTML::__destruct()
*/
PHP_METHOD(sundown_render_xhtml, __destruct)
{
	zval *render_flags;
	render_flags = zend_read_property(sundown_render_xhtml_class_entry, getThis(),"render_flags",sizeof("render_flags")-1, 0 TSRMLS_CC);
	if(Z_TYPE_P(render_flags) == IS_ARRAY) {
		zval_ptr_dtor(&render_flags);
	}
}


static zend_function_entry php_sundown_render_xhtml_methods[] = {
	PHP_ME(sundown_render_xhtml, __construct,     arginfo_sundown_render___construct,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_xhtml, __destruct,      NULL,                                        ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_sundown_render_xhtml_init(TSRMLS_D)
{
	zend_class_entry ce;
	
	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown","Render"),"XHTML", php_sundown_render_xhtml_methods);
	sundown_render_xhtml_class_entry = zend_register_internal_class_ex(&ce, sundown_render_html_class_entry, NULL TSRMLS_CC);
	sundown_render_xhtml_class_entry->create_object = php_sundown_render_xhtml_new;
	zend_declare_property_null(sundown_render_xhtml_class_entry, "render_flags", sizeof("render_flags")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
}
