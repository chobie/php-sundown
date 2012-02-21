--TEST--
Check for Sundown\Render\HTML::list_box() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
$render->setRenderFlags(array("filter_html"=>true));
$res = $render->getRenderFlags();
echo count($res) . PHP_EOL;
$res = $render->getRenderFlags();
echo count($res) . PHP_EOL;
$render = new Sundown\Render\HTML(array("filter_html"=>true));
$res = $render->setRenderFlags(array("filter_html"=>false));
$res = $render->getRenderFlags();
var_dump($render);
var_dump($res);
$res = $render->getRenderFlags();
var_dump($res);
--EXPECT--
1
1
object(Sundown\Render\HTML)#2 (1) {
  ["render_flags"]=>
  array(1) {
    ["filter_html"]=>
    bool(false)
  }
}
array(1) {
  ["filter_html"]=>
  bool(false)
}
array(1) {
  ["filter_html"]=>
  bool(false)
}