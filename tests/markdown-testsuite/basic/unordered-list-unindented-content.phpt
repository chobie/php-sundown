--TEST--
Check for unordered-list-unindented-content
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
*   This a very long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long paragraph in a list.
*   and yet another long long long long long long long long long long long long long long long long long long long long long long line.
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<ul>
<li>This a very long long long long long long long long long long
long long long long long long long long long long long long long
long long long long long long long long long paragraph in a
list.</li>
<li>and yet another long long long long long long long long long
long long long long long long long long long long long long long
line.</li>
</ul>
