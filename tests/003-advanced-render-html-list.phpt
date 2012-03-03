--TEST--
Check for Sundown\Render\HTML::list_box() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
echo $render->listBox("",1);
echo $render->listBox("",0);
echo $render->listItem("hello",1);
echo $render->listItem("hello",0);
--EXPECT--
<ol>
</ol>
<ul>
</ul>
<li>hello</li>
<li>hello</li>