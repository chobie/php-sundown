--TEST--
Check for Sundown\Render\HTML::list_box() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
$res = $render->getRenderFlags();
echo count($res) . PHP_EOL;
$res = $render->getRenderFlags();
echo count($res) . PHP_EOL;
$render = new Sundown\Render\HTML(array("filter_html"=>true));
$res = $render->getRenderFlags();
var_dump($res);
$res = $render->getRenderFlags();
var_dump($res);
--EXPECT--
0
0
array(1) {
  ["filter_html"]=>
  bool(true)
}
array(1) {
  ["filter_html"]=>
  bool(true)
}