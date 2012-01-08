--TEST--
Check for Sundown\Markdown::__construct() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$a = new Sundown\Markdown(Sundown\Render\HTML);
$a = new Sundown\Markdown(new Sundown\Render\HTML());
$r = new Sundown\Render\HTML();
$a = new Sundown\Markdown($r);
$a = new Sundown\Markdown($r,array());
echo "OK";
--EXPECT--
OK