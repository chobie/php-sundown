--TEST--
Check for Upskirt::to_html() feature
--SKIPIF--
<?php if (!extension_loaded("phpskirt")) print "skip"; ?>
--FILE--
<?php
$a = new Upskirt();
$data = <<<EOF
* Hello World
EOF;

// list
echo $a->to_html($data);
--EXPECT--
<ul>
<li>Hello World</li>
</ul>
