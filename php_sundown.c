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

extern void php_sundown_render_html_init(TSRMLS_D);
extern void php_sundown_render_xhtml_init(TSRMLS_D);
extern void php_sundown_markdown_init(TSRMLS_D);
extern void php_sundown_markdown_init(TSRMLS_D);

zend_class_entry *sundown_class_entry;

void php_sundown_init(TSRMLS_D);

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown__construct, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_ARRAY_INFO(0, extensions, 0)
ZEND_END_ARG_INFO()

static void sundown__render(SundownRendererType render_type, INTERNAL_FUNCTION_PARAMETERS)
{
	zval *object;
	struct buf input_buf, *output_buf;
	struct sd_callbacks sundown_render;
	struct php_sundown_renderopt opt;
	struct sd_markdown *markdown;
	unsigned int enabled_extensions = 0, render_flags = 0;
	char *buffer;
	int buffer_len = 0;
	zend_class_entry *ce;
	HashTable *table;
	
	object = getThis();
	buffer = Z_STRVAL_P(zend_read_property(sundown_class_entry, getThis(),"data",sizeof("data")-1, 0 TSRMLS_CC));
	buffer_len = strlen(buffer);
	
	memset(&input_buf, 0x0, sizeof(struct buf));
	input_buf.data = buffer;
	input_buf.size = strlen(buffer);
	
	output_buf = bufnew(128);
	bufgrow(output_buf, strlen(buffer) * 1.2f);

	if (Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
		table = Z_ARRVAL_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC));
	}
	php_sundown__get_flags(table, &enabled_extensions, &render_flags);

	// setup render
	switch (render_type) {
		case SUNDOWN_RENDER_HTML:
			sdhtml_renderer(&sundown_render, &opt.html, render_flags);
			break;
		case SUNDOWN_RENDER_TOC:
			sdhtml_toc_renderer(&sundown_render,&opt.html);
			break;
		default:
			RETURN_FALSE;
	}
	opt.self = object;

	markdown = sd_markdown_new(enabled_extensions, 16, &sundown_render, &opt);
	sd_markdown_render(output_buf, input_buf.data, input_buf.size, markdown);
	sd_markdown_free(markdown);

	if (Z_BVAL_P(zend_read_property(sundown_class_entry, getThis(),"enable_pants",sizeof("enable_pants")-1, 0 TSRMLS_CC))) {
		struct buf *smart_buf = bufnew(128);
		sdhtml_smartypants(smart_buf, output_buf->data,output_buf->size);
		RETVAL_STRINGL(smart_buf->data, smart_buf->size,1);
		bufrelease(smart_buf);
	} else {
		RETVAL_STRINGL(output_buf->data, output_buf->size,1);
	}
}

/* {{{ proto string Sundonw::__construct(string $string [, array $extensions])
	setup Sundown extension */
PHP_METHOD(sundown, __construct)
{
	zval *extensions = NULL,*c_extensions = NULL;
	char *buffer;
	int buffer_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s|a",&buffer, &buffer_len, &extensions) == FAILURE) {
		return;
	}

	add_property_string_ex(getThis(),"data",sizeof("data"),buffer,1 TSRMLS_CC);

	if (extensions == NULL) {
		MAKE_STD_ZVAL(c_extensions);
		array_init(c_extensions);
	} else {
		ALLOC_INIT_ZVAL(c_extensions);
		ZVAL_ZVAL(c_extensions,extensions,1,0);
	}
	add_property_zval_ex(getThis(),"extensions",sizeof("extensions"),c_extensions TSRMLS_CC);
}
/* }}} */

/* {{{ proto void Sundonw::__destruct()
	cleanup variables */
PHP_METHOD(sundown, __destruct)
{
	zval *extensions;
	
	extensions = zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC);
	zval_ptr_dtor(&extensions);
}
/* }}} */

/* {{{ proto string Sundonw::toHtml()
	Returns converted HTML string */
PHP_METHOD(sundown, toHtml)
{
	sundown__render(SUNDOWN_RENDER_HTML,INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string Sundonw::__toString()
	Returns converted HTML string */
PHP_METHOD(sundown, __toString)
{
	sundown__render(SUNDOWN_RENDER_HTML,INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string Sundonw::toToc()
	Returns table of contents*/
PHP_METHOD(sundown, toToc)
{
	sundown__render(SUNDOWN_RENDER_TOC,INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

static zend_function_entry php_sundown_methods[] = {
	PHP_ME(sundown, __construct, arginfo_sundown__construct, ZEND_ACC_PUBLIC)
	PHP_ME(sundown, __destruct,  NULL,                       ZEND_ACC_PUBLIC)
	PHP_ME(sundown, toHtml,      NULL,                       ZEND_ACC_PUBLIC)
	PHP_ME(sundown, toToc,       NULL,                       ZEND_ACC_PUBLIC)
	PHP_ME(sundown, __toString,  NULL,                       ZEND_ACC_PUBLIC)
	/* to_html and to_toc methods are compatible with Redcarpet */
	PHP_MALIAS(sundown, to_html, toHtml, NULL, ZEND_ACC_PUBLIC)
	PHP_MALIAS(sundown, to_toc,  toToc,  NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */


PHP_MINIT_FUNCTION(sundown) {
	php_sundown_init(TSRMLS_C);
	php_sundown_render_base_init(TSRMLS_C);
	php_sundown_render_html_init(TSRMLS_C);
	php_sundown_render_xhtml_init(TSRMLS_C);
	php_sundown_markdown_init(TSRMLS_C);

	REGISTER_NS_STRING_CONSTANT(ZEND_NS_NAME("Sundown","Render"), "HTML", "Sundown\\Render\\HTML", CONST_CS | CONST_PERSISTENT);
	REGISTER_NS_LONG_CONSTANT(ZEND_NS_NAME("Sundown","Render"), "LIST_ORDERED",    1, CONST_CS | CONST_PERSISTENT);
	return SUCCESS;
}


PHP_MINFO_FUNCTION(sundown)
{
	php_printf("PHP Sundown Extension\n");
	php_info_print_table_start();
	php_info_print_table_row(2,"Sundown Support",  "enabled");
	php_info_print_table_row(2,"Sundown Version", "86a4790c47e1d6e0c71e017d312ff758b610ef69");
	php_info_print_table_end();
}

zend_module_entry sundown_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"sundown",
	NULL,					/* Functions */
	PHP_MINIT(sundown),	/* MINIT */
	NULL,					/* MSHUTDOWN */
	NULL,					/* RINIT */
	NULL,					/* RSHUTDOWN */
	PHP_MINFO(sundown),	/* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
	PHP_SUNDOWN_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};


void php_sundown_init(TSRMLS_D)
{
	zend_class_entry ce;
	
	INIT_CLASS_ENTRY(ce, "Sundown", php_sundown_methods);
	sundown_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	zend_declare_property_null(sundown_class_entry, "extensions", sizeof("extensions")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(sundown_class_entry, "enable_pants", sizeof("enable_pants")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);
}


#ifdef COMPILE_DL_SUNDOWN
ZEND_GET_MODULE(sundown)
#endif
