PHP_ARG_ENABLE(redcarpet,Whether to enable the "redcarpet" extension,
  [  --enable-redcarpet      Enable "redcarpet" extension support])

if test $PHP_REDCARPET != "no"; then
  PHP_NEW_EXTENSION(redcarpet,php_redcarpet.c array.c buffer.c markdown.c render.c, $ext_shared)
fi