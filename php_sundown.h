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
	struct sd_callbacks cb;
} php_sundown_render_xhtml_t;

typedef struct{
	zend_object zo;
	struct html_renderopt html;
} php_sundown_render_base_t;

#define SPAN_CALLBACK_EX(buffer,method_name, ...) {\
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
	if (ret != NULL) {\
		bufput(buffer, Z_STRVAL_P(ret), Z_STRLEN_P(ret));\
	}\
	zval_ptr_dtor(&ret);\
	zval_dtor(&func);\
	return Z_LVAL_P(ret);\
}


#define BLOCK_CALLBACK_EX(buffer,method_name, ...) {\
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
	if (ret != NULL) {\
		bufput(buffer, Z_STRVAL_P(ret), Z_STRLEN_P(ret));\
	}\
	zval_ptr_dtor(&ret);\
	zval_dtor(&func);\
}

#define SUNDOWN_HAS_EXTENSION(name)  (table != NULL && zend_hash_exists(table, name,strlen(name)+1) == 1)

static int call_user_function_v(HashTable *function_table, zval **object_pp, zval *function_name, zval *retval_ptr, zend_uint param_count, ...)
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

static inline zval* buf2str(const struct buf *text)
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

static inline zval* char2str(char *text)
{
	zval *str;
	
	MAKE_STD_ZVAL(str);
	ZVAL_STRING(str, text ,1);
	return str;
}

static inline zval* buf2long(long value)
{
	zval *data;
	
	MAKE_STD_ZVAL(data);
	ZVAL_LONG(data,value);
	return data;
}

static inline struct buf* str2buf(const char *text, size_t length)
{
	struct buf* buffer;
	
	if (length > 0) {
		buffer = bufnew(length);
		bufput(buffer, text, length);
	} else {
		buffer = NULL;
	}
	
	return buffer;
}


#endif /* PHP_SUNDOWN_H */
