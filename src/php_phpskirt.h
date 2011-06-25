#ifndef PHP_PHPSKIRT_H

#define PHP_PHPSKIRT_H

#define PHP_PHPSKIRT_EXTNAME "phpskirt"
#define PHP_PHPSKIRT_EXTVER "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "markdown.h"
#include "html.h"

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry phpskirt_module_entry;
#define phpext_phpskirt_ptr &phpskirt_module_entry;

extern zend_class_entry *phpskirt_class_entry;

#endif /* PHP_PHPSKIRT_H */