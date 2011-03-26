#include "php_redcarpet.h"

PHPAPI zend_class_entry *redcarpet_class_entry;

void php_redcarpet_init(TSRMLS_D);

ZEND_BEGIN_ARG_INFO_EX(arginfo_redcarpet__construct, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, extensions)
ZEND_END_ARG_INFO()


#define REDCARPET_HAS_EXTENSION(name)   Z_BVAL_P(zend_read_property(redcarpet_class_entry,php_obj,name,sizeof(name), 1 TSRMLS_CC))

static void php_redcarpet__setup_render(zval *php_obj, struct mkd_renderer *rnd)
{
	TSRMLS_FETCH();
	unsigned int render_flags = RENDER_EXPAND_TABS;
	unsigned int parser_flags = 0;
    
	/* smart */
	if (REDCARPET_HAS_EXTENSION("smart"))
		render_flags |= RENDER_SMARTYPANTS;

	/* filter_html */
	if (REDCARPET_HAS_EXTENSION("filter_html"))
		render_flags |= RENDER_SKIP_HTML;

	/* no_image */
	if (REDCARPET_HAS_EXTENSION("no_image"))
		render_flags |= RENDER_SKIP_IMAGES;

	/* no_links */
	if (REDCARPET_HAS_EXTENSION("no_links"))
		render_flags |= RENDER_SKIP_LINKS;

	/* filter_style */
	if (REDCARPET_HAS_EXTENSION("filter_style"))
		render_flags |= RENDER_SKIP_STYLE;

	/* autolink */
	if (REDCARPET_HAS_EXTENSION("auto_link"))
		render_flags |= RENDER_AUTOLINK;

	/* safelink */
	if (REDCARPET_HAS_EXTENSION("safelink"))
		render_flags |= RENDER_SAFELINK;
    
    if (REDCARPET_HAS_EXTENSION("strict"))
        parser_flags |= PARSER_STRICT;

	init_renderer(rnd, render_flags, NULL, parser_flags, 16);
}

PHP_METHOD(redcarpet, __construct)
{
	char *buffer;
	int buffer_len = 0;
	zval *extensions;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s|z",&buffer,&buffer_len,&extensions) == FAILURE) {
		return;
	}
	add_property_stringl_ex(getThis(),"data",sizeof("data"),buffer,buffer_len,1 TSRMLS_CC);

    if(!extensions){
	    add_property_zval_ex(getThis(),"extensions",sizeof("extensions"),extensions TSRMLS_CC);
    }
}

PHP_METHOD(redcarpet, to_html)
{
	struct buf input_buf, *output_buf;
	struct mkd_renderer redcarpet_render;
	
	memset(&input_buf, 0x0, sizeof(struct buf));
	char *buffer = Z_STRVAL_P(zend_read_property(redcarpet_class_entry,getThis(),"data",sizeof("data")-1,1 TSRMLS_CC));
	input_buf.data = buffer;
	input_buf.size = strlen(buffer);
	
	output_buf = bufnew(64);
	
	php_redcarpet__setup_render(getThis(), &redcarpet_render);
	markdown(output_buf, &input_buf, &redcarpet_render);
	
	RETVAL_STRINGL(output_buf->data, output_buf->size,1);
	bufrelease(output_buf);
}

PHPAPI function_entry php_redcarpet_methods[] = {
	PHP_ME(redcarpet, __construct, arginfo_redcarpet__construct, ZEND_ACC_PUBLIC)
	PHP_ME(redcarpet, to_html, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(redcarpet) {
	php_redcarpet_init(TSRMLS_C);
	return SUCCESS;
}

PHP_MINFO_FUNCTION(redcarpet)
{
	return SUCCESS;
}

zend_module_entry redcarpet_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"redcarpet",
	NULL,					 /* Functions */
	PHP_MINIT(redcarpet),	/* MINIT */
	NULL,					/* MSHUTDOWN */
	NULL,					/* RINIT */
	NULL,					/* RSHUTDOWN */
	PHP_MINFO(redcarpet),		/* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
	PHP_REDCARPET_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};

void php_redcarpet_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Redcarpet", php_redcarpet_methods);
	redcarpet_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
}


#ifdef COMPILE_DL_REDCARPET
ZEND_GET_MODULE(redcarpet)
#endif
