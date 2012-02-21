--TEST--
Check for determine enabled extensions.
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();

$markdown = new Sundown\Markdown($render, array());
$extensions = array("no_intraemphasis","tables","fenced_code_blocks","autolink","strikethrough","lax_html_blocks","space_after_header","superscript");

$enabled = array();
foreach ($extensions as $ext) {
  $enabled[$ext] = true;
}

echo "default: all extensions are disabled\n";
foreach ($extensions as $ext) {
	var_dump($markdown->hasExtension($ext));
}

echo "enabled all:\n";
$markdown = new Sundown\Markdown($render, $enabled);
foreach ($extensions as $ext) {
	var_dump($markdown->hasExtension($ext));
}

foreach ($extensions as $ext) {
	echo "only {$ext}:\n";
	$markdown = new Sundown\Markdown($render, array($ext => true));
	foreach ($extensions as $ext2) {
		var_dump($markdown->hasExtension($ext2));
	}
}

--EXPECT--
default: all extensions are disabled
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
enabled all:
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
only no_intraemphasis:
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
only tables:
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
only fenced_code_blocks:
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
only autolink:
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
only strikethrough:
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
only lax_html_blocks:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
only space_after_header:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
only superscript:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
