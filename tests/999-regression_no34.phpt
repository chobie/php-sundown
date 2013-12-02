--TEST--
Check for https://github.com/chobie/php-sundown/issues/34 (incorrect render flags behavior)
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$options = array("filter_html"=>true);
$renderer = new \Sundown\Render\HTML($options);
$md = new \Sundown\Markdown($renderer);

/**
 * this will outputs `<p>foo</p>` as we don't allow any html elements.
 * just process `foo`. then output will be <p>foo</p>
 *
 * NOTE: This behavior only affects html render
 */
echo $md->render("<strong>foo</strong>");
--EXPECT--
<p>foo</p>