--TEST--
Check for ampersand-uri
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
There is an [ampersand](http://validator.w3.org/check?uri=http://www.w3.org/&verbose=1) in the URI.
DATA;
$md = new Sundown\Markdown(new Sundown\Render\Html());
$result = $md->render($data);

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p>There is an <a href=
"http://validator.w3.org/check?uri=http://www.w3.org/&amp;verbose=1">
ampersand</a> in the URI.</p>
