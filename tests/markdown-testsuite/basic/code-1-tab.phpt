--TEST--
Check for code-1-tab
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
	10 PRINT HELLO INFINITE
	20 GOTO 10
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<pre>
<code>10 PRINT HELLO INFINITE
20 GOTO 10
</code>
</pre>