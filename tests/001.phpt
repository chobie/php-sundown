--TEST--
Check for sundown presence
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
echo "sundown extension is available";
--EXPECT--
sundown extension is available
