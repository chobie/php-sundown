#include "php_phpskirt.h"

PHPAPI zend_class_entry *phpskirt_class_entry;

void php_phpskirt_init(TSRMLS_D);

ZEND_BEGIN_ARG_INFO_EX(arginfo_phpskirt__construct, 0, 0, 1)
	ZEND_ARG_INFO(0, string)
	ZEND_ARG_INFO(0, extensions)
ZEND_END_ARG_INFO()


#define PHPSKIRT_HAS_EXTENSION(name)   Z_BVAL_P(zend_read_property(phpskirt_class_entry,php_obj,name,sizeof(name), 1 TSRMLS_CC))

    static void php_phpskirt__get_flags(zval *php_obj, unsigned int *enabled_extensions_p, unsigned int *render_flags_p)
{
	TSRMLS_FETCH();
	unsigned int render_flags = HTML_EXPAND_TABS;
	unsigned int extensions = 0;
    
	/* smart */
/*
	if (PHPSKIRT_HAS_EXTENSION("smart"))
		render_flags |= RENDER_SMARTYPANTS;
*/

	/* filter_html */
	if (PHPSKIRT_HAS_EXTENSION("filter_html"))
		render_flags |= HTML_SKIP_HTML;

	/* no_image */
	if (PHPSKIRT_HAS_EXTENSION("no_image"))
		render_flags |= HTML_SKIP_IMAGES;

	/* no_links */
	if (PHPSKIRT_HAS_EXTENSION("no_links"))
		render_flags |= HTML_SKIP_LINKS;

	/* filter_style */
	if (PHPSKIRT_HAS_EXTENSION("filter_style"))
		render_flags |= HTML_SKIP_STYLE;

	/* autolink */
	if (PHPSKIRT_HAS_EXTENSION("auto_link"))
		render_flags |= MKDEXT_AUTOLINK;

	/* safelink */
	if (PHPSKIRT_HAS_EXTENSION("safelink"))
		render_flags |= HTML_SAFELINK;


	*enabled_extensions_p = extensions;
	*render_flags_p = render_flags;
}

PHP_METHOD(phpskirt, __construct)
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

PHP_METHOD(phpskirt, to_html)
{
	struct buf input_buf, *output_buf;
	struct mkd_renderer phpskirt_render;
	unsigned int enabled_extensions, render_flags;
	
	memset(&input_buf, 0x0, sizeof(struct buf));
	char *buffer = Z_STRVAL_P(zend_read_property(phpskirt_class_entry,getThis(),"data",sizeof("data")-1,1 TSRMLS_CC));
	input_buf.data = buffer;
	input_buf.size = strlen(buffer);
	
	output_buf = bufnew(128);
	
	php_phpskirt__get_flags(getThis(), &enabled_extensions, &render_flags);
	upshtml_renderer(&phpskirt_render, render_flags);

	ups_markdown(output_buf, &input_buf, &phpskirt_render, enabled_extensions);

	
	RETVAL_STRINGL(output_buf->data, output_buf->size,1);

	bufrelease(output_buf);
	upshtml_free_renderer(&phpskirt_render);
}

PHPAPI function_entry php_phpskirt_methods[] = {
	PHP_ME(phpskirt, __construct, arginfo_phpskirt__construct, ZEND_ACC_PUBLIC)
	PHP_ME(phpskirt, to_html, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};


PHP_MINIT_FUNCTION(phpskirt) {
	php_phpskirt_init(TSRMLS_C);
	return SUCCESS;
}

PHP_MINFO_FUNCTION(phpskirt)
{
	return SUCCESS;
}

zend_module_entry phpskirt_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"phpskirt",
	NULL,				    /* Functions */
	PHP_MINIT(phpskirt),	/* MINIT */
	NULL,					/* MSHUTDOWN */
	NULL,					/* RINIT */
	NULL,					/* RSHUTDOWN */
	PHP_MINFO(phpskirt),	/* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
	PHP_PHPSKIRT_EXTVER,
#endif
	STANDARD_MODULE_PROPERTIES
};

void php_phpskirt_init(TSRMLS_D)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "Upskirt", php_phpskirt_methods);
	phpskirt_class_entry = zend_register_internal_class(&ce TSRMLS_CC);
}


#ifdef COMPILE_DL_PHPSKIRT
ZEND_GET_MODULE(phpskirt)
#endif
