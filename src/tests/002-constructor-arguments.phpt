--TEST--
Check for Upskirt::__construct arguments
--SKIPIF--
<?php if (!extension_loaded("phpskirt")) print "skip"; ?>
--FILE--
<?php
$a = new Upskirt('a');
if (is_array($a->extensions) && is_null($a->enable_pants) && $a->data == 'a') {
  echo "OK" .PHP_EOL;
}
$a = new Upskirt('a',array('autolink'=>true));
if (isset($a->extensions['autolink']) && is_null($a->enable_pants) && $a->data == 'a') {
  echo "OK" .PHP_EOL;
}
--EXPECT--
OK
OK
