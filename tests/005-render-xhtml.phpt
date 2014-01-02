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

$a = new Sundown\Markdown(new Sundown\Render\XHTML());
echo $a->render($data);
echo PHP_EOL;

echo "force_enable_xhtml_flag:\n";
$a = new Sundown\Markdown(new Sundown\Render\XHTML(array("xhtml"=>false)));
echo $a->render($data);
--EXPECT--
<ul>
<li>Hello World</li>
</ul>

<hr/>

force_enable_xhtml_flag:
<ul>
<li>Hello World</li>
</ul>

<hr/>
