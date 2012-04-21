--TEST--
check for space_after_headers behavior
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$data = <<<EOF
#Level1
EOF;

echo "space_after_headers disabled:\n";
$a = new Sundown\Markdown(new Sundown\Render\HTML(), array("space_after_headers"=>false));
echo $a->render($data);
echo PHP_EOL;
echo "space_after_headers enabled:\n";
$a = new Sundown\Markdown(new Sundown\Render\HTML(), array("space_after_headers"=>true));
echo $a->render($data);
--EXPECT--
space_after_headers disabled:
<h1>Level1</h1>

space_after_headers enabled:
<p>#Level1</p>
