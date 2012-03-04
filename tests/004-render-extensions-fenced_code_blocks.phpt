--TEST--
Check for Sundown::to_html() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$data = <<<EOF
````php
<?php
echo $1;
````
EOF;

echo "fenced_code_blocks disabled:\n";
$a = new Sundown\Markdown(new Sundown\Render\HTML(), array("fenced_code_blocks"=>false));
echo $a->render($data);
echo PHP_EOL;
echo "fenced_code_blocks enabled:\n";
$a = new Sundown\Markdown(new Sundown\Render\HTML(), array("fenced_code_blocks"=>true));
echo $a->render($data);
--EXPECT--
fenced_code_blocks disabled:
<p><code>php
&lt;?php
echo $1;
</code></p>

fenced_code_blocks enabled:
<pre><code class="php">&lt;?php
echo $1;
</code></pre>