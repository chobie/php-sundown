--TEST--
Check for list-blockquote
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
*   a list containing a blockquote

    > this the blockquote in the list
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
<p>a list containing a blockquote</p>
<blockquote>
<p>this the blockquote in the list</p>
</blockquote>
</li>
</ul>
