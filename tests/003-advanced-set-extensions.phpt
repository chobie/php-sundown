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

$markdown = new Sundown\Markdown($render, array());
$exts = $markdown->getExtensions();
printf("ext count: %d\n", count($exts));

echo "# do set extensions\n";
$markdown->setExtensions($enabled);
foreach ($markdown->getExtensions() as $ext_name => $ext_value) {
	printf("%s => %d\n",$ext_name,$ext_value);
}

--EXPECT--
ext count: 0
# do set extensions
no_intraemphasis => 1
tables => 1
fenced_code_blocks => 1
autolink => 1
strikethrough => 1
lax_html_blocks => 1
space_after_header => 1
superscript => 1
