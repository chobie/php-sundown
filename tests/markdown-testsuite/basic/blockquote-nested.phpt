--TEST--
Check for blockquote-nested
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
> This is the first level of quoting.
> > This is nested blockquote.

DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<blockquote>
<p>This is the first level of quoting.</p>
<blockquote>
<p>This is nested blockquote.</p>
</blockquote>
</blockquote>
