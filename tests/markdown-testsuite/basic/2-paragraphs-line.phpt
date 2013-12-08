--TEST--
Check for 2-paragraphs-line
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
This a very long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long paragraph on 1 line.

A new long long long long long long long long long long long long long long long long paragraph on 1 line.
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p>This a very long long long long long long long long long long
long long long long long long long long long long long long long
long long long long long long long long long paragraph on 1
line.</p>
<p>A new long long long long long long long long long long long
long long long long long paragraph on 1 line.</p>
