--TEST--
Check for code-4-spaces-escaping
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
    10 PRINT < > &
    20 GOTO 10
DATA;
$md = new Sundown\Markdown(new Sundown\Render\Html());
$result = $md->render($data);

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<pre>
<code>10 PRINT &lt; &gt; &amp;
20 GOTO 10
</code>
</pre>