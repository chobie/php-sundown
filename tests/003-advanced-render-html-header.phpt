--TEST--
Check for Sundown\Render\HTML::header() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
echo $render->header("Hello World",1);
echo $render->header("Hello World",2);
echo $render->header("Hello World",3);
--EXPECT--
<h1>Hello World</h1>
<h2>Hello World</h2>
<h3>Hello World</h3>