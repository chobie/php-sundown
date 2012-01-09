PHP_ARG_ENABLE(sundown,Whether to enable the "sundown" extension,
  [  --enable-sundown      Enable "sundown" extension support])

if test $PHP_SUNDOWN != "no"; then
  PHP_NEW_EXTENSION(sundown,
	php_sundown.c \
	sundown_markdown.c \
	sundown_buffer.c \
	render_base.c \
	render_html.c \
	render_xhtml.c \
	buffer.c \
	markdown.c \
	html.c \
	html_smartypants.c \
	autolink.c \
	stack.c \
	houdini_href_e.c \
	houdini_html_e.c \
	, $ext_shared)
fi
