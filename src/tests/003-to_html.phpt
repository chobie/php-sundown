--TEST--
Check for Upskirt::to_html() feature
--SKIPIF--
<?php if (!extension_loaded("phpskirt")) print "skip"; ?>
--FILE--
<?php
$data = <<<EOF
* Hello World
EOF;

$a = new Upskirt($data);
// list
echo $a->to_html();
--EXPECT--
<ul>
<li>Hello World</li>
</ul>
