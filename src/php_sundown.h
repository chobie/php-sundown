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

extern zend_class_entry *sundown_class_entry;

#endif /* PHP_SUNDOWN_H */