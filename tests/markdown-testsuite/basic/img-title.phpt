--TEST--
Check for img-title
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
![HTML5](http://www.w3.org/html/logo/img/mark-word-icon.png "HTML5 logo for everyone")
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<p><img src="http://www.w3.org/html/logo/img/mark-word-icon.png"
alt="HTML5" title="HTML5 logo for everyone"></p>
