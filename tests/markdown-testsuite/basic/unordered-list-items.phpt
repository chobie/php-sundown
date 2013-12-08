--TEST--
Check for unordered-list-items
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
 * list item 1
 * list item 2
 * list item 3
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<ul>
<li>list item 1</li>
<li>list item 2</li>
<li>list item 3</li>
</ul>
