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

static void php_sundown_render_html_free_storage(php_sundown_render_html_t *obj TSRMLS_DC)
{
	zend_object_std_dtor(&obj->zo TSRMLS_CC);
	efree(obj);
}

zend_object_value php_sundown_render_html_new(zend_class_entry *ce TSRMLS_DC)
{
	zend_object_value retval;
	php_sundown_render_html_t *obj;
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
		(zend_objects_free_object_storage_t)php_sundown_render_html_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}

zend_class_entry *sundown_render_html_class_entry;
extern zend_class_entry *sundown_render_base_class_entry;

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, render_flags)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_block_code, 0, 0, 2)
	ZEND_ARG_INFO(0, language)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_block_quote, 0, 0, 1)
	ZEND_ARG_INFO(0, quote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_block_html, 0, 0, 1)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_header, 0, 0, 2)
	ZEND_ARG_INFO(0, htext)
	ZEND_ARG_INFO(0, header_level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_hrule, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_list_box, 0, 0, 2)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_list_item, 0, 0, 2)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_paragraph, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_table, 0, 0, 2)
	ZEND_ARG_INFO(0, header)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_table_row, 0, 0, 1)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_table_cell, 0, 0, 2)
	ZEND_ARG_INFO(0, content)
	ZEND_ARG_INFO(0, alignment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_autolink, 0, 0, 2)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, link_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_codespan, 0, 0, 1)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_double_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_image, 0, 0, 3)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, alt_text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_linebreak, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_link, 0, 0, 3)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_raw_html, 0, 0, 1)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_triple_emphasis, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_strikethrough, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_superscript, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_entity, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_normal_text, 0, 0, 1)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_doc_header, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_doc_footer, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto stirng Sundown\Render\HTML::blockCode($language, $code)
*/
PHP_METHOD(sundown_render_html, blockCode)
{
	char *code, *block_code;
	int code_len, block_code_len;
	struct buf *input, *lang, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss", &code, &code_len, &block_code, &block_code_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);\
	input = str2buf(block_code, block_code_len);
	lang = str2buf(code, code_len);
	output = bufnew(128);
	html->cb.blockcode(output, lang, input, &base->html);
	bufrelease(input);
	bufrelease(lang);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::blockQuote($quote)
*/
PHP_METHOD(sundown_render_html, blockQuote)
{
	char *quote;
	int quote_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &quote, &quote_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(quote, quote_len);
	output = bufnew(128);
	html->cb.blockquote(output, input, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::blockHtml()
*/
PHP_METHOD(sundown_render_html, blockHtml)
{
	char *raw;
	int raw_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &raw, &raw_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);\
	input = str2buf(raw, raw_len);
	output = bufnew(128);
	html->cb.blockhtml(output, input, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::header($htext, $header_level)
*/
PHP_METHOD(sundown_render_html, header)
{
	char *htext;
	int htext_len;
	long header_level;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &htext, &htext_len, &header_level) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(htext, htext_len);
        output = bufnew(128);
	html->cb.header(output, input, header_level, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::hrule()
*/
PHP_METHOD(sundown_render_html, hrule)
{
	php_sundown_render_html_t *html;
	struct buf *output;

	output = bufnew(128);
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	html->cb.hrule(output, &html->html);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);

}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::listBox($contents, $list_type)
*/
PHP_METHOD(sundown_render_html, listBox)
{
	char *contents;
	int contents_len;
	long list_type;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &contents, &contents_len, &list_type) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);\
	input = str2buf(contents, contents_len);
	output = bufnew(128);
	html->cb.list(output, input, list_type, &base->html);
	if (input != NULL) {
		bufrelease(input);
	}
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::listItem($text, $list_type)
*/
PHP_METHOD(sundown_render_html, listItem)
{
	char *text;
	int text_len;
	long list_type;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &text, &text_len, &list_type) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.listitem(output, input, list_type, &base->html);
	if (input != NULL) {
		bufrelease(input);
	}

	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::paragraph($text)
*/
PHP_METHOD(sundown_render_html, paragraph)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.paragraph(output, input, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::table($header, $body)
*/
PHP_METHOD(sundown_render_html, table)
{
	char *header, *body;
	int header_len, body_len;
	struct buf *b_header, *b_body, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"ss", &header, &header_len, &body, &body_len) == FAILURE) {
		return;
	}
	
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	b_header = str2buf(header, header_len);
	b_body = str2buf(body, body_len);
	output = bufnew(128);
	html->cb.table(output, b_header, b_body, &base->html);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(b_header);
	bufrelease(b_body);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::tableRow($content)
*/
PHP_METHOD(sundown_render_html, tableRow)
{
	char *contents;
	int contents_len;
	struct buf *output, *input;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &contents, &contents_len) == FAILURE) {
		return;
	}
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(contents, contents_len);
	output = bufnew(128);
	html->cb.table_row(output, input, &base->html);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(input);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::tableCell($content, $alignment)
*/
PHP_METHOD(sundown_render_html, tableCell)
{
	char *content;
	int content_len;
	long alignment;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &content, &content_len, &alignment) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(content, content_len);
	output = bufnew(128);
	html->cb.table_cell(output, input, alignment, &base->html);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(input);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::autolink($link, $link_type)
*/
PHP_METHOD(sundown_render_html, autolink)
{
	char *link;
	int link_len = 0;
	long link_type;
	struct buf *m_link, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sl", &link, &link_len, &link_type) == FAILURE) {
		return;
	}
	
	m_link = str2buf(link, link_len);
	output = bufnew(128);
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	html->cb.autolink(output, m_link, link_type, &html->html);
	bufrelease(m_link);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);

}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::codespan($code)
*/
PHP_METHOD(sundown_render_html, codespan)
{
	char *code;
	int code_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &code, &code_len) == FAILURE) {
		return;
	}

	input = str2buf(code, code_len);
	output = bufnew(128);
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	html->cb.codespan(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::doubleEmphasis($text)
*/
PHP_METHOD(sundown_render_html, doubleEmphasis)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);\
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.double_emphasis(output, input, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::emphasis($text)
*/
PHP_METHOD(sundown_render_html, emphasis)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.emphasis(output, input, &base->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::image($link, $title, $alt_text)
*/
PHP_METHOD(sundown_render_html, image)
{
	char *link, *title, *alt_text;
	int link_len, title_len, alt_text_len;
	struct buf *b_link, *b_title, *b_alt_text, *output;
	php_sundown_render_html_t *html;
	php_sundown_render_base_t *base;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sss", &link, &link_len, &title, &title_len, &alt_text, &alt_text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	b_link = str2buf(link, link_len);
	b_title = str2buf(title, title_len);
	b_alt_text = str2buf(alt_text, alt_text_len);
	output = bufnew(128);
	html->cb.image(output, b_link, b_title, b_alt_text, &base->html);
	bufrelease(b_link);
	bufrelease(b_title);
	bufrelease(b_alt_text);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::linebreak()
*/
PHP_METHOD(sundown_render_html, linebreak)
{
	php_sundown_render_html_t *html;
	struct buf *output;

	output = bufnew(128);
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	html->cb.linebreak(output, &html->html);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::link($link, $title, $content)
*/
PHP_METHOD(sundown_render_html, link)
{
	char *link, *title, *content;
	int link_len, title_len, content_len;
	struct buf *m_link, *m_title, *m_content, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"sss", &link, &link_len, &title, &title_len, &content, &content_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
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

/* {{{ proto string Sundown\Render\HTML::rawHtml($raw_html)
*/
PHP_METHOD(sundown_render_html, rawHtml)
{
	char *raw;
	int raw_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &raw, &raw_len) == FAILURE) {
		return;
	}

	input = str2buf(raw, raw_len);
	output = bufnew(128);
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	html->cb.raw_html_tag(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::tripleEmphasis($text)
*/
PHP_METHOD(sundown_render_html, tripleEmphasis)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.triple_emphasis(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::strikethrough($text)
*/
PHP_METHOD(sundown_render_html, strikethrough)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.strikethrough(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::superscript($text)
*/
PHP_METHOD(sundown_render_html, superscript)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.superscript(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::entity($text)
*/
PHP_METHOD(sundown_render_html, entity)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}
	
	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	if (html->cb.entity) {
		input = str2buf(text, text_len);
		output = bufnew(128);
		html->cb.entity(output, input, &html->html);
		bufrelease(input);
		RETVAL_STRINGL((char *)output->data, output->size, 1);
		bufrelease(output);
	} else {
		RETVAL_STRINGL((char *)text, text_len, 1);
	}
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::normalText($text)
*/
PHP_METHOD(sundown_render_html, normalText)
{
	char *text;
	int text_len;
	struct buf *input, *output;
	php_sundown_render_html_t *html;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &text, &text_len) == FAILURE) {
		return;
	}

	html = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	input = str2buf(text, text_len);
	output = bufnew(128);
	html->cb.normal_text(output, input, &html->html);
	bufrelease(input);
	RETVAL_STRINGL((char *)output->data, output->size, 1);
	bufrelease(output);
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::docHeader()
*/
PHP_METHOD(sundown_render_html, docHeader)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::docFooter()
*/
PHP_METHOD(sundown_render_html, docFooter)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto string Sundown\Render\HTML::__construct($render_flags)
*/
PHP_METHOD(sundown_render_html, __construct)
{
	php_sundown_render_html_t *object;
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

	object = (php_sundown_render_html_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	sdhtml_renderer(&object->cb, &opt.html, 0);
	opt.self = getThis();
}
/* }}} */

static zend_function_entry php_sundown_render_html_methods[] = {
	PHP_ME(sundown_render_html, __construct,     arginfo_sundown_render_html___construct,     ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(sundown_render_html, blockCode,       arginfo_sundown_render_html_block_code,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, blockQuote,      arginfo_sundown_render_html_block_quote,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, blockHtml,       arginfo_sundown_render_html_block_html,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, header,          arginfo_sundown_render_html_header,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, hrule,           arginfo_sundown_render_html_hrule,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, listBox,         arginfo_sundown_render_html_list_box,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, listItem,        arginfo_sundown_render_html_list_item,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, paragraph,       arginfo_sundown_render_html_paragraph,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, table,           arginfo_sundown_render_html_table,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, tableRow,        arginfo_sundown_render_html_table_row,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, tableCell,       arginfo_sundown_render_html_table_cell,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, autolink,        arginfo_sundown_render_html_autolink,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, codespan,        arginfo_sundown_render_html_codespan,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, doubleEmphasis,  arginfo_sundown_render_html_double_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, emphasis,        arginfo_sundown_render_html_emphasis,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, image,           arginfo_sundown_render_html_image,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, linebreak,       arginfo_sundown_render_html_linebreak,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, link,            arginfo_sundown_render_html_link,            ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, rawHtml,         arginfo_sundown_render_html_raw_html,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, tripleEmphasis,  arginfo_sundown_render_html_triple_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, strikethrough,   arginfo_sundown_render_html_strikethrough,   ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, superscript,     arginfo_sundown_render_html_superscript,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, entity,          arginfo_sundown_render_html_entity,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, normalText,      arginfo_sundown_render_html_normal_text,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, docHeader,       arginfo_sundown_render_html_doc_header,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, docFooter,       arginfo_sundown_render_html_doc_footer,      ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void php_sundown_render_html_init(TSRMLS_D)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown", "Render"), "HTML", php_sundown_render_html_methods);
	sundown_render_html_class_entry = zend_register_internal_class_ex(&ce, sundown_render_base_class_entry, NULL TSRMLS_CC);
	sundown_render_html_class_entry->create_object = php_sundown_render_html_new;
	zend_declare_property_null(sundown_render_html_class_entry, "render_flags", sizeof("render_flags")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
}
