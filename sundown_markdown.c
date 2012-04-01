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

extern zend_class_entry *sundown_render_base_class_entry;

zend_class_entry *sundown_markdown_class_entry;

static void rndr_blockcode(struct buf *ob, const struct buf *text, const struct buf *lang, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"blockCode", 2, buf2str(text), buf2str(lang));
}

static void rndr_blockquote(struct buf *ob, const struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"blockQuote", 1, buf2str(text));
}

static void rndr_raw_block(struct buf *ob, const struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"blockHtml", 1, buf2str(text));
}

static void rndr_header(struct buf *ob, const struct buf *text, int level, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"header", 2, buf2str(text), buf2long(level));
}

static void rndr_hrule(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"hrule", 0);
}

static void rndr_list(struct buf *ob, const struct buf *text, int flags, void *opaque)
{
	zval *flag;
	MAKE_STD_ZVAL(flag);
	ZVAL_LONG(flag, flags);
	BLOCK_CALLBACK_EX(ob,"listBox", 2,buf2str(text),flag);
	
}

static void rndr_listitem(struct buf *ob, const struct buf *text, int flags, void *opaque)
{
	zval *flag;
	MAKE_STD_ZVAL(flag);
	ZVAL_LONG(flag, flags);
	BLOCK_CALLBACK_EX(ob,"listItem", 2, buf2str(text),flag);
}

static void rndr_paragraph(struct buf *ob, const struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"paragraph", 1,buf2str(text));
}

static void rndr_table(struct buf *ob, const struct buf *header, const struct buf *body, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"table", 2, buf2str(header), buf2str(body));
}

static void rndr_tablerow(struct buf *ob, const struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob,"tableRow", 1, buf2str(text));
}

static void rndr_tablecell(struct buf *ob, const struct buf *text, int align, void *opaque)
{
	zval php_align;

	switch (align) {
		case MKD_TABLE_ALIGN_L:
			ZVAL_STRING(&php_align, "left", 1);
			break;

		case MKD_TABLE_ALIGN_R:
			ZVAL_STRING(&php_align, "right", 1);
			break;

		case MKD_TABLE_ALIGN_CENTER:
			ZVAL_STRING(&php_align, "center", 1);
			break;

		default:
			ZVAL_NULL(&php_align);
			break;
	}

	BLOCK_CALLBACK_EX(ob,"tableCell", 2,buf2str(text), &php_align);
}

/***
* SPAN LEVEL
*/
static int rndr_autolink(struct buf *ob, const struct buf *link, enum mkd_autolink type, void *opaque)
{
	zval *m_type;
	
	MAKE_STD_ZVAL(m_type);
	if (type == MKDA_NORMAL) {
		ZVAL_STRING(m_type, "url",1);
	} else {
		ZVAL_STRING(m_type, "email",1);
	}
	SPAN_CALLBACK_EX(ob,"autolink", 2,buf2str(link),m_type);
}

static int rndr_codespan(struct buf *ob, const struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob,"codespan", 1, buf2str(text));
}

static int rndr_double_emphasis(struct buf *ob, const struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob,"doubleEmphasis", 1, buf2str(text));
}

static int rndr_emphasis(struct buf *ob, const struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob,"emphasis", 1, buf2str(text));
}

static int rndr_image(struct buf *ob, const struct buf *link, const struct buf *title, const struct buf *alt, void *opaque)
{
	SPAN_CALLBACK_EX(ob,"image", 3, buf2str(link), buf2str(title), buf2str(alt));
}

static int rndr_linebreak(struct buf *ob, void *opaque)
{
	SPAN_CALLBACK_EX(ob,"linebreak", 0);
}

static int rndr_link(struct buf *ob, const struct buf *link, const struct buf *title, const struct buf *content, void *opaque)
{
	SPAN_CALLBACK_EX(ob,"link", 3, buf2str(link), buf2str(title), buf2str(content));
}

static int rndr_raw_html(struct buf *ob, const struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "rawHtml", 1, buf2str(text));
}

static int rndr_triple_emphasis(struct buf *ob, const struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "tripleEmphasis", 1, buf2str(text));
}

static int rndr_strikethrough(struct buf *ob, const struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "strikethrough", 1, buf2str(text));
}

static int rndr_superscript(struct buf *ob, const struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "superscript", 1, buf2str(text));
}

/**
* direct writes
*/
static void rndr_entity(struct buf *ob, const struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "entity", 1, buf2str(text));
}

static void rndr_normal_text(struct buf *ob, const struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "normalText", 1, buf2str(text));
}

static void rndr_doc_header(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "docHeader", 0);
}

static void rndr_doc_footer(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "docFooter", 0);
}


static struct sd_callbacks php_sundown_callbacks = {
	rndr_blockcode,
	rndr_blockquote,
	rndr_raw_block,
	rndr_header,
	rndr_hrule,
	rndr_list,
	rndr_listitem,
	rndr_paragraph,
	rndr_table,
	rndr_tablerow,
	rndr_tablecell,

	rndr_autolink,
	rndr_codespan,
	rndr_double_emphasis,
	rndr_emphasis,
	rndr_image,
	rndr_linebreak,
	rndr_link,
	rndr_raw_html,
	rndr_triple_emphasis,
	rndr_strikethrough,
	rndr_superscript,

	rndr_entity,
	rndr_normal_text,

	rndr_doc_header,
	rndr_doc_footer,
};

static const char *php_sundown_method_names[] = {
	"blockcode",
	"blockquote",
	"blockhtml",
	"header",
	"hrule",
	"listbox",
	"listitem",
	"paragraph",
	"table",
	"tablerow",
	"tablecell",

	"autolink",
	"codespan",
	"doubleemphasis",
	"emphasis",
	"image",
	"linebreak",
	"link",
	"rawhtml",
	"tripleemphasis",
	"strikethrough",
	"superscript",

	"entity",
	"normaltext",

	"docheader",
	"docfooter"
};

static const size_t php_sundown_method_count = sizeof(php_sundown_method_names)/sizeof(char *);

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown__construct, 0, 0, 2)
	ZEND_ARG_INFO(0, render)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown_render, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown_set_extensions, 0, 0, 1)
	ZEND_ARG_INFO(0, extension)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown_has_extension, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown_has_render_flag, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
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
#if ZEND_MODULE_API_NO >= 20100525
	object_properties_init(&(obj->zo), ce); 
#else
	zend_hash_copy(obj->zo.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));
#endif
	retval.handle = zend_objects_store_put(obj, 
		(zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t)php_sundown_markdown_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}


/* {{{ proto string Sundown\Markdown::__construct(Sundown\Render\Base $render [, array $extensions])
*/
PHP_METHOD(sundown_markdown, __construct)
{
	zval *render;
	zval *extensions = NULL, *c_extensions = NULL;
	zend_class_entry **ce;
	php_sundown_markdown_t *object = (php_sundown_markdown_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	
	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC,
		"z|a", &render, &extensions) == FAILURE) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC," Sundown\\Markdown::__construct() expects parameter 2 to be array");
		return;
	}

	if (Z_TYPE_P(render) == IS_STRING) {
		if (zend_lookup_class(Z_STRVAL_P(render), strlen(Z_STRVAL_P(render)), &ce TSRMLS_CC) == FAILURE) {
			zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC,"class %s does not find.", Z_STRVAL_P(render));
		    return;
		} else {
			zval *func,*ret, *obj;
			
			MAKE_STD_ZVAL(func);
			MAKE_STD_ZVAL(ret);
			MAKE_STD_ZVAL(obj);
			ZVAL_STRING(func,"__construct",1);
			object_init_ex(obj, *ce);
			call_user_function(NULL, &obj,func,ret,0,NULL TSRMLS_CC);
			zval_ptr_dtor(&func);
			zval_ptr_dtor(&ret);
			render = obj;
		}
	} else {
		Z_ADDREF_P(render);	
	}

	if (!instanceof_function_ex(Z_OBJCE_P(render), sundown_render_base_class_entry, 0 TSRMLS_CC)) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC,"Render class must extend Sundown\\Render\\Base");
		return;
	}
	object->render = render;

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


/* {{{ proto void Sundown\Markdown::__destruct()
	cleanup variables */
PHP_METHOD(sundown_markdown, __destruct)
{
	zval *extensions;
	
	extensions = zend_read_property(sundown_markdown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC);
	zval_ptr_dtor(&extensions);
}
/* }}} */



/* {{{ proto string Sundown\Markdown::render(string $body)
*/
PHP_METHOD(sundown_markdown, render)
{
	php_sundown_markdown_t *object = (php_sundown_markdown_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	php_sundown_buffer_t *buffer_object_t;
	php_sundown_render_base_t *render_base;
	zval *buffer_object, preprocess, postprocess, *params[1], *ret, *render, *m_retval;
	struct buf input_buf, *output_buf;
	struct sd_callbacks sundown_render;
	struct php_sundown_renderopt_ex opt;
	struct sd_markdown *markdown;
	unsigned int enabled_extensions = 0, render_flags = 0;
	char *buffer;
	int buffer_len = 0;
	zend_class_entry *ce;
	HashTable *table;
	void **source, **dest;
	size_t i;

	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &buffer, &buffer_len) == FAILURE) {
		return;
	}
		
	output_buf = bufnew(128);
	bufgrow(output_buf, buffer_len * 1.2f);
	
	if (zend_call_method_with_0_params(&this_ptr, Z_OBJCE_P(getThis()), NULL, "getrender", &render) == FAILURE) {
		return;
	}

	if (zend_call_method_with_0_params(&render, Z_OBJCE_P(render), NULL, "getrenderflags", &m_retval)) {
		table = Z_ARRVAL_P(m_retval);
		php_sundown__get_render_flags(table, &render_flags);
		table = NULL;
		zval_ptr_dtor(&m_retval);
	}

	if (zend_call_method_with_0_params(&this_ptr, Z_OBJCE_P(getThis()), NULL, "getextensions", &m_retval)) {
		table = Z_ARRVAL_P(m_retval);
		php_sundown__get_extensions(table, &enabled_extensions);
		table = NULL;
		zval_ptr_dtor(&m_retval);
	}
	
	/* @todo: setup render */
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

	render_base = (php_sundown_render_base_t *) zend_object_store_get_object(render TSRMLS_CC);
	render_base->html = opt.html;

	ce = Z_OBJCE_P(render);
	opt.self = render;

	source = (void **)&php_sundown_callbacks;
	dest = (void **)&sundown_render;
	for (i = 0; i < php_sundown_method_count; ++i) {
		if (zend_hash_exists(&ce->function_table, php_sundown_method_names[i], strlen(php_sundown_method_names[i])+1)) {
			dest[i] = source[i];
		}
	}

	/* preprocess */
	MAKE_STD_ZVAL(ret);
	MAKE_STD_ZVAL(params[0]);
	ZVAL_STRINGL(params[0], buffer,strlen(buffer), 1);
	ZVAL_STRING(&preprocess,"preprocess",1);
	if (call_user_function(NULL, &render, &preprocess, ret, 1, params TSRMLS_CC) == FAILURE) {
	}
	
	memset(&input_buf, 0x0, sizeof(struct buf));
	if (ret != NULL && Z_TYPE_P(ret) == IS_STRING) {
		input_buf.data = Z_STRVAL_P(ret);
		input_buf.size = Z_STRLEN_P(ret);
	} else {
		input_buf.data = buffer;
		input_buf.size = strlen(buffer);
	}
	zval_ptr_dtor(&params[0]);

	/* proceess markdown */
	markdown = sd_markdown_new(enabled_extensions, 16, &sundown_render, &opt);
	sd_markdown_render(output_buf, input_buf.data, input_buf.size, markdown);
	sd_markdown_free(markdown);
	
	zval_ptr_dtor(&ret);
	/* postprocess */
	MAKE_STD_ZVAL(ret);
	MAKE_STD_ZVAL(params[0]);
	ZVAL_STRINGL(params[0], output_buf->data,output_buf->size, 1);
	ZVAL_STRING(&postprocess,"postprocess",1);
	if (call_user_function(NULL, &render, &postprocess, ret, 1, params TSRMLS_CC) == FAILURE) {
	}
	
	if (ret != NULL && Z_TYPE_P(ret) == IS_STRING) {
		RETVAL_STRINGL(Z_STRVAL_P(ret), Z_STRLEN_P(ret),1);
	} else {
		RETVAL_STRINGL(output_buf->data,output_buf->size,1);
	}
	zval_ptr_dtor(&ret);
	zval_ptr_dtor(&params[0]);
	zval_dtor(&postprocess);
	zval_dtor(&preprocess);
	zval_ptr_dtor(&render);
}
/* }}} */


/* {{{ proto string Sundown\Markdown::hasExtension(string $ext_name)
*/
PHP_METHOD(sundown_markdown, hasExtension)
{
	char *name;
	int name_len = 0;
	HashTable *table;

	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &name, &name_len) == FAILURE) {
		return;
	}
	
	if (Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
		table = Z_ARRVAL_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC));
		RETVAL_BOOL(php_sundown_has_ext(table, name));
	}
}
/* }}} */

/* {{{ proto string Sundown\Markdown::hasRenderFlag(string $ext_name)
*/
PHP_METHOD(sundown_markdown, hasRenderFlag)
{
	char *name;
	int name_len = 0;
	HashTable *table;
	zval *render_flags,*render = NULL;
	php_sundown_markdown_t *object = (php_sundown_markdown_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &name, &name_len) == FAILURE) {
		return;
	}

	if (zend_call_method_with_0_params(&this_ptr, Z_OBJCE_P(getThis()), NULL, "getrender", &render) == FAILURE) {
		return;
	}

	if (zend_call_method_with_0_params(&render, Z_OBJCE_P(render), NULL, "getrenderflags", &render_flags) == FAILURE) {
		return;
	}

	table = Z_ARRVAL_P(render_flags);
	RETVAL_BOOL(php_sundown_has_ext(table, name));
	zval_ptr_dtor(&render);
	zval_ptr_dtor(&render_flags);
}
/* }}} */


/* {{{ proto void string Sundown\Markdown::setExtension(array $extensions)
*/
PHP_METHOD(sundown_markdown, setExtensions)
{
	zval *tmp, *extensions = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"a", &extensions) == FAILURE) {
		return;
	}

	tmp = zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC);
	zval_ptr_dtor(&tmp);

	add_property_zval_ex(getThis(),"extensions",sizeof("extensions"),extensions TSRMLS_CC);
}
/* }}} */

/* {{{ proto array Sundown\Markdown::getExtensions()
*/
PHP_METHOD(sundown_markdown, getExtensions)
{
	if (Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
		zval *result;

		result = zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC);
		RETVAL_ZVAL(result, 1, 0);
	}
}
/* }}} */

/* {{{ proto Sundown\Render\Base Sundown\Markdown::getRender()
*/
PHP_METHOD(sundown_markdown, getRender)
{
	php_sundown_markdown_t *object = (php_sundown_markdown_t *) zend_object_store_get_object(getThis() TSRMLS_CC);
	RETVAL_ZVAL(object->render, 1, 0);
}
/* }}} */

static zend_function_entry php_sundown_markdown_methods[] = {
	PHP_ME(sundown_markdown, __construct, arginfo_sundown_markdown__construct, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, __destruct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, render, arginfo_sundown_markdown_render, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, getRender, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, getExtensions, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, setExtensions, arginfo_sundown_markdown_set_extensions, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, hasExtension, arginfo_sundown_markdown_has_extension, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, hasRenderFlag, arginfo_sundown_markdown_has_render_flag, ZEND_ACC_PUBLIC)
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
