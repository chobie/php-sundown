--TEST--
Check for Sundown\Render\HTML::block_quote() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
echo $render->blockQuote("quoted paragraph") . PHP_EOL;
--EXPECT--
<blockquote>
quoted paragraph</blockquote>