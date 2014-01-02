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

extern zend_class_entry *sundown_render_base_class_entry;
static zend_class_entry *spl_ce_InvalidArgumentException;

zend_class_entry *sundown_markdown_class_entry;

static void rndr_blockcode(hoedown_buffer *ob, const hoedown_buffer *text, const hoedown_buffer *lang, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "blockCode", 2, buf2str(text), buf2str(lang));
}

static void rndr_blockquote(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "blockQuote", 1, buf2str(text));
}

static void rndr_raw_block(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "blockHtml", 1, buf2str(text));
}

static void rndr_header(hoedown_buffer *ob, const hoedown_buffer *text, int level, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "header", 2, buf2str(text), buf2long(level));
}

static void rndr_hrule(hoedown_buffer *ob, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "hrule", 0);
}

static void rndr_list(hoedown_buffer *ob, const hoedown_buffer *text, int flags, void *opaque)
{
	zval *flag;
	MAKE_STD_ZVAL(flag);
	ZVAL_LONG(flag, flags);
	BLOCK_CALLBACK_EX(ob, "listBox", 2, buf2str(text), flag);
	
}

static void rndr_listitem(hoedown_buffer *ob, const hoedown_buffer *text, int flags, void *opaque)
{
	zval *flag;
	MAKE_STD_ZVAL(flag);
	ZVAL_LONG(flag, flags);
	BLOCK_CALLBACK_EX(ob, "listItem", 2, buf2str(text), flag);
}

static void rndr_paragraph(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "paragraph", 1, buf2str(text));
}

static void rndr_table(hoedown_buffer *ob, const hoedown_buffer *header, const hoedown_buffer *body, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "table", 2, buf2str(header), buf2str(body));
}

static void rndr_tablerow(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "tableRow", 1, buf2str(text));
}

static void rndr_tablecell(hoedown_buffer *ob, const hoedown_buffer *text, unsigned int num, void *opaque)
{
	zval *php_align;

	MAKE_STD_ZVAL(php_align);
	switch (num) {
		case HOEDOWN_TABLE_ALIGN_L:
			ZVAL_STRING(php_align, "left", 1);
			break;

		case HOEDOWN_TABLE_ALIGN_R:
			ZVAL_STRING(php_align, "right", 1);
			break;

		case HOEDOWN_TABLE_ALIGN_CENTER:
			ZVAL_STRING(php_align, "center", 1);
			break;

		default:
			ZVAL_NULL(php_align);
			break;
	}

	BLOCK_CALLBACK_EX(ob, "tableCell", 2, buf2str(text), php_align);
}

/***
* SPAN LEVEL
*/
static int rndr_autolink(hoedown_buffer *ob, const hoedown_buffer *link, enum hoedown_autolink type, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "autolink", 2, buf2str(link), buf2long(type));
}

static int rndr_codespan(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "codespan", 1, buf2str(text));
}

static int rndr_double_emphasis(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "doubleEmphasis", 1, buf2str(text));
}

static int rndr_emphasis(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "emphasis", 1, buf2str(text));
}

static int rndr_image(hoedown_buffer *ob, const hoedown_buffer *link, const hoedown_buffer *title, const hoedown_buffer *alt, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "image", 3, buf2str(link), buf2str(title), buf2str(alt));
}

static int rndr_linebreak(hoedown_buffer *ob, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "linebreak", 0);
}

static int rndr_link(hoedown_buffer *ob, const hoedown_buffer *link, const hoedown_buffer *title, const hoedown_buffer *content, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "link", 3, buf2str(link), buf2str(title), buf2str(content));
}

static int rndr_raw_html(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "rawHtml", 1, buf2str(text));
}

static int rndr_triple_emphasis(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "tripleEmphasis", 1, buf2str(text));
}

static int rndr_strikethrough(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "strikethrough", 1, buf2str(text));
}

static int rndr_superscript(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	SPAN_CALLBACK_EX(ob, "superscript", 1, buf2str(text));
}

/**
* direct writes
*/
static void rndr_entity(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "entity", 1, buf2str(text));
}

static void rndr_normal_text(hoedown_buffer *ob, const hoedown_buffer *text, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "normalText", 1, buf2str(text));
}

static void rndr_doc_header(hoedown_buffer *ob, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "docHeader", 0);
}

static void rndr_doc_footer(hoedown_buffer *ob, void *opaque)
{
	BLOCK_CALLBACK_EX(ob, "docFooter", 0);
}

static struct hoedown_renderer php_sundown_callbacks = {
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
	NULL, // footnotes
	NULL, // footnote_def

	rndr_autolink,
	rndr_codespan,
	rndr_double_emphasis,
	rndr_emphasis,
	NULL, // underline
	NULL, // highlight
	NULL, // quote
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



static void php_sundown_get_render(zval *instance, zval **result TSRMLS_DC)
{
	zval *render;

	zend_call_method_with_0_params(&instance, Z_OBJCE_P(instance), NULL, "getrender", &render);
	*result = render;
}

static void php_sundown_get_renderflags(zval *instance, unsigned int *result TSRMLS_DC)
{
	unsigned int render_flags = 0;
	zval *flags;

	zend_call_method_with_0_params(&instance, Z_OBJCE_P(instance), NULL, "getrenderflags", &flags);
	php_sundown__get_render_flags(Z_ARRVAL_P(flags), &render_flags);

	*result = render_flags;
	zval_ptr_dtor(&flags);
}

static void php_sundown_get_extensions(zval *instance, unsigned int *result TSRMLS_DC)
{
	unsigned int enabled_extensions = 0;
	zval *flags;

	zend_call_method_with_0_params(&instance, Z_OBJCE_P(instance), NULL, "getextensions", &flags);
	php_sundown__get_extensions(Z_ARRVAL_P(flags), &enabled_extensions);

	*result = enabled_extensions;
	zval_ptr_dtor(&flags);
}

static void php_sundown_markdown_preprocess(zval *instance, zval *render, const char *input, hoedown_buffer *buffer TSRMLS_DC)
{
	zval preprocess, *ret, *params[1];

	MAKE_STD_ZVAL(ret);
	MAKE_STD_ZVAL(params[0]);

	ZVAL_STRINGL(params[0], input, strlen(input), 1);
	ZVAL_STRING(&preprocess, "preprocess", 1);

	if (call_user_function(NULL, &render, &preprocess, ret, 1, params TSRMLS_CC) == FAILURE) {
	}

	if (ret != NULL && Z_TYPE_P(ret) == IS_STRING) {
        buffer->data = emalloc(Z_STRLEN_P(ret));
		memcpy(buffer->data, Z_STRVAL_P(ret), Z_STRLEN_P(ret));
		buffer->size = Z_STRLEN_P(ret);
	} else {
        buffer->data = emalloc(strlen(input));
		memcpy(buffer->data, input, strlen(input));
		buffer->size = strlen(input);
	}

	zval_ptr_dtor(&params[0]);
	zval_ptr_dtor(&ret);
    zval_dtor(&preprocess);
}

static void php_sundown_markdown_postprocess(zval *instance, zval *render, hoedown_buffer *input, zval **result TSRMLS_DC)
{
	zval postprocess, *tmp, *ret, *params[1];

	MAKE_STD_ZVAL(ret);
	MAKE_STD_ZVAL(params[0]);
	MAKE_STD_ZVAL(tmp);

	ZVAL_STRINGL(params[0], (char *)input->data, input->size, 1);
	ZVAL_STRING(&postprocess, "postprocess", 1);

	if (call_user_function(NULL, &render, &postprocess, ret, 1, params TSRMLS_CC) == FAILURE) {
	}

	if (ret != NULL && Z_TYPE_P(ret) == IS_STRING) {
        ZVAL_ZVAL(tmp, ret, 1, 0);
	} else {
        ZVAL_STRINGL(tmp, (char*)input->data, input->size,  1);
	}

	zval_ptr_dtor(&params[0]);
	zval_ptr_dtor(&ret);
	zval_dtor(&postprocess);

	*result = tmp;
}

static void php_sundown_markdown_inherit_functions(zval *render, int render_flags, struct php_sundown_renderopt_ex *opt, hoedown_renderer *sundown_render TSRMLS_DC)
{
	php_sundown_render_base_t *render_base;
	zend_class_entry *ce;
	void **source, **dest;
	int i;

	render_base = (php_sundown_render_base_t *) zend_object_store_get_object(render TSRMLS_CC);
	//render_base->html = opt->html;
	ce = Z_OBJCE_P(render);

	opt->self = render;

	source = (void **)&php_sundown_callbacks;
	dest = (void **)sundown_render;
	for (i = 0; i < php_sundown_method_count; ++i) {
		if (zend_hash_exists(&ce->function_table, php_sundown_method_names[i], strlen(php_sundown_method_names[i])+1)) {
			dest[i] = source[i];
		}
	}

	if (instanceof_function_ex(ce, sundown_render_html_class_entry, 0 TSRMLS_CC)) {
		if (render_flags & HOEDOWN_HTML_SKIP_IMAGES) {
			sundown_render->image = NULL;
		}
		if (render_flags & HOEDOWN_HTML_SKIP_LINKS) {
			sundown_render->link = NULL;
			//sundown_render->_autolink = NULL;
		}
		if (render_flags & HOEDOWN_HTML_SKIP_HTML || render_flags & HOEDOWN_HTML_ESCAPE) {
			sundown_render->blockhtml = NULL;
		}
	}
}

void php_sundown_markdon_render(SundownRendererType render_type, INTERNAL_FUNCTION_PARAMETERS)
{
	zval *render, *result;
	hoedown_buffer input_buf, *output_buf;
	hoedown_renderer *sundown_render;
	struct php_sundown_renderopt_ex opt;
	hoedown_markdown *markdown;
	unsigned int enabled_extensions = 0, render_flags = 0;
	char *buffer;
	int buffer_len = 0;
	int is_sundown_markdown = 0;

	if (Z_OBJCE_P(getThis()) == sundown_markdown_class_entry) {
		is_sundown_markdown = 1;
	}

	if (is_sundown_markdown) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
			"s", &buffer, &buffer_len) == FAILURE) {
			return;
		}
	} else {
		buffer = Z_STRVAL_P(zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("data")-1, 0 TSRMLS_CC));
		buffer_len = strlen(buffer);
		memset(&input_buf, 0x0, sizeof(hoedown_buffer));
		input_buf.data = (uint8_t *)buffer;
		input_buf.size = strlen(buffer);
	}

	output_buf = hoedown_buffer_new(128);
	hoedown_buffer_grow(output_buf, buffer_len * 1.2f);

	if (is_sundown_markdown) {
		php_sundown_get_render(getThis(), &render TSRMLS_CC);
		php_sundown_get_renderflags(render, &render_flags TSRMLS_CC);
		php_sundown_get_extensions(getThis(), &enabled_extensions TSRMLS_CC);
	} else {
		if (Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
			php_sundown__get_flags(Z_ARRVAL_P(zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC)), &enabled_extensions, &render_flags);
		}
	}

	switch (render_type) {
		case SUNDOWN_RENDER_HTML:
			sundown_render = hoedown_html_renderer_new(render_flags, 0);
			break;
		case SUNDOWN_RENDER_TOC:
			sundown_render = hoedown_html_toc_renderer(0, 0);
			break;
		default:
			RETURN_FALSE;
	}

	if (is_sundown_markdown) {
		//php_sundown_markdown_inherit_functions(render, render_flags, &opt, &sundown_render TSRMLS_CC);
		php_sundown_markdown_preprocess(getThis(), render, buffer, &input_buf TSRMLS_CC);
	} else {
		opt.self = getThis();
	}

	if (is_sundown_markdown) {
		markdown = hoedown_markdown_new(enabled_extensions, 16, sundown_render);
		if (SETJMP(SUNDOWN_G(jump)) == 0) {
			hoedown_markdown_render(output_buf, input_buf.data, input_buf.size, markdown);
			efree(input_buf.data);
			hoedown_markdown_free(markdown);
		} else {
			efree(input_buf.data);
			zval_ptr_dtor(&render);
			hoedown_markdown_free(markdown);
			return;
		}
		php_sundown_markdown_postprocess(getThis(), render, output_buf, &result TSRMLS_CC);
		zval_ptr_dtor(&render);
		RETURN_ZVAL(result, 0, 1);
	} else {
		markdown = hoedown_markdown_new(enabled_extensions, 16, sundown_render);
		hoedown_markdown_render(output_buf, input_buf.data, input_buf.size, markdown);
		hoedown_markdown_free(markdown);

		if (Z_BVAL_P(zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("enable_pants")-1, 0 TSRMLS_CC))) {
			hoedown_buffer *smart_buf = hoedown_buffer_new(128);
			hoedown_html_smartypants(smart_buf, output_buf->data, output_buf->size);

			RETVAL_STRINGL((char*)smart_buf->data, smart_buf->size, 1);
			hoedown_buffer_free(smart_buf);
		} else {
			RETVAL_STRINGL((char*)output_buf->data, output_buf->size, 1);
		}
		free(sundown_render);
	}
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown__construct, 0, 0, 2)
	ZEND_ARG_INFO(0, render)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown_render, 0, 0, 1)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_markdown_set_render, 0, 0, 1)
	ZEND_ARG_INFO(0, render)
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

	obj = ecalloc(1, sizeof(*obj));
	zend_object_std_init( &obj->zo, ce TSRMLS_CC );
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
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC, " Sundown\\Markdown::__construct() expects parameter 2 to be array");
		return;
	}

	if (Z_TYPE_P(render) == IS_STRING) {
		zval *obj, *retval;

		if (zend_lookup_class(Z_STRVAL_P(render), strlen(Z_STRVAL_P(render)), &ce TSRMLS_CC) == FAILURE) {
			zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC, "class %s does not find.", Z_STRVAL_P(render));
		    return;
		}

		MAKE_STD_ZVAL(obj);
		object_init_ex(obj, *ce);
		zend_call_method_with_0_params(&obj, Z_OBJCE_P(obj), NULL, "__construct", &retval);
		zval_ptr_dtor(&retval);

		render = obj;
	} else if (Z_TYPE_P(render) == IS_OBJECT) {
		/* nothing to do */
		Z_ADDREF_P(render);
	} else {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC, "passed unexpected value. Sundown\\Markdown::__construct expects string or object");
		return;
	}

	if (!instanceof_function_ex(Z_OBJCE_P(render), sundown_render_base_class_entry, 0 TSRMLS_CC)) {
		Z_DELREF_P(render);
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC, "Render class must extend Sundown\\Render\\Base");
		return;
	}
	object->render = render;

	if (extensions == NULL) {
		MAKE_STD_ZVAL(c_extensions);
		array_init(c_extensions);
	} else {
		ALLOC_INIT_ZVAL(c_extensions);
		ZVAL_ZVAL(c_extensions, extensions, 1, 0);
	}
	add_property_zval_ex(getThis(), ZEND_STRS("extensions"), c_extensions TSRMLS_CC);
}
/* }}} */


/* {{{ proto void Sundown\Markdown::__destruct()
	cleanup variables */
PHP_METHOD(sundown_markdown, __destruct)
{
	zval *extensions;
	
	extensions = zend_read_property(sundown_markdown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC);
	zval_ptr_dtor(&extensions);
}
/* }}} */

/* {{{ proto string Sundown\Markdown::render(string $body)
*/
PHP_METHOD(sundown_markdown, render)
{
    php_sundown_markdon_render(SUNDOWN_RENDER_HTML, INTERNAL_FUNCTION_PARAM_PASSTHRU);
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
	
	if (Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
		table = Z_ARRVAL_P(zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC));
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
	zval *render_flags, *render = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s", &name, &name_len) == FAILURE) {
		return;
	}

	zend_call_method_with_0_params(&this_ptr, Z_OBJCE_P(getThis()), NULL, "getrender", &render);
	zend_call_method_with_0_params(&render, Z_OBJCE_P(render), NULL, "getrenderflags", &render_flags);

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

	tmp = zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC);
	zval_ptr_dtor(&tmp);

	add_property_zval_ex(getThis(), ZEND_STRS("extensions"), extensions TSRMLS_CC);
}
/* }}} */

/* {{{ proto array Sundown\Markdown::getExtensions()
*/
PHP_METHOD(sundown_markdown, getExtensions)
{
	if (Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
		zval *result;

		result = zend_read_property(sundown_class_entry, getThis(), ZEND_STRS("extensions")-1, 0 TSRMLS_CC);
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

/* {{{ proto void string Sundown\Markdown::setRender(Sundown\Render\Base $render)
*/
PHP_METHOD(sundown_markdown, setRender)
{
	zval *render = NULL;
	php_sundown_markdown_t *object = (php_sundown_markdown_t *) zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"z", &render) == FAILURE) {
		return;
	}

	if (!instanceof_function_ex(Z_OBJCE_P(render), sundown_render_base_class_entry, 0 TSRMLS_CC)) {
		zend_throw_exception_ex(spl_ce_InvalidArgumentException, 0 TSRMLS_CC, "Render class must extend Sundown\\Render\\Base");
		return;
	}
	zval_ptr_dtor(&object->render);
	object->render = render;
	Z_ADDREF_P(render);
}
/* }}} */


static zend_function_entry php_sundown_markdown_methods[] = {
	PHP_ME(sundown_markdown, __construct, arginfo_sundown_markdown__construct, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, __destruct, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, render, arginfo_sundown_markdown_render, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, getRender, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, setRender, arginfo_sundown_markdown_set_render, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, getExtensions, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, setExtensions, arginfo_sundown_markdown_set_extensions, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, hasExtension, arginfo_sundown_markdown_has_extension, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_markdown, hasRenderFlag, arginfo_sundown_markdown_has_render_flag, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void php_sundown_markdown_init(TSRMLS_D)
{
	zend_class_entry ce;
	
	INIT_NS_CLASS_ENTRY(ce, "Sundown", "Markdown", php_sundown_markdown_methods);
	sundown_markdown_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
	sundown_markdown_class_entry->create_object = php_sundown_markdown_new;
	zend_declare_property_null(sundown_markdown_class_entry, ZEND_STRS("extensions")-1,  ZEND_ACC_PUBLIC TSRMLS_CC);

	if (!spl_ce_InvalidArgumentException) {
		/* when I'm building this extension on windows box. I can't fix redefintion macro error. for now lookup the class */
		zend_class_entry **pce;
		
		if (zend_hash_find(CG(class_table), ZEND_STRS("invalidargumentexception")-1, (void **)&pce) == SUCCESS) {
			spl_ce_InvalidArgumentException = *pce;
		}
	}
}
