--TEST--
Check for Sundown\Markdown\render() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$data = <<<EOF
* Hello World

* * *
EOF;

$a = new Sundown\Markdown(new Sundown\Render\HTML());
// list
echo $a->render($data);
--EXPECT--
<ul>
<li>Hello World</li>
</ul>
<hr>