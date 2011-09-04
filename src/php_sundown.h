#ifndef PHP_SUNDOWN_H

#define PHP_SUNDOWN_H

#define PHP_SUNDOWN_EXTNAME "sundown"
#define PHP_SUNDOWN_EXTVER "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "markdown.h"
#include "html.h"

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry sundown_module_entry;
#define phpext_sundown_ptr &sundown_module_entry;

extern int call_user_function_v(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, ...);
extern inline zval* buf2str(struct buf *text);
extern inline zval* buf2obj(struct buf *text);
extern inline zval* buf2long(long value);


extern zend_class_entry *sundown_class_entry, *sundown_buffer_class_entry, *php_sundown_buffer_class_entry;

typedef enum
{
	SUNDOWN_RENDER_HTML,
	SUNDOWN_RENDER_TOC
} SundownRendererType;

struct php_sundown_renderopt {
	struct html_renderopt html;
	zval *self;
};

struct php_sundown_renderopt_ex {
	struct html_renderopt html;
	zval *self;
};


typedef struct{
    zend_object zo;
    zval *render;
} php_sundown_markdown_t;

typedef struct{
    zend_object zo;
    struct buf *buffer;
} php_sundown_buffer_t;

#endif /* PHP_SUNDOWN_H */