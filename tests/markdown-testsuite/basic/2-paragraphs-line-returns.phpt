--TEST--
Check for 2-paragraphs-line-returns
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
A first paragraph.



A second paragraph after 3 CR (carriage return).
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p>A first paragraph.</p>
<p>A second paragraph after 3 CR (carriage return).</p>
