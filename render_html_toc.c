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
#include "html.h"

static void php_sundown_render_html_toc_free_storage(php_sundown_render_html_toc_t *obj TSRMLS_DC)
{
	zend_object_std_dtor(&obj->zo TSRMLS_CC);
	efree(obj);
}

zend_object_value php_sundown_render_html_toc_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	php_sundown_render_html_toc_t *obj;
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
		(zend_objects_free_object_storage_t)php_sundown_render_html_toc_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}

zend_class_entry *sundown_render_html_toc_class_entry;
extern zend_class_entry *sundown_render_base_class_entry;

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, render_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_block_code, 0, 0, 2)
	ZEND_ARG_INFO(0, language)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_block_quote, 0, 0, 1)
	ZEND_ARG_INFO(0, quote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_block_html, 0, 0, 1)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_header, 0, 0, 2)
	ZEND_ARG_INFO(0, htext)
	ZEND_ARG_INFO(0, header_level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_hrule, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_list_box, 0, 0, 2)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_list_item, 0, 0, 2)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_paragraph, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_table, 0, 0, 2)
	ZEND_ARG_INFO(0, header)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_table_row, 0, 0, 1)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_table_cell, 0, 0, 2)
	ZEND_ARG_INFO(0, content)
	ZEND_ARG_INFO(0, alignment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_autolink, 0, 0, 2)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, link_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_codespan, 0, 0, 1)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_double_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_image, 0, 0, 3)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, alt_text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_linebreak, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_link, 0, 0, 3)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_raw_html, 0, 0, 1)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_triple_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_strikethrough, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_superscript, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_entity, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_normal_text, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_doc_header, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_doc_footer, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_toc_postprocess, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

/* {{{ proto stirng Sundown\Render\HTML_TOC::blockCode($language, $code)
*/
PHP_METHOD(sundown_render_html_toc, blockCode)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::blockQuote($quote)
*/
PHP_METHOD(sundown_render_html_toc, blockQuote)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::blockHtml()
*/
PHP_METHOD(sundown_render_html_toc, blockHtml)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::header($htext, $header_level)
*/
PHP_METHOD(sundown_render_html_toc, header)
{
	char *htext;
	int htext_len;
	long header_level;
	struct buf *input, *output;
	php_sundown_render_html_toc_t *html;
	php_sundown_render_base_t *base;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &htext, &htext_len, &header_level) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(htext, htext_len);
	output = bufnew(128);
	html->cb.header(output, input, header_level, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::hrule()
*/
PHP_METHOD(sundown_render_html_toc, hrule)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::listBox($contents, $list_type)
*/
PHP_METHOD(sundown_render_html_toc, listBox)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::listItem($text, $list_type)
*/
PHP_METHOD(sundown_render_html_toc, listItem)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::paragraph($text)
*/
PHP_METHOD(sundown_render_html_toc, paragraph)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::table($header, $body)
*/
PHP_METHOD(sundown_render_html_toc, table)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::tableRow($content)
*/
PHP_METHOD(sundown_render_html_toc, tableRow)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::tableCell($content, $alignment)
*/
PHP_METHOD(sundown_render_html_toc, tableCell)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::autolink($link, $link_type)
*/
PHP_METHOD(sundown_render_html_toc, autolink)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::codespan($code)
*/
PHP_METHOD(sundown_render_html_toc, codespan)
{
	char *code;
	int code_len;
	struct buf *input, *output;
	php_sundown_render_html_toc_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &code, &code_len) == FAILURE) {
		return;
	}

	input = str2buf(code, code_len);
	output = bufnew(128);
	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	html->cb.codespan(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::doubleEmphasis($text)
*/
PHP_METHOD(sundown_render_html_toc, doubleEmphasis)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_toc_t *html;
	php_sundown_render_base_t *base;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);\
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.double_emphasis(output, input, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::emphasis($text)
*/
PHP_METHOD(sundown_render_html_toc, emphasis)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_toc_t *html;
	php_sundown_render_base_t *base;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.emphasis(output, input, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::image($link, $title, $alt_text)
*/
PHP_METHOD(sundown_render_html_toc, image)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::linebreak()
*/
PHP_METHOD(sundown_render_html_toc, linebreak)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::link($link, $title, $content)
*/
PHP_METHOD(sundown_render_html_toc, link)
{
	char *link, *title, *content;
	int link_len, title_len, content_len;
	struct buf *m_link, *m_title, *m_content, *output;
	php_sundown_render_html_toc_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sss", &link, &link_len, &title, &title_len, &content, &content_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	m_link = str2buf(link, link_len);
	m_title = str2buf(title, title_len);
	m_content = str2buf(content, content_len);
	output = bufnew(128);
	html->cb.link(output, m_link, m_title, m_content, &html->html);
	bufrelease(m_link);
	bufrelease(m_title);
	bufrelease(m_content);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::rawHtml($raw_html)
*/
PHP_METHOD(sundown_render_html_toc, rawHtml)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::tripleEmphasis($text)
*/
PHP_METHOD(sundown_render_html_toc, tripleEmphasis)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_toc_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	
	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.triple_emphasis(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::strikethrough($text)
*/
PHP_METHOD(sundown_render_html_toc, strikethrough)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_toc_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.strikethrough(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::superscript($text)
*/
PHP_METHOD(sundown_render_html_toc, superscript)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_toc_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.superscript(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::entity($text)
*/
PHP_METHOD(sundown_render_html_toc, entity)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::normalText($text)
*/
PHP_METHOD(sundown_render_html_toc, normalText)
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

/* {{{ proto string Sundown\Render\HTML_TOC::docHeader()
*/
PHP_METHOD(sundown_render_html_toc, docHeader)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::docFooter()
*/
PHP_METHOD(sundown_render_html_toc, docFooter)
{
	struct buf *output;
	php_sundown_render_html_toc_t *html;

	html = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	output = bufnew(128);
	html->cb.doc_footer(output, &html->html);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML_TOC::__construct($render_flags)
*/
PHP_METHOD(sundown_render_html_toc, __construct)
{
	php_sundown_render_html_toc_t *object;
	struct php_sundown_renderopt_ex opt;
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

	object = (php_sundown_render_html_toc_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	sdhtml_toc_renderer(&object->cb, &opt.html);
	opt.self = getThis();
}
/* }}} */

static zend_function_entry php_sundown_render_html_toc_methods[] = {
	PHP_ME(sundown_render_html_toc, __construct,     arginfo_sundown_render_html_toc___construct,     ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(sundown_render_html_toc, blockCode,       arginfo_sundown_render_html_toc_block_code,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, blockQuote,      arginfo_sundown_render_html_toc_block_quote,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, blockHtml,       arginfo_sundown_render_html_toc_block_html,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, header,          arginfo_sundown_render_html_toc_header,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, hrule,           arginfo_sundown_render_html_toc_hrule,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, listBox,         arginfo_sundown_render_html_toc_list_box,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, listItem,        arginfo_sundown_render_html_toc_list_item,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, paragraph,       arginfo_sundown_render_html_toc_paragraph,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, table,           arginfo_sundown_render_html_toc_table,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, tableRow,        arginfo_sundown_render_html_toc_table_row,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, tableCell,       arginfo_sundown_render_html_toc_table_cell,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, autolink,        arginfo_sundown_render_html_toc_autolink,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, codespan,        arginfo_sundown_render_html_toc_codespan,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, doubleEmphasis,  arginfo_sundown_render_html_toc_double_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, emphasis,        arginfo_sundown_render_html_toc_emphasis,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, image,           arginfo_sundown_render_html_toc_image,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, linebreak,       arginfo_sundown_render_html_toc_linebreak,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, link,            arginfo_sundown_render_html_toc_link,            ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, rawHtml,         arginfo_sundown_render_html_toc_raw_html,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, tripleEmphasis,  arginfo_sundown_render_html_toc_triple_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, strikethrough,   arginfo_sundown_render_html_toc_strikethrough,   ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, superscript,     arginfo_sundown_render_html_toc_superscript,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, entity,          arginfo_sundown_render_html_toc_entity,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, normalText,      arginfo_sundown_render_html_toc_normal_text,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, docHeader,       arginfo_sundown_render_html_toc_doc_header,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html_toc, docFooter,       arginfo_sundown_render_html_toc_doc_footer,      ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void php_sundown_render_html_toc_init(TSRMLS_D)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown", "Render"), "HTML_TOC", php_sundown_render_html_toc_methods);
	sundown_render_html_toc_class_entry = zend_register_internal_class_ex(&ce, sundown_render_base_class_entry, NULL TSRMLS_CC);
	sundown_render_html_toc_class_entry->create_object = php_sundown_render_html_toc_new;
	zend_declare_property_null(sundown_render_html_toc_class_entry, "render_flags", sizeof("render_flags")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
}
