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

zend_class_entry *sundown_buffer_class_entry;

static void php_sundown_buffer_free_storage(php_sundown_buffer_t *obj TSRMLS_DC)
{
	zend_object_std_dtor(&obj->zo TSRMLS_CC);
	if (obj->buffer != NULL) {
		//free(obj->buffer);
	}
	efree(obj);
}

zend_object_value php_sundown_buffer_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	php_sundown_buffer_t *obj;
	zval *tmp;

	obj = ecalloc(1, sizeof(*obj));
	zend_object_std_init( &obj->zo, ce TSRMLS_CC );
	zend_hash_copy(obj->zo.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, 
		(zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t)php_sundown_buffer_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}

PHP_METHOD(sundown_buffer, __construct)
{
	php_sundown_buffer_t *object = (php_sundown_buffer_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	struct buf *buffer;
	int length = 8192;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"|l", &length) == FAILURE){
		return;
	}

	buffer = bufnew(128);
	bufgrow(buffer, length);
	object->buffer = buffer;
}

PHP_METHOD(sundown_buffer, append)
{
	char *string;
	int string_len = 0;
	php_sundown_buffer_t *object = (php_sundown_buffer_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &string, &string_len) == FAILURE){
		return;
	}
	bufput(object->buffer, string, string_len);
}

PHP_METHOD(sundown_buffer, __toString)
{
	php_sundown_buffer_t *object = (php_sundown_buffer_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_STRINGL(object->buffer->data, object->buffer->size,1)
}

PHP_METHOD(sundown_buffer, length)
{
	php_sundown_buffer_t *object = (php_sundown_buffer_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_LONG(object->buffer->size);
}

static zend_function_entry php_sundown_buffer_methods[] = {
	PHP_ME(sundown_buffer, __construct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_buffer, append, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_buffer, length, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_buffer, __toString, NULL, ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_sundown_buffer_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, "Sundown","Buffer", php_sundown_buffer_methods);
	sundown_buffer_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	sundown_buffer_class_entry->create_object = php_sundown_buffer_new;
}
