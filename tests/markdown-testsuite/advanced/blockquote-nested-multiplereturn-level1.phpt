--TEST--
Check for blockquote-nested-multiplereturn-level1
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
> This is the first level of quoting.
>
> > This is nested blockquote.
>
> Back to the first level.

DATA;
$md = new Sundown\Markdown(new Sundown\Render\Html());
$result = $md->render($data);

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
<p>Back to the first level.</p>
</blockquote>
