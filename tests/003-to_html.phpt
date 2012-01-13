--TEST--
Check for Sundown::to_html() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$data = <<<EOF
* Hello World
EOF;

$a = new Sundown($data);
// list
echo $a->to_html();
--EXPECT--
<ul>
<li>Hello World</li>
</ul>
