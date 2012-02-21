--TEST--
Check for determine enabled extensions.
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();

$extensions = array("no_intraemphasis","tables","fenced_code_blocks","autolink","strikethrough","lax_html_blocks","space_after_header","superscript");
$enabled = array();
foreach ($extensions as $ext) {
  $enabled[$ext] = true;
}

$markdown = new Sundown\Markdown($render, $enabled);
foreach ($markdown->getExtensions() as $ext_name => $ext_value) {
	printf("%s => %d\n",$ext_name,$ext_value);
}

echo "# reference count check\n";
$markdown = new Sundown\Markdown($render, $enabled);
foreach ($markdown->getExtensions() as $ext_name => $ext_value) {
	printf("%s => %d\n",$ext_name,$ext_value);
}

--EXPECT--
no_intraemphasis => 1
tables => 1
fenced_code_blocks => 1
autolink => 1
strikethrough => 1
lax_html_blocks => 1
space_after_header => 1
superscript => 1
# reference count check
no_intraemphasis => 1
tables => 1
fenced_code_blocks => 1
autolink => 1
strikethrough => 1
lax_html_blocks => 1
space_after_header => 1
superscript => 1