--TEST--
Check for phpskirt presence
--SKIPIF--
<?php if (!extension_loaded("phpskirt")) print "skip"; ?>
--FILE--
<?php
echo "phpskirt extension is available";
--EXPECT--
phpskirt extension is available
