--TEST--
Check for 2-paragraphs-line-tab
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
This a very long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long long paragraph on 1 line.
	
1 tab to separate them and a new long long long long long long long long long long long long long long long long paragraph on 1 line.
DATA;
$md = new Sundown\Markdown(new Sundown\Render\Html());
$result = $md->render($data);

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p>This a very long long long long long long long long long long
long long long long long long long long long long long long long
long long long long long long long long long paragraph on 1
line.</p>
<p>1 tab to separate them and a new long long long long long long
long long long long long long long long long long paragraph on 1
line.</p>
