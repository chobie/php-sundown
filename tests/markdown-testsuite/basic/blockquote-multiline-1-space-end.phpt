--TEST--
Check for blockquote-multiline-1-space-end
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
>A blockquote 
>on multiple lines 
>like this. 
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<blockquote>
<p>A blockquote on multiple lines like this.</p>
</blockquote>
--XFAIL--
discrepancy in interpretation