--TEST--
Check for link-idref-title-next-line
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
[World Wide Web Consortium][w3c]

[w3c]: http://www.w3.org/
   "Discover W3C"
DATA;
$md = new Sundown\Markdown(new Sundown\Render\Html());
$result = $md->render($data);

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p><a href="http://www.w3.org/" title="Discover W3C">World Wide Web
Consortium</a></p>
