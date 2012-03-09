--TEST--
Check for https://github.com/chobie/php-sundown/issues/12
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$data = <<<EOF
* Hello World

* * *
EOF;

$a = new Sundown\Markdown(new Sundown\Render\HTML());
echo $a->render($data);
echo $a->render($data);
echo $a->render($data);
--EXPECT--
<ul>
<li>Hello World</li>
</ul>
<hr>
<ul>
<li>Hello World</li>
</ul>
<hr>
<ul>
<li>Hello World</li>
</ul>
<hr>