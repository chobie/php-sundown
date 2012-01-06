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

zend_class_entry *sundown_render_base_class_entry;

static void php_sundown_render_base_free_storage(php_sundown_render_base_t *obj TSRMLS_DC)
{
	zend_object_std_dtor(&obj->zo TSRMLS_CC);
	efree(obj);
}

zend_object_value php_sundown_render_base_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	php_sundown_render_base_t *obj;
	zval *tmp;

	obj = ecalloc(1, sizeof(*obj));
	zend_object_std_init( &obj->zo, ce TSRMLS_CC);
	zend_hash_copy(obj->zo.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, 
		(zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t)php_sundown_render_base_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_enable_pants, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_block_code, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_INFO(0, language)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_block_quote, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, quote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_block_html, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_header, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, htext)
	ZEND_ARG_INFO(0, header_level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_hrule, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_list_box, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_list_item, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_paragraph, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_table, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, header)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_table_row, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_table_cell, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, content)
	ZEND_ARG_INFO(0, alignment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_autolink, 0, 0, 4)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, link_type)
	ZEND_ARG_INFO(0, link)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_codespan, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_double_emphasis, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_emphasis, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_image, 0, 0, 4)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, alt_text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_linebreak, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_link, 0, 0, 4)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_raw_html, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_triple_emphasis, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_strikethrough, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_superscript, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_entity, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_normal_text, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_doc_header, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_doc_footer, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_preprocess, 0, 0, 1)
	ZEND_ARG_INFO(0, full_document)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_postprocess, 0, 0, 1)
	ZEND_ARG_INFO(0, full_document)
ZEND_END_ARG_INFO()

/* {{{ proto void enable_pants()
*/
PHP_METHOD(sundown_render_base, enable_pants)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void block_code($buffer, $code, $language)
*/
PHP_METHOD(sundown_render_base, block_code)
{
	char *code, *block_code;
	int code_len, block_code_len;
	zval *buffer;
	php_sundown_buffer_t *object;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zss", &buffer,&code, &code_len, &block_code, &block_code_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, code, code_len);
}
/* }}} */

/* {{{ proto void block_quote($buffer, $quote)
*/
PHP_METHOD(sundown_render_base, block_quote)
{
	char *quote;
	int quote_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&quote, &quote_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, quote, quote_len);
}
/* }}} */

/* {{{ proto void block_html($buffer, $raw_html)
*/
PHP_METHOD(sundown_render_base, block_html)
{
	char *raw;
	int raw_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&raw, &raw_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, raw, raw_len);
}
/* }}} */

/* {{{ proto void header($buffer, $htext,$header_level)
*/
PHP_METHOD(sundown_render_base, header)
{
	char *htext;
	long htext_len, header_level;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zsl", &buffer,&htext, &htext_len, &header_level) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, htext, htext_len);
}
/* }}} */

/* {{{ proto void hrule()
*/
PHP_METHOD(sundown_render_base, hrule)
{
}
/* }}} */

/* {{{ proto void list_box($buffer, $contents, $list_type)
*/
PHP_METHOD(sundown_render_base, list_box)
{
	char *contents, *list_type;
	int contents_len, list_type_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zss", &buffer, &contents, &contents_len, &list_type, &list_type_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, contents, contents_len);
}
/* }}} */

/* {{{ proto void list_item($buffer, $text, $list_type)
*/
PHP_METHOD(sundown_render_base, list_item)
{
	char *text, *list_type;
	int text_len, list_type_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zss", &buffer,&text, &text_len, &list_type, &list_type_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void paragraph($buffer, $text)
*/
PHP_METHOD(sundown_render_base, paragraph)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void table($buffer, $header, $body)
*/
PHP_METHOD(sundown_render_base, table)
{
	char *header, *body;
	int header_len, body_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zss", &buffer, &header, &header_len, &body, &body_len) == FAILURE){
		return;
	}
	
	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, body, body_len);
}
/* }}} */

/* {{{ proto void table_row($buffer, $content)
*/
PHP_METHOD(sundown_render_base, table_row)
{
	char *contents;
	int contents_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&contents, &contents_len) == FAILURE){
		return;
	}
	
	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, contents, contents_len);
}
/* }}} */

/* {{{ proto void table_cell($buffer, $content, $alignment)
*/
PHP_METHOD(sundown_render_base, table_cell)
{
	char *content, *alignment;
	int content_len, alignment_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zss", &buffer,&content, &content_len, &alignment, &alignment_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, content, content_len);
}
/* }}} */

/* {{{ proto void autolink($buffer, $link, $link_type)
*/
PHP_METHOD(sundown_render_base, autolink)
{
	char *link, *link_type;
	int link_len, link_type_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zss", &buffer,&link, &link_len, &link_type, &link_type_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, link, link_len);
}
/* }}} */

/* {{{ proto void codespan($buffer, $code)
*/
PHP_METHOD(sundown_render_base, codespan)
{
	char *code;
	int code_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&code, &code_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, code, code_len);
}
/* }}} */

/* {{{ proto void double_emphasis($buffer, $text)
*/
PHP_METHOD(sundown_render_base, double_emphasis)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void emphasis($buffer, $text)
*/
PHP_METHOD(sundown_render_base, emphasis)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void image($buffer, $link, $title, $alt_text)
*/
PHP_METHOD(sundown_render_base, image)
{
	char *link, *title, *alt_text;
	int link_len, title_len, alt_text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zsss", &buffer,&link, &link_len, &title, &title_len, &alt_text, &alt_text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, link, link_len);
}
/* }}} */

/* {{{ proto void linebreak($buffer)
*/
PHP_METHOD(sundown_render_base, linebreak)
{
}
/* }}} */

/* {{{ proto void link($link,$title,$content)
*/
PHP_METHOD(sundown_render_base, link)
{
	char *link, *title, *content;
	int link_len, title_len, content_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zsss", &buffer,&link, &link_len, &title, &title_len, &content, &content_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, link, link_len);
}
/* }}} */

/* {{{ proto void raw_html($buffer, $raw_html)
*/
PHP_METHOD(sundown_render_base, raw_html)
{
	char *raw;
	int raw_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&raw, &raw_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, raw, raw_len);
}
/* }}} */

/* {{{ proto void triple_emphasis($buffer, $text)
*/
PHP_METHOD(sundown_render_base, triple_emphasis)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&text, &text_len) == FAILURE){
		return;
	}
	
	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void strikethrough($buffer, $text)
*/
PHP_METHOD(sundown_render_base, strikethrough)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void superscript($buffer, $text)
*/
PHP_METHOD(sundown_render_base, superscript)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void entity($buffer, $text)
*/
PHP_METHOD(sundown_render_base, entity)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer, &text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void normal_text($buffer, $text)
*/
PHP_METHOD(sundown_render_base, normal_text)
{
	char *text;
	int text_len;
	zval *buffer;
	php_sundown_buffer_t *object;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer, &text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, text, text_len);
}
/* }}} */

/* {{{ proto void doc_header()
*/
PHP_METHOD(sundown_render_base, doc_header)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void doc_footer()
*/
PHP_METHOD(sundown_render_base, doc_footer)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void preprocess($full_document)
*/
PHP_METHOD(sundown_render_base, preprocess)
{
	char *text;
	int text_len;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE){
		return;
	}
	
	RETURN_STRINGL(text,text_len, 1);
}
/* }}} */

/* {{{ proto void postprocess($full_document)
*/
PHP_METHOD(sundown_render_base, postprocess)
{
	char *text;
	int text_len;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE){
		return;
	}
	
	RETURN_STRINGL(text,text_len, 1);
}
/* }}} */

static zend_function_entry php_sundown_render_base_methods[] = {
	PHP_ME(sundown_render_base, enable_pants,    arginfo_sundown_render_base_enable_pants,    ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, block_code,      arginfo_sundown_render_base_block_code,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, block_quote,     arginfo_sundown_render_base_block_quote,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, block_html,      arginfo_sundown_render_base_block_html,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, header,          arginfo_sundown_render_base_header,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, hrule,           arginfo_sundown_render_base_hrule,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, list_box,        arginfo_sundown_render_base_list_box,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, list_item,       arginfo_sundown_render_base_list_item,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, paragraph,       arginfo_sundown_render_base_paragraph,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, table,           arginfo_sundown_render_base_table,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, table_row,       arginfo_sundown_render_base_table_row,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, table_cell,      arginfo_sundown_render_base_table_cell,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, autolink,        arginfo_sundown_render_base_autolink,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, codespan,        arginfo_sundown_render_base_codespan,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, double_emphasis, arginfo_sundown_render_base_double_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, emphasis,        arginfo_sundown_render_base_emphasis,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, image,           arginfo_sundown_render_base_image,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, linebreak,       arginfo_sundown_render_base_linebreak,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, link,            arginfo_sundown_render_base_link,            ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, raw_html,        arginfo_sundown_render_base_raw_html,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, triple_emphasis, arginfo_sundown_render_base_triple_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, strikethrough,   arginfo_sundown_render_base_strikethrough,   ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, superscript,     arginfo_sundown_render_base_superscript,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, entity,          arginfo_sundown_render_base_entity,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, normal_text,     arginfo_sundown_render_base_normal_text,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, doc_header,      arginfo_sundown_render_base_doc_header,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, doc_footer,      arginfo_sundown_render_base_doc_footer,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, preprocess,      arginfo_sundown_render_base_preprocess,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, postprocess,     arginfo_sundown_render_base_postprocess,     ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_sundown_render_base_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown","Render"),"Base", php_sundown_render_base_methods);
	sundown_render_base_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	sundown_render_base_class_entry->create_object = php_sundown_render_base_new;

}
