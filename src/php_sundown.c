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

zend_class_entry *sundown_class_entry, *sundown_render_base_class_entry, *sundown_markdown_class_entry, *sundown_render_html_class_entry;

void php_sundown_init(TSRMLS_D);

typedef enum
{
	SUNDOWN_RENDER_HTML,
	SUNDOWN_RENDER_TOC
} SundownRendererType;

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown__construct, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_ARRAY_INFO(0, extensions, 0)
ZEND_END_ARG_INFO()

struct php_sundown_renderopt {
	struct html_renderopt html;
	zval *self;
};

static inline zval* buf2str(struct buf *text)
{
	zval *str;
	MAKE_STD_ZVAL(str);
	if (!text || !text->size) return NULL;
	ZVAL_STRINGL(str, text->data, text->size,1);
	return str;
}

static int call_user_function_v(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, ...)
{
	va_list ap;
	size_t i;
	int ret;
	zval **params;
	TSRMLS_FETCH();

	if (param_count > 0) {
		params = emalloc(sizeof(zval**) * param_count);
		va_start(ap, param_count);
		for (i=0; i<param_count;i++) {
			params[i] = va_arg(ap, zval*);
		}
		va_end(ap);
	} else {
		params = NULL;
	}
	ret = call_user_function(function_table, object_pp, function_name, retval_ptr, param_count,params TSRMLS_CC);

	if (param_count > 0) {
		for (i=0; i<param_count;i++) {
			if (params[i] != NULL) {
				zval_ptr_dtor(&params[i]);
			}
		}
		efree(params);
	}
	return ret;
}

#define SPAN_CALLBACK(method_name, ...) {\
	TSRMLS_FETCH();\
	struct php_sundown_renderopt *opt = (struct php_sundown_renderopt*)opaque;\
	zval func, *ret;\
	int param_num = 1;\
\
	MAKE_STD_ZVAL(ret);\
	ZVAL_STRING(&func,method_name,1);\
	\
	if(call_user_function_v(NULL,&opt->self,&func,ret,__VA_ARGS__) == FAILURE){\
		fprintf(stderr,"Can't cal method");\
	}\
	if(!Z_BVAL_P(ret)) {\
		zval_ptr_dtor(&ret);\
		zval_dtor(&func);\
		return 0;\
	}\
	bufput(ob, Z_STRVAL_P(ret), strlen(Z_STRVAL_P(ret)));\
	\
	zval_ptr_dtor(&ret);\
	zval_dtor(&func);\
	return 1;\
}


#define BLOCK_CALLBACK(method_name, ...) {\
	TSRMLS_FETCH();\
	struct php_sundown_renderopt *opt = (struct php_sundown_renderopt*)opaque;\
	zval func, *ret;\
	int param_num = 1;\
\
	MAKE_STD_ZVAL(ret);\
	ZVAL_STRING(&func,method_name,1);\
	\
	if(call_user_function_v(NULL,&opt->self,&func,ret,__VA_ARGS__) == FAILURE){\
		fprintf(stderr,"Can't cal method");\
	}\
	if(!Z_BVAL_P(ret)) {\
		zval_ptr_dtor(&ret);\
		zval_dtor(&func);\
		return;\
	}\
	bufput(ob, Z_STRVAL_P(ret), strlen(Z_STRVAL_P(ret)));\
	\
	zval_ptr_dtor(&ret);\
	zval_dtor(&func);\
}

static void rndr_blockcode(struct buf *ob, struct buf *text, struct buf *lang, void *opaque)
{
	BLOCK_CALLBACK("block_code", 2, buf2str(text), buf2str(lang));
}

static void rndr_blockquote(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK("block_quote", 1, buf2str(text));
}

static void rndr_raw_block(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK("block_html", 1, buf2str(text));
}

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque)
{
	BLOCK_CALLBACK("header", 2, buf2str(text), level);
}

static void rndr_hrule(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK("hrule", 0);
}

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque)
{
	BLOCK_CALLBACK("list", 2, buf2str(text),
	(flags & MKD_LIST_ORDERED) ? buf2str("ordered") : buf2str("unordered"));
}

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque)
{
	BLOCK_CALLBACK("list_item", 2, buf2str(text),
	(flags & MKD_LIST_ORDERED) ? buf2str("ordered") : buf2str("unordered"));
}

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK("paragraph", 1, buf2str(text));
}

static void rndr_table(struct buf *ob, struct buf *header, struct buf *body, void *opaque)
{
	BLOCK_CALLBACK("table", 2, buf2str(header), buf2str(body));
}

static void rndr_tablerow(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK("table_row", 1, buf2str(text));
}

static void rndr_tablecell(struct buf *ob, struct buf *text, int align, void *opaque)
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

	BLOCK_CALLBACK("table_cell", 2, buf2str(text), &php_align);
}

/***
* SPAN LEVEL
*/
static int rndr_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type, void *opaque)
{
	zval url,email;
	ZVAL_STRING(&url,"url",1);
	ZVAL_STRING(&email,"email",1);
	SPAN_CALLBACK("autolink", 2, buf2str(link),
	type == MKDA_NORMAL ? url : email);
	zval_ptr_dtor(&url);
	zval_ptr_dtor(&email);
}

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK("codespan", 1, buf2str(text));
}

static int rndr_double_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK("double_emphasis", 1, buf2str(text));
}

static int rndr_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK("emphasis", 1, buf2str(text));
}

static int rndr_image(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt, void *opaque)
{
	SPAN_CALLBACK("image", 3, buf2str(link), buf2str(title), buf2str(alt));
}

static int rndr_linebreak(struct buf *ob, void *opaque)
{
	SPAN_CALLBACK("linebreak", 0);
}

static int rndr_link(struct buf *ob, struct buf *link, struct buf *title, struct buf *content, void *opaque)
{
	SPAN_CALLBACK("link", 3, buf2str(link), buf2str(title), buf2str(content));
}

static int rndr_raw_html(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK("raw_html", 1, buf2str(text));
}

static int rndr_triple_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK("triple_emphasis", 1, buf2str(text));
}

static int rndr_strikethrough(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK("strikethrough", 1, buf2str(text));
}

static int rndr_superscript(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK("superscript", 1, buf2str(text));
}

/**
* direct writes
*/
static void rndr_entity(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK("entity", 1, buf2str(text));
}

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK("normal_text", 1, buf2str(text));
}

static void rndr_doc_header(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK("doc_header", 0);
}

static void rndr_doc_footer(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK("doc_footer", 0);
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
	"block_code",
	"block_quote",
	"block_html",
	"header",
	"hrule",
	"list_box",
	"list_item",
	"paragraph",
	"table",
	"table_row",
	"table_cell",

	"autolink",
	"codespan",
	"double_emphasis",
	"emphasis",
	"image",
	"linebreak",
	"link",
	"raw_html",
	"triple_emphasis",
	"strikethrough",
	"superscript",

	"entity",
	"normal_text",

	"doc_header",
	"doc_footer"
};

static const size_t php_sundown_method_count = sizeof(php_sundown_method_names)/sizeof(char *);

#define SUNDOWN_HAS_EXTENSION(name)  (table != NULL && zend_hash_exists(table, name,strlen(name)+1) == 1)

static void php_sundown__get_flags(HashTable *table, unsigned int *enabled_extensions_p, unsigned int *render_flags_p)
{
	TSRMLS_FETCH();
	unsigned int render_flags = HTML_EXPAND_TABS;
	unsigned int extensions = 0;

	/* filter_html */
	if (SUNDOWN_HAS_EXTENSION("filter_html")) {
		render_flags |= HTML_SKIP_HTML;
	}

	/* no_image */
	if (SUNDOWN_HAS_EXTENSION("no_image")) {
		render_flags |= HTML_SKIP_IMAGES;
	}

	/* no_links */
	if (SUNDOWN_HAS_EXTENSION("no_links")) {
		render_flags |= HTML_SKIP_LINKS;
	}

	/* filter_style */
	if (SUNDOWN_HAS_EXTENSION("filter_styles")) {
		render_flags |= HTML_SKIP_STYLE;
	}

	/* safelink */
	if (SUNDOWN_HAS_EXTENSION("safelink")) {
		render_flags |= HTML_SAFELINK;
	}

	if (SUNDOWN_HAS_EXTENSION("generate_toc")) {
		render_flags |= HTML_TOC;
	}

	if (SUNDOWN_HAS_EXTENSION("hard_wrap")) {
		render_flags |= HTML_HARD_WRAP;
	}

	if (SUNDOWN_HAS_EXTENSION("gh_blockcode")) {
		render_flags |= HTML_GITHUB_BLOCKCODE;
	}

	if (SUNDOWN_HAS_EXTENSION("xhtml")) {
		render_flags |= HTML_USE_XHTML;
	}

	/**
	 * Markdown extensions -- all disabled by default 
	 */
	if (SUNDOWN_HAS_EXTENSION("autolink")) {
		extensions |= MKDEXT_AUTOLINK;
	}

	if (SUNDOWN_HAS_EXTENSION("no_intraemphasis")) {
		extensions |= MKDEXT_NO_INTRA_EMPHASIS;
	}

	if (SUNDOWN_HAS_EXTENSION("tables")) {
		extensions |= MKDEXT_TABLES;
	}

	if (SUNDOWN_HAS_EXTENSION("fenced_code")) {
		extensions |= MKDEXT_FENCED_CODE;
	}

	if (SUNDOWN_HAS_EXTENSION("strikethrough")) {
		extensions |= MKDEXT_STRIKETHROUGH;
	}

	if (SUNDOWN_HAS_EXTENSION("lax_htmlblock")) {
		extensions |= MKDEXT_LAX_HTML_BLOCKS;
	}

	if (SUNDOWN_HAS_EXTENSION("space_header")) {
		extensions |= MKDEXT_SPACE_HEADERS;
	}

	*enabled_extensions_p = extensions;
	*render_flags_p = render_flags;
}

static void sundown__render(SundownRendererType render_type, INTERNAL_FUNCTION_PARAMETERS)
{
	zval *object;
	struct buf input_buf, *output_buf;
	struct sd_callbacks sundown_render;
	struct php_sundown_renderopt opt;
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

	if(Z_TYPE_P(zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC)) != IS_NULL) {
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

	ce = Z_OBJCE_P(object);
	opt.self = object;
	//overrides
	void **source = (void **)&php_sundown_callbacks;
	void **dest = (void **)&sundown_render;
	size_t i;
	for (i = 0; i < php_sundown_method_count; ++i) {
		if (zend_hash_exists(&ce->function_table, php_sundown_method_names[i], strlen(php_sundown_method_names[i])+1)) {
			dest[i] = source[i];
		}
	}


	sd_markdown(output_buf, &input_buf, enabled_extensions, &sundown_render, &opt);

	if (Z_BVAL_P(zend_read_property(sundown_class_entry, getThis(),"enable_pants",sizeof("enable_pants")-1, 0 TSRMLS_CC))) {
		struct buf *smart_buf = bufnew(128);
		sdhtml_smartypants(smart_buf, output_buf);
		RETVAL_STRINGL(smart_buf->data, smart_buf->size,1);
		bufrelease(smart_buf);
	} else {
		RETVAL_STRINGL(output_buf->data, output_buf->size,1);
	}

	bufrelease(output_buf);
	bufrelease(&input_buf);
}

/* {{{ proto string __construct(string $string [, array $extensions])
	setup Sundown extension */
PHP_METHOD(sundown, __construct)
{
	zval *extensions = NULL;
	char *buffer;
	int buffer_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s|a",&buffer, &buffer_len, &extensions) == FAILURE) {
		return;
	}

	add_property_string_ex(getThis(),"data",sizeof("data"),buffer,1 TSRMLS_CC);

	if (extensions == NULL) {
		MAKE_STD_ZVAL(extensions);
		array_init(extensions);
	}
	add_property_zval_ex(getThis(),"extensions",sizeof("extensions"),extensions TSRMLS_CC);
}
/* }}} */

/* {{{ proto void __destruct()
	cleanup variables */
PHP_METHOD(sundown, __destruct)
{
	zval *extensions;
	
	extensions = zend_read_property(sundown_class_entry, getThis(),"extensions",sizeof("extensions")-1, 0 TSRMLS_CC);
	if(extensions != NULL && Z_TYPE_P(extensions) != IS_NULL) {
		zval_ptr_dtor(&extensions);
	}
}
/* }}} */

/* {{{ proto string to_html()
	Returns converted HTML string */
PHP_METHOD(sundown, to_html)
{
	sundown__render(SUNDOWN_RENDER_HTML,INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string __toString()
	Returns converted HTML string */
PHP_METHOD(sundown, __toString)
{
	sundown__render(SUNDOWN_RENDER_HTML,INTERNAL_FUNCTION_PARAM_PASSTHRU);
}
/* }}} */

/* {{{ proto string to_toc()
	Returns table of contents*/
PHP_METHOD(sundown, to_toc)
{
	sundown__render(SUNDOWN_RENDER_TOC,INTERNAL_FUNCTION_PARAM_PASSTHRU);
}

static zend_function_entry php_sundown_methods[] = {
	PHP_ME(sundown, __construct, arginfo_sundown__construct, ZEND_ACC_PUBLIC)
	PHP_ME(sundown, __destruct,  NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown, to_html, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown, to_toc,  NULL, ZEND_ACC_PUBLIC)
	PHP_ME(sundown, __toString,  NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */


PHP_MINIT_FUNCTION(sundown) {
	php_sundown_init(TSRMLS_C);
	php_sundown_render_base_init(TSRMLS_C);
	php_sundown_render_html_init(TSRMLS_C);
	return SUCCESS;
}


PHP_MINFO_FUNCTION(sundown)
{
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


/* begin Todo: move these declaration to each files */
ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_base_enable_pants, 0, 0, 0)
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

/* {{{ proto void enable_pants()
*/
PHP_METHOD(sundown_render_base, enable_pants)
{
}
/* }}} */

/* {{{ proto void block_code($code, $language)
*/
PHP_METHOD(sundown_render_base, block_code)
{
}
/* }}} */

/* {{{ proto void block_quote($quote)
*/
PHP_METHOD(sundown_render_base, block_quote)
{
}
/* }}} */

/* {{{ proto void block_html($raw_html)
*/
PHP_METHOD(sundown_render_base, block_html)
{
}
/* }}} */

/* {{{ proto void header($htext,$header_level)
*/
PHP_METHOD(sundown_render_base, header)
{
}
/* }}} */

/* {{{ proto void hrule()
*/
PHP_METHOD(sundown_render_base, hrule)
{
}
/* }}} */

/* {{{ proto void list_box($contents, $list_type)
*/
PHP_METHOD(sundown_render_base, list_box)
{
}
/* }}} */

/* {{{ proto void list_item($text, $list_type)
*/
PHP_METHOD(sundown_render_base, list_item)
{
}
/* }}} */

/* {{{ proto void paragraph($text)
*/
PHP_METHOD(sundown_render_base, paragraph)
{
}
/* }}} */

/* {{{ proto void table($header, $body)
*/
PHP_METHOD(sundown_render_base, table)
{
}
/* }}} */

/* {{{ proto void table_row($content)
*/
PHP_METHOD(sundown_render_base, table_row)
{
}
/* }}} */

/* {{{ proto void table_cell($content, $alignment)
*/
PHP_METHOD(sundown_render_base, table_cell)
{
}
/* }}} */

/* {{{ proto void autolink($link, $link_type)
*/
PHP_METHOD(sundown_render_base, autolink)
{
}
/* }}} */

/* {{{ proto void codespan($code)
*/
PHP_METHOD(sundown_render_base, codespan)
{
}
/* }}} */

/* {{{ proto void double_emphasis($text)
*/
PHP_METHOD(sundown_render_base, double_emphasis)
{
}
/* }}} */

/* {{{ proto void emphasis($text)
*/
PHP_METHOD(sundown_render_base, emphasis)
{
}
/* }}} */

/* {{{ proto void image($link, $title, $alt_text)
*/
PHP_METHOD(sundown_render_base, image)
{
}
/* }}} */

/* {{{ proto void linebreak()
*/
PHP_METHOD(sundown_render_base, linebreak)
{
}
/* }}} */

/* {{{ proto void link($link,$title,$content)
*/
PHP_METHOD(sundown_render_base, link)
{
}
/* }}} */

/* {{{ proto void raw_html($raw_html)
*/
PHP_METHOD(sundown_render_base, raw_html)
{
}
/* }}} */

/* {{{ proto void triple_emphasis($text)
*/
PHP_METHOD(sundown_render_base, triple_emphasis)
{
}
/* }}} */

/* {{{ proto void strikethrough($text)
*/
PHP_METHOD(sundown_render_base, strikethrough)
{
}
/* }}} */

/* {{{ proto void superscript($text)
*/
PHP_METHOD(sundown_render_base, superscript)
{
}
/* }}} */

/* {{{ proto void entity($text)
*/
PHP_METHOD(sundown_render_base, entity)
{
}
/* }}} */

/* {{{ proto void normal_text($text)
*/
PHP_METHOD(sundown_render_base, normal_text)
{
}
/* }}} */

/* {{{ proto void doc_header()
*/
PHP_METHOD(sundown_render_base, doc_header)
{
}
/* }}} */

/* {{{ proto void doc_footer()
*/
PHP_METHOD(sundown_render_base, doc_footer)
{
}
/* }}} */

/* {{{ proto void preprocess($full_document)
*/
PHP_METHOD(sundown_render_base, preprocess)
{
}
/* }}} */

/* {{{ proto void postprocess($full_document)
*/
PHP_METHOD(sundown_render_base, postprocess)
{
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
}

static zend_function_entry php_sundown_render_html_methods[] = {
	{NULL,NULL,NULL}
};

void php_sundown_render_html_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown","Render"),"HTML", php_sundown_render_html_methods);
	sundown_render_html_class_entry = zend_register_internal_class_ex(&ce, sundown_render_base_class_entry, NULL TSRMLS_CC);
}

static zend_function_entry php_sundown_markdown_methods[] = {
	{NULL,NULL,NULL}
};

void php_sundown_markdown_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, "Sundown","Markdown", php_sundown_markdown_methods);
	sundown_markdown_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
}

/* end Todo: move these declaration to each files */

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
