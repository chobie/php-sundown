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

extern void php_sundown_render_html_init(TSRMLS_D);
extern void php_sundown_markdown_init(TSRMLS_D);
extern void php_sundown_markdown_init(TSRMLS_D);
extern void php_sundown_buffer_init(TSRMLS_D);

zend_class_entry *sundown_class_entry;

void php_sundown_init(TSRMLS_D);

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown__construct, 0, 0, 2)
	ZEND_ARG_INFO(0, data)
	ZEND_ARG_ARRAY_INFO(0, extensions, 0)
ZEND_END_ARG_INFO()

inline zval* buf2str(struct buf *text)
{
	zval *str;
	MAKE_STD_ZVAL(str);
	if (text == NULL || text->size == 0) {
		ZVAL_NULL(str);
	} else {
		ZVAL_STRINGL(str, text->data, text->size,1);
	}
	return str;
}

inline zval* char2str(char *text)
{
	zval *str;
	MAKE_STD_ZVAL(str);
	ZVAL_STRING(str, text ,1);
	return str;
}

inline zval* buf2long(long value)
{
	zval *data;
	MAKE_STD_ZVAL(data);
	ZVAL_LONG(data,value);
	return data;
}

inline zval* buf2obj(struct buf *text)
{
	TSRMLS_FETCH();
	zval *obj;
	MAKE_STD_ZVAL(obj);

	object_init_ex(obj, sundown_buffer_class_entry);
	php_sundown_buffer_t *object = (php_sundown_buffer_t *) zend_object_store_get_object(obj TSRMLS_CC);
	object->buffer = text;
	
	return obj;
}

struct buf* str2buf(const char *text, size_t length)
{
	struct buf* buffer;
	buffer = bufnew(length);
	bufput(buffer, text, length);
	
	return buffer;
}

int call_user_function_v(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, ...)
{
	va_list ap;
	size_t i;
	int ret;
	zval **params;
	zval *tmp;
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

static void rndr_blockcode(struct buf *ob, struct buf *text, struct buf *lang, void *opaque)
{
	BLOCK_CALLBACK_EX("block_code", 3,buf2obj(ob), buf2str(text), buf2str(lang));
}

static void rndr_blockquote(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX("block_quote", 2,buf2obj(ob), buf2str(text));
}

static void rndr_raw_block(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX("block_html", 2,buf2obj(ob), buf2str(text));
}

static void rndr_header(struct buf *ob, struct buf *text, int level, void *opaque)
{
	BLOCK_CALLBACK_EX("header", 3, buf2obj(ob), buf2str(text), buf2long(level));
}

static void rndr_hrule(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK_EX("hrule", 1,buf2obj(ob));
}

static void rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque)
{
	BLOCK_CALLBACK_EX("list_box", 3,buf2obj(ob), buf2str(text),
	(flags & MKD_LIST_ORDERED) ? char2str("ordered") : char2str("unordered"));
	
}

static void rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque)
{
	BLOCK_CALLBACK_EX("list_item", 3,buf2obj(ob), buf2str(text),
	(flags & MKD_LIST_ORDERED) ? char2str("ordered") : char2str("unordered"));
}

static void rndr_paragraph(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX("paragraph", 2,buf2obj(ob), buf2str(text));
}

static void rndr_table(struct buf *ob, struct buf *header, struct buf *body, void *opaque)
{
	BLOCK_CALLBACK_EX("table", 3,buf2obj(ob), buf2str(header), buf2str(body));
}

static void rndr_tablerow(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX("table_row", 2,buf2obj(ob), buf2str(text));
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

	BLOCK_CALLBACK_EX("table_cell", 3,buf2obj(ob), buf2str(text), &php_align);
}

/***
* SPAN LEVEL
*/
static int rndr_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type, void *opaque)
{
	zval url,email;
	ZVAL_STRING(&url,"url",1);
	ZVAL_STRING(&email,"email",1);
	SPAN_CALLBACK_EX("autolink", 3, buf2obj(ob),buf2str(link),
	type == MKDA_NORMAL ? url : email);
	zval_ptr_dtor(&url);
	zval_ptr_dtor(&email);
}

static int rndr_codespan(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX("codespan", 2,buf2obj(ob), buf2str(text));
}

static int rndr_double_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX("double_emphasis", 2,buf2obj(ob), buf2str(text));
}

static int rndr_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX("emphasis", 2,buf2obj(ob), buf2str(text));
}

static int rndr_image(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt, void *opaque)
{
	SPAN_CALLBACK_EX("image", 4, buf2obj(ob), buf2str(link), buf2str(title), buf2str(alt));
}

static int rndr_linebreak(struct buf *ob, void *opaque)
{
	SPAN_CALLBACK_EX("linebreak", 1,buf2obj(ob));
}

static int rndr_link(struct buf *ob, struct buf *link, struct buf *title, struct buf *content, void *opaque)
{
	SPAN_CALLBACK_EX("link", 4,buf2obj(ob), buf2str(link), buf2str(title), buf2str(content));
}

static int rndr_raw_html(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX("raw_html", 2,buf2obj(ob), buf2str(text));
}

static int rndr_triple_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX("triple_emphasis", 2,buf2obj(ob), buf2str(text));
}

static int rndr_strikethrough(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX("strikethrough", 2, buf2obj(ob), buf2str(text));
}

static int rndr_superscript(struct buf *ob, struct buf *text, void *opaque)
{
	SPAN_CALLBACK_EX("superscript", 2,buf2obj(ob), buf2str(text));
}

/**
* direct writes
*/
static void rndr_entity(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX("entity", 2, buf2obj(ob), buf2str(text));
}

static void rndr_normal_text(struct buf *ob, struct buf *text, void *opaque)
{
	BLOCK_CALLBACK_EX("normal_text", 2, buf2obj(ob), buf2str(text));
}

static void rndr_doc_header(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK_EX("doc_header", 1, buf2obj(ob));
}

static void rndr_doc_footer(struct buf *ob, void *opaque)
{
	BLOCK_CALLBACK_EX("doc_footer", 1, buf2obj(ob));
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

void php_sundown__get_flags(HashTable *table, unsigned int *enabled_extensions_p, unsigned int *render_flags_p)
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

	markdown = sd_markdown_new(0, 16, &sundown_render, &opt);
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
	php_sundown_buffer_init(TSRMLS_C);
	php_sundown_render_base_init(TSRMLS_C);
	php_sundown_render_html_init(TSRMLS_C);
	php_sundown_markdown_init(TSRMLS_C);
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
