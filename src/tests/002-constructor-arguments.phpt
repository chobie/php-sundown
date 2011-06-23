--TEST--
Check for Upskirt::__construct arguments
--SKIPIF--
<?php if (!extension_loaded("phpskirt")) print "skip"; ?>
--FILE--
<?php
$a = new Upskirt();
if (is_null($a->extensions) && is_null($a->enable_pants)) {
  echo "OK" .PHP_EOL;
}
$a = new Upskirt(array('autolink'=>true));
if (isset($a->extensions['autolink']) && is_null($a->enable_pants)) {
  echo "OK" .PHP_EOL;
}
--EXPECT--
OK
OK