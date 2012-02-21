--TEST--
Check for Sundown\Render\HTML::list_box() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
echo $render->list_box("",1);
echo $render->list_box("",0);
echo $render->list_item("hello",1);
echo $render->list_item("hello",0);
--EXPECT--
<ol>
</ol>
<ul>
</ul>
<li>hello</li>
<li>hello</li>