--TEST--
Check for Sundown\Markdown::setRender
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML_TOC();
$data = <<<EOH
# Hello World

lorem ipsum dolar sit amet

## the World, What a beautiful it is.

lorem ipsum dolar sit amet

EOH;

$md = new Sundown\Markdown($render);
echo "TOC_DATA\n";
echo $md->render($data);
echo "DATA\n";
$md->setRender(new Sundown\Render\HTML(array("with_toc_data"=>true)));
echo $md->render($data);
$render = new Sundown\Render\HTML(array("with_toc_data"=>true));
$md->setRender($render);
echo $md->render($data);
$md->setRender(new Sundown\Render\HTML(array("with_toc_data"=>true)));
echo "#check render was not destroyed\n";
if($render instanceof Sundown\Render\HTML) {
echo "OK";
} else {
echo "FAILURE";
}

--EXPECT--
TOC_DATA
<ul>
<li>
<a href="#toc_0">Hello World</a>
<ul>
<li>
<a href="#toc_1">the World, What a beautiful it is.</a>
</li>
</ul>
</li>
</ul>
DATA
<h1 id="toc_0">Hello World</h1>
<p>lorem ipsum dolar sit amet</p>
<h2 id="toc_1">the World, What a beautiful it is.</h2>
<p>lorem ipsum dolar sit amet</p>
<h1 id="toc_0">Hello World</h1>
<p>lorem ipsum dolar sit amet</p>
<h2 id="toc_1">the World, What a beautiful it is.</h2>
<p>lorem ipsum dolar sit amet</p>
#check render was not destroyed
OK