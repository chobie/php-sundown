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
   | Authors: Shuhei Tanuma <chobieeee@php.net>                           |
   +----------------------------------------------------------------------+
 */
#include "php_sundown.h"
#include "ext/standard/php_array.h"

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

	obj = ecalloc(1, sizeof(*obj));
	zend_object_std_init( &obj->zo, ce TSRMLS_CC);
#if ZEND_MODULE_API_NO >= 20100525
		object_properties_init(&(obj->zo), ce);
#else
	{
		zval *tmp;
		zend_hash_copy(obj->zo.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
	}
#endif

	retval.handle = zend_objects_store_put(obj, 
		(zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t)php_sundown_render_xhtml_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_xhtml__construct,0,0,1)
	ZEND_ARG_INFO(0, render_flags)
ZEND_END_ARG_INFO()

/* {{{ proto Sundown\Render\XHTML::__construct([array render_flags])
*/
PHP_METHOD(sundown_render_xhtml, __construct)
{
	zval *render_flags = NULL, *enable_xhtml, *t;
	char *key = "xhtml";
	uint length = sizeof("xhtml");
	php_sundown_render_html_t *object;
	struct php_sundown_renderopt_ex opt;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"|a", &render_flags) == FAILURE){
		return;
	}

	if (render_flags == NULL) {
		MAKE_STD_ZVAL(render_flags);
		array_init(render_flags);
	}

	MAKE_STD_ZVAL(enable_xhtml);
	MAKE_STD_ZVAL(t);
	ZVAL_TRUE(t);

	array_init(enable_xhtml);
	zend_hash_add(Z_ARRVAL_P(enable_xhtml), key, length, &t, sizeof(zval *), NULL);
	Z_ADDREF_P(t);

	/* NOTE: XHTML Render always override `xhtml` value to `true`. */
	zend_hash_merge(Z_ARRVAL_P(render_flags), Z_ARRVAL_P(enable_xhtml), (copy_ctor_func_t)zval_add_ref, NULL, sizeof(zval *), 1);
	add_property_zval_ex(getThis(), ZEND_STRS("render_flags"), render_flags TSRMLS_CC);
	zval_ptr_dtor(&enable_xhtml);
	zval_ptr_dtor(&t);

	object = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	sdhtml_renderer(&object->cb, &opt.html, 0);
	opt.self = getThis();
}
/* }}} */


static zend_function_entry php_sundown_render_xhtml_methods[] = {
	PHP_ME(sundown_render_xhtml, __construct, arginfo_sundown_render_xhtml__construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	{NULL,NULL,NULL}
};

void php_sundown_render_xhtml_init(TSRMLS_D)
{
	zend_class_entry ce;
	
	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown","Render"),"XHTML", php_sundown_render_xhtml_methods);
	sundown_render_xhtml_class_entry = zend_register_internal_class_ex(&ce, sundown_render_html_class_entry, NULL TSRMLS_CC);
	sundown_render_xhtml_class_entry->create_object = php_sundown_render_xhtml_new;
	zend_declare_property_null(sundown_render_xhtml_class_entry, ZEND_STRS("render_flags")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
}
