PHP_ARG_ENABLE(phpskirt,Whether to enable the "phpskirt" extension,
  [  --enable-phpskirt      Enable "phpskirt" extension support])

if test $PHP_PHPSKIRT != "no"; then
  PHP_NEW_EXTENSION(phpskirt,phpskirt.c array.c buffer.c markdown.c html.c html_smartypants.c, $ext_shared)
fi
