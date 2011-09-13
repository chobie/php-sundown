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
	zend_hash_copy(obj->zo.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval.handle = zend_objects_store_put(obj, 
		(zend_objects_store_dtor_t)zend_objects_destroy_object,
		(zend_objects_free_object_storage_t)php_sundown_render_html_free_storage,
		NULL TSRMLS_CC);
	retval.handlers = zend_get_std_object_handlers();
	return retval;
}


/********************
 * GENERIC RENDERER *
 ********************/
static int
rndr_autolink(struct buf *ob, struct buf *link, enum mkd_autolink type, void *opaque)
{
	struct html_renderopt *options = opaque;

	if (!link || !link->size)
		return 0;

	if ((options->flags & HTML_SAFELINK) != 0 &&
		!sd_autolink_issafe(link->data, link->size) &&
		type != MKDA_EMAIL)
		return 0;

	BUFPUTSL(ob, "<a href=\"");
	if (type == MKDA_EMAIL)
		BUFPUTSL(ob, "mailto:");
	sdhtml_escape(ob, link->data, link->size);

	if (options->link_attributes) {
		bufputc(ob, '\"');
		options->link_attributes(ob, link, opaque);
		bufputc(ob, '>');
	} else {
		BUFPUTSL(ob, "\">");
	}

	/*
	 * Pretty printing: if we get an email address as
	 * an actual URI, e.g. `mailto:foo@bar.com`, we don't
	 * want to print the `mailto:` prefix
	 */
	if (bufprefix(link, "mailto:") == 0) {
		sdhtml_escape(ob, link->data + 7, link->size - 7);
	} else {
		sdhtml_escape(ob, link->data, link->size);
	}

	BUFPUTSL(ob, "</a>");

	return 1;
}

static void
rndr_blockcode(struct buf *ob, struct buf *text, struct buf *lang, void *opaque)
{
	if (ob->size) bufputc(ob, '\n');

	if (lang && lang->size) {
		size_t i, cls;
		BUFPUTSL(ob, "<pre><code class=\"");

		for (i = 0, cls = 0; i < lang->size; ++i, ++cls) {
			while (i < lang->size && isspace(lang->data[i]))
				i++;

			if (i < lang->size) {
				size_t org = i;
				while (i < lang->size && !isspace(lang->data[i]))
					i++;

				if (lang->data[org] == '.')
					org++;

				if (cls) bufputc(ob, ' ');
				sdhtml_escape(ob, lang->data + org, i - org);
			}
		}

		BUFPUTSL(ob, "\">");
	} else
		BUFPUTSL(ob, "<pre><code>");

	if (text)
		sdhtml_escape(ob, text->data, text->size);

	BUFPUTSL(ob, "</code></pre>\n");
}

/*
 * GitHub style code block:
 *
 *		<pre lang="LANG"><code>
 *		...
 *		</pre></code>
 *
 * Unlike other parsers, we store the language identifier in the <pre>,
 * and don't let the user generate custom classes.
 *
 * The language identifier in the <pre> block gets postprocessed and all
 * the code inside gets syntax highlighted with Pygments. This is much safer
 * than letting the user specify a CSS class for highlighting.
 *
 * Note that we only generate HTML for the first specifier.
 * E.g.
 *		~~~~ {.python .numbered}	=>	<pre lang="python"><code>
 */
static void
rndr_blockcode_github(struct buf *ob, struct buf *text, struct buf *lang, void *opaque)
{
	if (ob->size) bufputc(ob, '\n');

	if (lang && lang->size) {
		size_t i = 0;
		BUFPUTSL(ob, "<pre lang=\"");

		while (i < lang->size && !isspace(lang->data[i]))
			i++;

		if (lang->data[0] == '.')
			sdhtml_escape(ob, lang->data + 1, i - 1);
		else
			sdhtml_escape(ob, lang->data, i);

		BUFPUTSL(ob, "\"><code>");
	} else
		BUFPUTSL(ob, "<pre><code>");

	if (text)
		sdhtml_escape(ob, text->data, text->size);

	BUFPUTSL(ob, "</code></pre>\n");
}

static void
rndr_blockquote(struct buf *ob, struct buf *text, void *opaque)
{
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<blockquote>\n");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</blockquote>\n");
}

static int
rndr_codespan(struct buf *ob, struct buf *text, void *opaque)
{
	BUFPUTSL(ob, "<code>");
	if (text) sdhtml_escape(ob, text->data, text->size);
	BUFPUTSL(ob, "</code>");
	return 1;
}

static int
rndr_strikethrough(struct buf *ob, struct buf *text, void *opaque)
{
	if (!text || !text->size)
		return 0;

	BUFPUTSL(ob, "<del>");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</del>");
	return 1;
}

static int
rndr_double_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	if (!text || !text->size)
		return 0;

	BUFPUTSL(ob, "<strong>");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</strong>");

	return 1;
}

static int
rndr_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "<em>");
	if (text) bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</em>");
	return 1;
}

static int
rndr_linebreak(struct buf *ob, void *opaque)
{
	struct html_renderopt *options = opaque;	
	bufputs(ob, USE_XHTML(options) ? "<br/>\n" : "<br>\n");
	return 1;
}

static void
rndr_header(struct buf *ob, struct buf *text, int level, void *opaque)
{
	struct html_renderopt *options = opaque;
	
	if (ob->size)
		bufputc(ob, '\n');

	if (options->flags & HTML_TOC)
		bufprintf(ob, "<h%d id=\"toc_%d\">", level, options->toc_data.header_count++);
	else
		bufprintf(ob, "<h%d>", level);

	if (text) bufput(ob, text->data, text->size);
	bufprintf(ob, "</h%d>\n", level);
}

static int
rndr_link(struct buf *ob, struct buf *link, struct buf *title, struct buf *content, void *opaque)
{
	struct html_renderopt *options = opaque;
	
	if (link != NULL && (options->flags & HTML_SAFELINK) != 0 && !sd_autolink_issafe(link->data, link->size))
		return 0;

	BUFPUTSL(ob, "<a href=\"");

	if (link && link->size)
		sdhtml_escape(ob, link->data, link->size);

	if (title && title->size) {
		BUFPUTSL(ob, "\" title=\"");
		sdhtml_escape(ob, title->data, title->size);
	}

	if (options->link_attributes) {
		bufputc(ob, '\"');
		options->link_attributes(ob, link, opaque);
		bufputc(ob, '>');
	} else {
		BUFPUTSL(ob, "\">");
	}

	if (content && content->size) bufput(ob, content->data, content->size);
	BUFPUTSL(ob, "</a>");
	return 1;
}

static void
rndr_list(struct buf *ob, struct buf *text, int flags, void *opaque)
{
	if (ob->size) bufputc(ob, '\n');
	bufput(ob, flags & MKD_LIST_ORDERED ? "<ol>\n" : "<ul>\n", 5);
	if (text) bufput(ob, text->data, text->size);
	bufput(ob, flags & MKD_LIST_ORDERED ? "</ol>\n" : "</ul>\n", 6);
}

static void
rndr_listitem(struct buf *ob, struct buf *text, int flags, void *opaque)
{
	BUFPUTSL(ob, "<li>");
	if (text) {
		while (text->size && text->data[text->size - 1] == '\n')
			text->size -= 1;
		bufput(ob, text->data, text->size); }
	BUFPUTSL(ob, "</li>\n");
}

static void
rndr_paragraph(struct buf *ob, struct buf *text, void *opaque)
{
	struct html_renderopt *options = opaque;
	size_t i = 0;

	if (ob->size) bufputc(ob, '\n');

	if (!text || !text->size)
		return;

	while (i < text->size && isspace(text->data[i])) i++;

	if (i == text->size)
		return;

	BUFPUTSL(ob, "<p>");
	if (options->flags & HTML_HARD_WRAP) {
		size_t org;
		while (i < text->size) {
			org = i;
			while (i < text->size && text->data[i] != '\n')
				i++;

			if (i > org)
				bufput(ob, text->data + org, i - org);

			/*
			 * do not insert a line break if this newline
			 * is the last character on the paragraph
			 */
			if (i >= text->size - 1)
				break;
			
			rndr_linebreak(ob, opaque);
			i++;
		}
	} else {
		bufput(ob, &text->data[i], text->size - i);
	}
	BUFPUTSL(ob, "</p>\n");
}

static void
rndr_raw_block(struct buf *ob, struct buf *text, void *opaque)
{
	size_t org, sz;
	if (!text) return;
	sz = text->size;
	while (sz > 0 && text->data[sz - 1] == '\n') sz -= 1;
	org = 0;
	while (org < sz && text->data[org] == '\n') org += 1;
	if (org >= sz) return;
	if (ob->size) bufputc(ob, '\n');
	bufput(ob, text->data + org, sz - org);
	bufputc(ob, '\n');
}

static int
rndr_triple_emphasis(struct buf *ob, struct buf *text, void *opaque)
{
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "<strong><em>");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</em></strong>");
	return 1;
}

static void
rndr_hrule(struct buf *ob, void *opaque)
{
	struct html_renderopt *options = opaque;	
	if (ob->size) bufputc(ob, '\n');
	bufputs(ob, USE_XHTML(options) ? "<hr/>\n" : "<hr>\n");
}

static int
rndr_image(struct buf *ob, struct buf *link, struct buf *title, struct buf *alt, void *opaque)
{
	struct html_renderopt *options = opaque;	
	if (!link || !link->size) return 0;
	BUFPUTSL(ob, "<img src=\"");
	sdhtml_escape(ob, link->data, link->size);
	BUFPUTSL(ob, "\" alt=\"");
	if (alt && alt->size)
		sdhtml_escape(ob, alt->data, alt->size);
	if (title && title->size) {
		BUFPUTSL(ob, "\" title=\"");
		sdhtml_escape(ob, title->data, title->size); }

	bufputs(ob, USE_XHTML(options) ? "\"/>" : "\">");
	return 1;
}

static int
rndr_raw_html(struct buf *ob, struct buf *text, void *opaque)
{
	struct html_renderopt *options = opaque;	

	if ((options->flags & HTML_SKIP_HTML) != 0)
		return 1;

	if ((options->flags & HTML_SKIP_STYLE) != 0 && sdhtml_tag(text->data, text->size, "style"))
		return 1;

	if ((options->flags & HTML_SKIP_LINKS) != 0 && sdhtml_tag(text->data, text->size, "a"))
		return 1;

	if ((options->flags & HTML_SKIP_IMAGES) != 0 && sdhtml_tag(text->data, text->size, "img"))
		return 1;

	bufput(ob, text->data, text->size);
	return 1;
}

static void
rndr_table(struct buf *ob, struct buf *header, struct buf *body, void *opaque)
{
	if (ob->size) bufputc(ob, '\n');
	BUFPUTSL(ob, "<table><thead>\n");
	if (header)
		bufput(ob, header->data, header->size);
	BUFPUTSL(ob, "</thead><tbody>\n");
	if (body)
		bufput(ob, body->data, body->size);
	BUFPUTSL(ob, "</tbody></table>\n");
}

static void
rndr_tablerow(struct buf *ob, struct buf *text, void *opaque)
{
	BUFPUTSL(ob, "<tr>\n");
	if (text)
		bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</tr>\n");
}

static void
rndr_tablecell(struct buf *ob, struct buf *text, int flags, void *opaque)
{
	if (flags & MKD_TABLE_HEADER) {
		BUFPUTSL(ob, "<th");
	} else {
		BUFPUTSL(ob, "<td");
	}

	switch (flags & MKD_TABLE_ALIGNMASK) {
	case MKD_TABLE_ALIGN_CENTER:
		BUFPUTSL(ob, " align=\"center\">");
		break;

	case MKD_TABLE_ALIGN_L:
		BUFPUTSL(ob, " align=\"left\">");
		break;

	case MKD_TABLE_ALIGN_R:
		BUFPUTSL(ob, " align=\"right\">");
		break;

	default:
		BUFPUTSL(ob, ">");
	}

	if (text)
		bufput(ob, text->data, text->size);

	if (flags & MKD_TABLE_HEADER) {
		BUFPUTSL(ob, "</th>\n");
	} else {
		BUFPUTSL(ob, "</td>\n");
	}
}

static int
rndr_superscript(struct buf *ob, struct buf *text, void *opaque)
{
	if (!text || !text->size) return 0;
	BUFPUTSL(ob, "<sup>");
	bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</sup>");
	return 1;
}

static void
rndr_normal_text(struct buf *ob, struct buf *text, void *opaque)
{
	if (text)
		sdhtml_escape(ob, text->data, text->size);
}

static void
toc_header(struct buf *ob, struct buf *text, int level, void *opaque)
{
	struct html_renderopt *options = opaque;

	while (level > options->toc_data.current_level) {
		if (options->toc_data.current_level > 0)
			BUFPUTSL(ob, "<li>");
		BUFPUTSL(ob, "<ul>\n");
		options->toc_data.current_level++;
	}

	while (level < options->toc_data.current_level) {
		BUFPUTSL(ob, "</ul>");
		if (options->toc_data.current_level > 1)
			BUFPUTSL(ob, "</li>\n");
		options->toc_data.current_level--;
	}

	bufprintf(ob, "<li><a href=\"#toc_%d\">", options->toc_data.header_count++);
	if (text)
		bufput(ob, text->data, text->size);
	BUFPUTSL(ob, "</a></li>\n");
}

static void
toc_finalize(struct buf *ob, void *opaque)
{
	struct html_renderopt *options = opaque;

	while (options->toc_data.current_level > 1) {
		BUFPUTSL(ob, "</ul></li>\n");
		options->toc_data.current_level--;
	}

	if (options->toc_data.current_level)
		BUFPUTSL(ob, "</ul>\n");
}


zend_class_entry *sundown_render_html_class_entry;
extern zend_class_entry *sundown_render_base_class_entry;

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_enable_pants, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_block_code, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, code)
	ZEND_ARG_INFO(0, language)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_block_quote, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, quote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_block_html, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_header, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, htext)
	ZEND_ARG_INFO(0, header_level)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_hrule, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_list_box, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, contents)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_list_item, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
	ZEND_ARG_INFO(0, list_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_paragraph, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_table, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, header)
	ZEND_ARG_INFO(0, body)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_table_row, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_table_cell, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, content)
	ZEND_ARG_INFO(0, alignment)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_autolink, 0, 0, 3)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, link_type)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_codespan, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, code)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_double_emphasis, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_emphasis, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_image, 0, 0, 4)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, alt_text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_linebreak, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_link, 0, 0, 4)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, link)
	ZEND_ARG_INFO(0, title)
	ZEND_ARG_INFO(0, content)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_raw_html, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, raw_html)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_triple_emphasis, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_strikethrough, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_superscript, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_entity, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_normal_text, 0, 0, 2)
	ZEND_ARG_INFO(0, buffer)
	ZEND_ARG_INFO(0, text)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_doc_header, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_doc_footer, 0, 0, 1)
	ZEND_ARG_INFO(0, buffer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_preprocess, 0, 0, 1)
	ZEND_ARG_INFO(0, full_document)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_sundown_render_html_postprocess, 0, 0, 1)
	ZEND_ARG_INFO(0, full_document)
ZEND_END_ARG_INFO()

/* {{{ proto void enable_pants()
*/
PHP_METHOD(sundown_render_html, enable_pants)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void block_code($code, $language)
*/
PHP_METHOD(sundown_render_html, block_code)
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

/* {{{ proto void block_quote($quote)
*/
PHP_METHOD(sundown_render_html, block_quote)
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

/* {{{ proto void block_html($raw_html)
*/
PHP_METHOD(sundown_render_html, block_html)
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

/* {{{ proto void header($htext,$header_level)
*/
PHP_METHOD(sundown_render_html, header)
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
PHP_METHOD(sundown_render_html, hrule)
{
}
/* }}} */

/* {{{ proto void list_box($contents, $list_type)
*/
PHP_METHOD(sundown_render_html, list_box)
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

/* {{{ proto void list_item($text, $list_type)
*/
PHP_METHOD(sundown_render_html, list_item)
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

/* {{{ proto void paragraph($text)
*/
PHP_METHOD(sundown_render_html, paragraph)
{
	char *text;
	int text_len;
	zval *buffer;
	struct buf *input;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zs", &buffer,&text, &text_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	php_sundown_render_base_t *base = (php_sundown_render_base_t *) zend_object_store_get_object(getThis() TSRMLS_CC);\
	input = str2buf(text, text_len);
	rndr_paragraph(object->buffer,input, &base->html);
	bufrelease(input);
}
/* }}} */

/* {{{ proto void table($header, $body)
*/
PHP_METHOD(sundown_render_html, table)
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

/* {{{ proto void table_row($content)
*/
PHP_METHOD(sundown_render_html, table_row)
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

/* {{{ proto void table_cell($content, $alignment)
*/
PHP_METHOD(sundown_render_html, table_cell)
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

/* {{{ proto void autolink($link, $link_type)
*/
PHP_METHOD(sundown_render_html, autolink)
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

/* {{{ proto void codespan($code)
*/
PHP_METHOD(sundown_render_html, codespan)
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

/* {{{ proto void double_emphasis($text)
*/
PHP_METHOD(sundown_render_html, double_emphasis)
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

/* {{{ proto void emphasis($text)
*/
PHP_METHOD(sundown_render_html, emphasis)
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

/* {{{ proto void image($link, $title, $alt_text)
*/
PHP_METHOD(sundown_render_html, image)
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

/* {{{ proto void linebreak()
*/
PHP_METHOD(sundown_render_html, linebreak)
{
}
/* }}} */

/* {{{ proto void link($link,$title,$content)
*/
PHP_METHOD(sundown_render_html, link)
{
	char *link, *title, *content;
	int link_len, title_len, content_len;
	zval *buffer;
	php_sundown_buffer_t *object;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"zss", &buffer,&link, &link_len, &title, &title_len, &content, &content_len) == FAILURE){
		return;
	}

	object = (php_sundown_buffer_t *) zend_object_store_get_object(buffer TSRMLS_CC);
	bufput(object->buffer, link, link_len);
}
/* }}} */

/* {{{ proto void raw_html($raw_html)
*/
PHP_METHOD(sundown_render_html, raw_html)
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

/* {{{ proto void triple_emphasis($text)
*/
PHP_METHOD(sundown_render_html, triple_emphasis)
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

/* {{{ proto void strikethrough($text)
*/
PHP_METHOD(sundown_render_html, strikethrough)
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

/* {{{ proto void superscript($text)
*/
PHP_METHOD(sundown_render_html, superscript)
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

/* {{{ proto void entity($text)
*/
PHP_METHOD(sundown_render_html, entity)
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

/* {{{ proto void normal_text($text)
*/
PHP_METHOD(sundown_render_html, normal_text)
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
PHP_METHOD(sundown_render_html, doc_header)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void doc_footer()
*/
PHP_METHOD(sundown_render_html, doc_footer)
{
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void preprocess($full_document)
*/
PHP_METHOD(sundown_render_html, preprocess)
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
PHP_METHOD(sundown_render_html, postprocess)
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
	

static zend_function_entry php_sundown_render_html_methods[] = {
	PHP_ME(sundown_render_html, enable_pants,    arginfo_sundown_render_html_enable_pants,    ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, block_code,      arginfo_sundown_render_html_block_code,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, block_quote,     arginfo_sundown_render_html_block_quote,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, block_html,      arginfo_sundown_render_html_block_html,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, header,          arginfo_sundown_render_html_header,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, hrule,           arginfo_sundown_render_html_hrule,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, list_box,        arginfo_sundown_render_html_list_box,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, list_item,       arginfo_sundown_render_html_list_item,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, paragraph,       arginfo_sundown_render_html_paragraph,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, table,           arginfo_sundown_render_html_table,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, table_row,       arginfo_sundown_render_html_table_row,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, table_cell,      arginfo_sundown_render_html_table_cell,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, autolink,        arginfo_sundown_render_html_autolink,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, codespan,        arginfo_sundown_render_html_codespan,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, double_emphasis, arginfo_sundown_render_html_double_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, emphasis,        arginfo_sundown_render_html_emphasis,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, image,           arginfo_sundown_render_html_image,           ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, linebreak,       arginfo_sundown_render_html_linebreak,       ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, link,            arginfo_sundown_render_html_link,            ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, raw_html,        arginfo_sundown_render_html_raw_html,        ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, triple_emphasis, arginfo_sundown_render_html_triple_emphasis, ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, strikethrough,   arginfo_sundown_render_html_strikethrough,   ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, superscript,     arginfo_sundown_render_html_superscript,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, entity,          arginfo_sundown_render_html_entity,          ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, normal_text,     arginfo_sundown_render_html_normal_text,     ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, doc_header,      arginfo_sundown_render_html_doc_header,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, doc_footer,      arginfo_sundown_render_html_doc_footer,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, preprocess,      arginfo_sundown_render_html_preprocess,      ZEND_ACC_PUBLIC)
	PHP_ME(sundown_render_html, postprocess,     arginfo_sundown_render_html_postprocess,     ZEND_ACC_PUBLIC)
	{NULL,NULL,NULL}
};

void php_sundown_render_html_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_NS_CLASS_ENTRY(ce, ZEND_NS_NAME("Sundown","Render"),"HTML", php_sundown_render_html_methods);
	sundown_render_html_class_entry = zend_register_internal_class_ex(&ce, sundown_render_base_class_entry, NULL TSRMLS_CC);
	sundown_render_html_class_entry->create_object = php_sundown_render_html_new;
}
