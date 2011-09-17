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
#include "ext/spl/spl_exceptions.h"

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry sundown_module_entry;
#define phpext_sundown_ptr &sundown_module_entry;

extern int call_user_function_v(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, ...);
extern inline zval* buf2str(const struct buf *text);
extern inline zval* buf2obj(const struct buf *text);
extern inline zval* buf2long(long value);
extern inline zval* char2str(char *text);
extern struct buf* str2buf(const char *text, size_t length);

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

typedef struct{
	zend_object zo;
	struct html_renderopt html;
	struct sd_callbacks cb;
} php_sundown_render_html_t;

typedef struct{
	zend_object zo;
	struct html_renderopt html;
} php_sundown_render_base_t;

#define SPAN_CALLBACK_EX(method_name, ...) {\
	TSRMLS_FETCH();\
	struct php_sundown_renderopt_ex *opt = (struct php_sundown_renderopt_ex*)opaque;\
	zval func, *ret;\
\
	MAKE_STD_ZVAL(ret);\
	ZVAL_STRING(&func,method_name,1);\
	\
	if(call_user_function_v(NULL,&opt->self,&func,ret,__VA_ARGS__) == FAILURE){\
		fprintf(stderr,"Can't call method %s\n", method_name);\
		return 0;\
	}\
	zval_ptr_dtor(&ret);\
	zval_dtor(&func);\
	return Z_LVAL_P(ret);\
}


#define BLOCK_CALLBACK_EX(method_name, ...) {\
	TSRMLS_FETCH();\
	struct php_sundown_renderopt_ex *opt = (struct php_sundown_renderopt_ex*)opaque;\
	zval func, *ret;\
\
	MAKE_STD_ZVAL(ret);\
	ZVAL_STRING(&func,method_name,1);\
	\
	if(call_user_function_v(NULL,&opt->self,&func,ret,__VA_ARGS__) == FAILURE){\
		fprintf(stderr,"Can't call method %s\n", method_name);\
	}\
	zval_ptr_dtor(&ret);\
	zval_dtor(&func);\
}

#endif /* PHP_SUNDOWN_H */