--TEST--
Check for Sundown::__construct arguments
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$a = new Sundown('a');
if (is_array($a->extensions) && is_null($a->enable_pants) && $a->data == 'a') {
  echo "OK" .PHP_EOL;
}
$a = new Sundown('a',array('autolink'=>true));
if (isset($a->extensions['autolink']) && is_null($a->enable_pants) && $a->data == 'a') {
  echo "OK" .PHP_EOL;
}
--EXPECT--
OK
OK
