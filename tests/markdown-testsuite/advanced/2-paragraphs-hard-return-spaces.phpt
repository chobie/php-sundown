--TEST--
Check for 2-paragraphs-hard-return-spaces
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
This is a first paragraph,
on multiple lines.
     
This is a second paragraph.
There are spaces in between the two.
DATA;
$md = new Sundown\Markdown(new Sundown\Render\Html());
$result = $md->render($data);

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p>This is a first paragraph, on multiple lines.</p>
<p>This is a second paragraph. There are spaces in between the
two.</p>
