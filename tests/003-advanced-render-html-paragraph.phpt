--TEST--
Check for Sundown\Render\HTML::paragraph() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
echo $render->paragraph("Hello World");
--EXPECT--
<p>Hello World</p>