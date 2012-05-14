PHP_ARG_ENABLE(sundown,Whether to enable the "sundown" extension,
  [  --enable-sundown      Enable "sundown" extension support])

if test $PHP_SUNDOWN != "no"; then
  SUNDOWN_SOURCES="
php_sundown.c
sundown_markdown.c
render_base.c
render_html.c
render_xhtml.c
render_html_toc.c
sundown/src/buffer.c
sundown/src/markdown.c
sundown/src/autolink.c
sundown/src/stack.c
sundown/html/html.c
sundown/html/html_smartypants.c
sundown/html/houdini_href_e.c
sundown/html/houdini_html_e.c
"

  PHP_NEW_EXTENSION(sundown,$SUNDOWN_SOURCES, $ext_shared)
  CFLAGS=" -Wunused-variable -Wpointer-sign -Wimplicit-function-declaration"
  PHP_SUBST([CFLAGS])

  PHP_ADD_BUILD_DIR([$ext_builddir/sundown/src])
  PHP_ADD_BUILD_DIR([$ext_builddir/sundown/html])

  PHP_ADD_INCLUDE([$ext_srcdir/sundown/src])
  PHP_ADD_INCLUDE([$ext_srcdir/sundown/html])
  PHP_ADD_INCLUDE([$ext_builddir/sundown/src])
  PHP_ADD_INCLUDE([$ext_builddir/sundown/html])
fi
