--TEST--
Check for determine enabled extensions.
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();

$markdown = new Sundown\Markdown($render, array());
$r = $markdown->getRender();

var_dump(spl_object_hash($render) == spl_object_hash($r));

$r = $markdown->getRender();
$r = $markdown->getRender();
$r = $markdown->getRender();
var_dump(spl_object_hash($render) == spl_object_hash($r));
--EXPECT--
bool(true)
bool(true)