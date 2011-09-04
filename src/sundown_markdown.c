/*
 * The MIT License
 *
 * Copyright (c) 2011 Shuhei Tanuma
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "php_sundown.h"

typedef struct{
    zend_object zo;
    zval *render;
} php_sundown_markdown_t;

extern struct sd_callbacks php_sundown_callbacks;
extern const char *php_sundown_method_names[];
extern const size_t php_sundown_method_count;


zend_class_entry *sundown_markdown_class_entry;

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown__construct, 0, 0, 2)
	ZEND_ARG_INFO(0, render)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown_render, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

static void php_sundown_markdown_free_storage(php_sundown_markdown_t *obj TSRMLS_DC)
{
	zend_object_std_dtor(&obj->zo TSRMLS_CC);
	if (obj->render != NULL) {
		zval_ptr_dtor(&obj->render);
	}
	efree(obj);
}

zend_object_value php_sundown_markdown_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	php_sundown_markdown_t *obj;
	zval *tmp;

	obj = ecalloc(1, sizeof(*obj));
	zend_object_std_init( &obj->zo, ce TSRMLS_CC );
	zend_hash_copy(obj->zo.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, 
		(zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t)php_sundown_markdown_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}


/* {{{ proto string __construct($render [, $options])
*/
PHP_METHOD(sundown_markdown, __construct)
{
	zval *render;
	zval *options;
	php_sundown_markdown_t *object = (php_sundown_markdown_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"z|a", &render, &options) == FAILURE){
		return;
	}

	if (Z_TYPE_P(render) == IS_STRING) {
		
	}
	object->render = render;
	zval_add_ref(&object->render);

	if (options == NULL) {
		MAKE_STD_ZVAL(options);
		array_init(options);
	}
	add_property_zval_ex(getThis(),"extensions",sizeof("extensions"),options TSRMLS_CC);

}
/* }}} */


/* {{{ proto void __destruct()
	cleanup variables */
PHP_METHOD(sundown_markdown, __destruct)
{
	zval *extensions;
	
	extensions = zend_read_property(sundown_markdown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC);
	if(extensions != NULL && Z_TYPE_P(extensions) != IS_NULL) {
		zval_ptr_dtor(&extensions);
	}
}
/* }}} */



/* {{{ proto string render($body)
*/
PHP_METHOD(sundown_markdown, render)
{
	php_sundown_markdown_t *object = (php_sundown_markdown_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	struct buf input_buf, *output_buf;
	struct sd_callbacks sundown_render;
	struct php_sundown_renderopt opt;
	unsigned int enabled_extensions = 0, render_flags = 0;
	char *buffer;
	int buffer_len = 0;
	zend_class_entry *ce;
	HashTable *table;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &buffer, &buffer_len) == FAILURE){
		return;
	}
	
	memset(&input_buf, 0x0, sizeof(struct buf));
	input_buf.data = buffer;
	input_buf.size = strlen(buffer);
	
	output_buf = bufnew(128);
	bufgrow(output_buf, strlen(buffer) * 1.2f);

	if(Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
		table = Z_ARRVAL_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC));
	}
	php_sundown__get_flags(table, &enabled_extensions, &render_flags);

	// setup render
	switch (SUNDOWN_RENDER_HTML) {
		case SUNDOWN_RENDER_HTML:
			sdhtml_renderer(&sundown_render, &opt.html, render_flags);
			break;
		case SUNDOWN_RENDER_TOC:
			sdhtml_toc_renderer(&sundown_render,&opt.html);
			break;
		default:
			RETURN_FALSE;
	}

	ce = Z_OBJCE_P(object->render);
	opt.self = object->render;
	void **source = (void **)&php_sundown_callbacks;
	void **dest = (void **)&sundown_render;
	size_t i;
	for (i = 0; i < php_sundown_method_count; ++i) {
		if (zend_hash_exists(&ce->function_table, php_sundown_method_names[i], strlen(php_sundown_method_names[i])+1)) {
			dest[i] = source[i];
		}
	}

	sd_markdown(output_buf, &input_buf, enabled_extensions, &sundown_render, &opt);
/*
	if (Z_BVAL_P(zend_read_property(ce, object->render,"enable_pants",sizeof("enable_pants")-1, 0 TSRMLS_CC))) {
		struct buf *smart_buf = bufnew(128);
		sdhtml_smartypants(smart_buf, output_buf);
		RETVAL_STRINGL(smart_buf->data, smart_buf->size,1);
		bufrelease(smart_buf);
	} else {
*/
		RETVAL_STRINGL(output_buf->data, output_buf->size,1);
//	}

	bufrelease(output_buf);
	bufrelease(&input_buf);
}


static zend_function_entry php_sundown_markdown_methods[] = {
	PHP_ME(sundown_markdown, __construct, arginfo_sundown_markdown__construct, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, __destruct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, render, arginfo_sundown_markdown_render, ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_sundown_markdown_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, "Sundown","Markdown", php_sundown_markdown_methods);
	sundown_markdown_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
    sundown_markdown_class_entry->create_object = php_sundown_markdown_new;
	zend_declare_property_null(sundown_markdown_class_entry, "extensions", sizeof("extensions")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
}
