--TEST--
Check for unordered-list-paragraphs
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
* list item in paragraph

* another list item in paragraph
DATA;
$md = new Sundown\Markdown(new Sundown\Render\Html());
$result = $md->render($data);

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<ul>
<li>
<p>list item in paragraph</p>
</li>
<li>
<p>another list item in paragraph</p>
</li>
</ul>
