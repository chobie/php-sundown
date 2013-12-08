--TEST--
Check for link-automatic-email
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
This is an automatic email link <foolspam@example.org>
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p>This is an automatic email link <a href=
"mailto:foolspam@example.org">foolspam@example.org</a></p>
