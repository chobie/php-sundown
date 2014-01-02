PHP_ARG_ENABLE(sundown,Whether to enable the "sundown" extension,
  [  --enable-sundown      Enable "sundown" extension support])

PHP_ARG_ENABLE(sundown-debug, for sundown debug support,
  [  --enable-sundown-debug       Enable sundown debug support], no, no)

if test $PHP_SUNDOWN != "no"; then
  SUNDOWN_SOURCES="
php_sundown.c
sundown_markdown.c
render_base.c
render_html.c
render_xhtml.c
render_html_toc.c
hoedown/src/autolink.c
hoedown/src/buffer.c
hoedown/src/escape.c
hoedown/src/html.c
hoedown/src/html_blocks.c
hoedown/src/html_smartypants.c
hoedown/src/markdown.c
hoedown/src/stack.c
"

#  patch -p1 -N -b < buffer_win32_compat.patch

  PHP_NEW_EXTENSION(sundown,$SUNDOWN_SOURCES, $ext_shared)
  CFLAGS="-Wunused-variable -Wpointer-sign -Wimplicit-function-declaration -Winline -Wunused-macros -Wredundant-decls -Wstrict-aliasing=2 -Wswitch-enum -Wdeclaration-after-statement"

  if test "$PHP_SUNDOWN_DEBUG" != "no"; then
    CFLAGS="-g -O0 $CFLAGS"
  fi

  PHP_SUBST([CFLAGS])
  PHP_ADD_BUILD_DIR([$ext_builddir/hoedown/src])

  PHP_ADD_INCLUDE([$ext_srcdir/hoedown/src])
  PHP_ADD_INCLUDE([$ext_builddir/hoedown/src])
  PHP_ADD_MAKEFILE_FRAGMENT
fi
