#ifndef PHP_REDCARPET_H

#define PHP_REDCARPET_H

#define PHP_REDCARPET_EXTNAME "redcarpet"
#define PHP_REDCARPET_EXTVER "0.1"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "markdown.h"

/* Define the entry point symbol
 * Zend will use when loading this module
 */
extern zend_module_entry redcarpet_module_entry;
#define phpext_redcarpet_ptr &redcarpet_module_entry;

extern PHPAPI zend_class_entry *redcarpet_class_entry;

#endif /* PHP_REDCARPET_H */