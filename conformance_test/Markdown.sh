#/usr/bin/bash
PHP=`which php`
PWD=`pwd`
$PHP -n -d extension=$PWD/../../modules/sundown.so $PWD/../Markdown.php "$1"
