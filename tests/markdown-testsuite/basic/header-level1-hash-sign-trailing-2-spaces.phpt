--TEST--
Check for header-level1-hash-sign-trailing-2-spaces
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
# this is an h1 with two trailing spaces  
A new paragraph.
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<h1>this is an h1 with two trailing spaces</h1>
<p>A new paragraph.</p>
