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
		(zend_objects_free_object_storage_t)php_sundown_render_base_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, render_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_set_render_flags, 0, 0, 1)
	ZEND_ARG_INFO(0, render_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_block_code, 0, 0, 2)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_INFO(0, language)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_block_quote, 0, 0, 1)
	ZEND_ARG_INFO(0, quote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_block_html, 0, 0, 1)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_header, 0, 0, 2)
	ZEND_ARG_INFO(0, htext)
	ZEND_ARG_INFO(0, header_level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_hrule, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_list_box, 0, 0, 2)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_list_item, 0, 0, 2)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_paragraph, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_table, 0, 0, 2)
	ZEND_ARG_INFO(0, header)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_table_row, 0, 0, 1)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_table_cell, 0, 0, 2)
	ZEND_ARG_INFO(0, content)
	ZEND_ARG_INFO(0, alignment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_autolink, 0, 0, 2)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, link_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_codespan, 0, 0, 1)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_double_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_image, 0, 0, 3)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, alt_text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_linebreak, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_link, 0, 0, 3)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_raw_html, 0, 0, 1)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_triple_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_strikethrough, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_superscript, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_entity, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_normal_text, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_doc_header, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_doc_footer, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_preprocess, 0, 0, 1)
	ZEND_ARG_INFO(0, full_document)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_postprocess, 0, 0, 1)
	ZEND_ARG_INFO(0, full_document)
ZEND_END_ARG_INFO()

/* {{{ proto string Sundown\Render\Base::blockCode($code, $language)
*/
PHP_METHOD(sundown_render_base, blockCode)
{
	char *code, *block_code;
	int code_len, block_code_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss", &code, &code_len, &block_code, &block_code_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(code, code_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::blockQuote($quote)
*/
PHP_METHOD(sundown_render_base, blockQuote)
{
	char *quote;
	int quote_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &quote, &quote_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(quote, quote_len,1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::blockHtml($raw_html)
*/
PHP_METHOD(sundown_render_base, blockHtml)
{
	char *raw;
	int raw_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &raw, &raw_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(raw, raw_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::header($htext, $header_level)
*/
PHP_METHOD(sundown_render_base, header)
{
	char *htext;
	int htext_len, header_level;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &htext, &htext_len, &header_level) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(htext, htext_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::hrule()
*/
PHP_METHOD(sundown_render_base, hrule)
{
/** 
 * on Sundown\Render\Base, actually this method do nothing.
 **/
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::listBox($contents, $list_type)
*/
PHP_METHOD(sundown_render_base, listBox)
{
	char *contents;
	int contents_len;
	long list_type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &contents, &contents_len, &list_type) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(contents, contents_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::listItem($text, $list_type)
*/
PHP_METHOD(sundown_render_base, listItem)
{
	char *text;
	int text_len;
	long list_type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &text, &text_len, &list_type) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::paragraph($text)
*/
PHP_METHOD(sundown_render_base, paragraph)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::table($header, $body)
*/
PHP_METHOD(sundown_render_base, table)
{
	char *header, *body;
	int header_len, body_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss", &header, &header_len, &body, &body_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(body, body_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::tableRow($content)
*/
PHP_METHOD(sundown_render_base, tableRow)
{
	char *contents;
	int contents_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &contents, &contents_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(contents, contents_len, 1);	
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::tableCell($content, $alignment)
*/
PHP_METHOD(sundown_render_base, tableCell)
{
	char *content, *alignment;
	int content_len, alignment_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss", &content, &content_len, &alignment, &alignment_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(content, content_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::autolink($link, $link_type)
*/
PHP_METHOD(sundown_render_base, autolink)
{
	char *link;
	int link_len;
	long link_type;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &link, &link_len, &link_type) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(link, link_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::codespan($code)
*/
PHP_METHOD(sundown_render_base, codespan)
{
	char *code;
	int code_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &code, &code_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(code, code_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::doubleEmphasis($text)
*/
PHP_METHOD(sundown_render_base, doubleEmphasis)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::emphasis($text)
*/
PHP_METHOD(sundown_render_base, emphasis)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::image($link, $title, $alt_text)
*/
PHP_METHOD(sundown_render_base, image)
{
	char *link, *title, *alt_text;
	int link_len, title_len, alt_text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sss", &link, &link_len, &title, &title_len, &alt_text, &alt_text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(link, link_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::linebreak()
*/
PHP_METHOD(sundown_render_base, linebreak)
{
/** 
 * on Sundown\Render\Base, actually this method do nothing.
 **/
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::link($link, $title, $content)
*/
PHP_METHOD(sundown_render_base, link)
{
	char *link, *title, *content;
	int link_len, title_len, content_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sss", &link, &link_len, &title, &title_len, &content, &content_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(link, link_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::rawHtml($raw_html)
*/
PHP_METHOD(sundown_render_base, rawHtml)
{
	char *raw;
	int raw_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &raw, &raw_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(raw, raw_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::tripleEmphasis($text)
*/
PHP_METHOD(sundown_render_base, tripleEmphasis)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);	
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::strikethrough($text)
*/
PHP_METHOD(sundown_render_base, strikethrough)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::superscript($text)
*/
PHP_METHOD(sundown_render_base, superscript)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::entity($text)
*/
PHP_METHOD(sundown_render_base, entity)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::normalText($text)
*/
PHP_METHOD(sundown_render_base, normalText)
{
	char *text;
	int text_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::docHeader()
*/
PHP_METHOD(sundown_render_base, docHeader)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::docFooter()
*/
PHP_METHOD(sundown_render_base, docFooter)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::preprocess($full_document)
*/
PHP_METHOD(sundown_render_base, preprocess)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	
	RETVAL_STRINGL(text, text_len, 1);
}
/* }}} */

/* {{{ proto string Sundown\Render\Base::postprocess($full_document)
*/
PHP_METHOD(sundown_render_base, postprocess)
{
	char *text;
	int text_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	if (Z_BVAL_P(zend_read_property(Z_OBJCE_P(getThis()), getThis(), "enable_pants", sizeof("enable_pants")-1, 1 TSRMLS_CC))) {
		struct buf *smart_buf = bufnew(128);
		sdhtml_smartypants(smart_buf, (uint8_t *)text, text_len);
		RETVAL_STRINGL((char*)smart_buf->data, smart_buf->size, 1);
		bufrelease(smart_buf);
	} else {
		RETVAL_STRINGL(text, text_len, 1);
	}
}
/* }}} */

/* {{{ proto Sundown\Render\Base::__construct([array render_flags])
*/
PHP_METHOD(sundown_render_base, __construct)
{
	zval *render_flags = NULL, *c_flags = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"|z", &render_flags) == FAILURE) {
		return;
	}

	if (render_flags != NULL && Z_TYPE_P(render_flags) == IS_ARRAY) {
		ALLOC_INIT_ZVAL(c_flags);
		ZVAL_ZVAL(c_flags, render_flags, 1, 0);
	} else {
		MAKE_STD_ZVAL(c_flags);
		array_init(c_flags);
	}
	add_property_zval_ex(getThis(), "render_flags", sizeof("render_flags"), c_flags TSRMLS_CC);

}
/* }}} */



/* {{{ proto array Sundown\Render\Base::getRenderFlags()
*/
PHP_METHOD(sundown_render_base, getRenderFlags)
{
	if (Z_TYPE_P(zend_read_property(sundown_render_base_class_entry, getThis(), "render_flags", sizeof("render_flags")-1, 0 TSRMLS_CC)) != IS_NULL) {
		zval *tmp = NULL;

		tmp = zend_read_property(sundown_render_base_class_entry, getThis(), "render_flags", sizeof("render_flags")-1, 0 TSRMLS_CC);
		RETVAL_ZVAL(tmp, 1, 0);
	}
}
/* }}} */

/* {{{ proto void Sundown\Render\Base::setRenderFlags(array render_flags)
*/
PHP_METHOD(sundown_render_base, setRenderFlags)
{
	zval *render_flags, *c_render_flags, *tmp = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"a", &render_flags) == FAILURE) {
		return;
	}
	MAKE_STD_ZVAL(c_render_flags);
	ZVAL_ZVAL(c_render_flags, render_flags, 1, 0);
	tmp = zend_read_property(sundown_render_base_class_entry, getThis(), "render_flags", sizeof("render_flags")-1, 0 TSRMLS_CC);
	zval_ptr_dtor(&tmp);
	
	add_property_zval_ex(getThis(), "render_flags", sizeof("render_flags"), c_render_flags TSRMLS_CC);
}
/* }}} */


/* {{{ proto Sundown\Render\Base::__destruct()
*/
PHP_METHOD(sundown_render_base, __destruct)
{
	zval *render_flags;
	render_flags = zend_read_property(sundown_render_base_class_entry, getThis(), "render_flags", sizeof("render_flags")-1, 0 TSRMLS_CC);
	if (Z_TYPE_P(render_flags) == IS_ARRAY) {
		zval_ptr_dtor(&render_flags);
	}
}

static zend_function_entry php_sundown_render_base_methods[] = {
	PHP_ME(sundown_render_base, __construct,    arginfo_sundown_render___construct,          ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(sundown_render_base, getRenderFlags, NULL,                                        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, setRenderFlags, arginfo_sundown_render_base_set_render_flags,ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, blockCode,      arginfo_sundown_render_base_block_code,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, blockQuote,     arginfo_sundown_render_base_block_quote,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, blockHtml,      arginfo_sundown_render_base_block_html,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, header,         arginfo_sundown_render_base_header,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, hrule,          arginfo_sundown_render_base_hrule,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, listBox,        arginfo_sundown_render_base_list_box,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, listItem,       arginfo_sundown_render_base_list_item,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, paragraph,      arginfo_sundown_render_base_paragraph,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, table,          arginfo_sundown_render_base_table,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, tableRow,       arginfo_sundown_render_base_table_row,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, tableCell,      arginfo_sundown_render_base_table_cell,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, autolink,       arginfo_sundown_render_base_autolink,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, codespan,       arginfo_sundown_render_base_codespan,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, doubleEmphasis, arginfo_sundown_render_base_double_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, emphasis,       arginfo_sundown_render_base_emphasis,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, image,          arginfo_sundown_render_base_image,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, linebreak,      arginfo_sundown_render_base_linebreak,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, link,           arginfo_sundown_render_base_link,            ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, rawHtml,        arginfo_sundown_render_base_raw_html,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, tripleEmphasis, arginfo_sundown_render_base_triple_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, strikethrough,  arginfo_sundown_render_base_strikethrough,   ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, superscript,    arginfo_sundown_render_base_superscript,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, entity,         arginfo_sundown_render_base_entity,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, normalText,     arginfo_sundown_render_base_normal_text,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, docHeader,      arginfo_sundown_render_base_doc_header,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, docFooter,      arginfo_sundown_render_base_doc_footer,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, preprocess,     arginfo_sundown_render_base_preprocess,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, postprocess,    arginfo_sundown_render_base_postprocess,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_base, __destruct,     NULL,                                        ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_sundown_render_base_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown", "Render"), "Base", php_sundown_render_base_methods);
	sundown_render_base_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	sundown_render_base_class_entry->create_object = php_sundown_render_base_new;
	zend_declare_property_null(sundown_render_base_class_entry, ZEND_STRS("render_flags")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_class_constant_long(sundown_render_base_class_entry, ZEND_STRS("LIST_ORDERED")-1, 1 TSRMLS_CC);
	zend_declare_class_constant_long(sundown_render_base_class_entry, ZEND_STRS("MKDA_EMAIL")-1, MKDA_EMAIL TSRMLS_CC);
	zend_declare_class_constant_long(sundown_render_base_class_entry, ZEND_STRS("MKDA_NORMAL")-1,MKDA_NORMAL TSRMLS_CC);
}
