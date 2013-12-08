--TEST--
Check for list-code
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
*   a list containing a block of code

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
<ul>
<li>
<p>a list containing a block of code</p>
<pre>
<code>10 PRINT HELLO INFINITE
20 GOTO 10
</code>
</pre></li>
</ul>
