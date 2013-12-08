--TEST--
Check for determine render flags.
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$r = new Sundown("http://chobie.org/");

$flags = array("filter_html","no_images","no_links","no_styles","safe_links_only","with_toc_data","hard_wrap","xhtml");
foreach ($flags as $flag) {
    var_dump($r->hasRenderFlag($flag));
}

foreach ($flags as $flag) {
    $r = new Sundown("http://chobie.org/", array($flag => true));
    echo "test {$flag}:\n";
    foreach ($flags as $f) {
        var_dump($r->hasRenderFlag($f));
    }
}

$extensions = array("no_intra_emphasis","tables","fenced_code_blocks","autolink","strikethrough","lax_html_blocks","space_after_header","superscript");
$exts = array();
foreach ($extensions as $ext) {
    $exts[$ext] = true;
}

$r = new Sundown("http://chobie.org/");
echo "render flags and extensions should not conflict 1:\n";
foreach ($flags as $flag) {
    var_dump($r->hasRenderFlag($flag));
}
echo "render flags and extensions should not conflict 2:\n";
$r = new Sundown("http://chobie.org/",$exts);
foreach ($flags as $flag) {
    var_dump($r->hasRenderFlag($flag));
}

foreach ($flags as $flag) {
    $r = new Sundown("http://chobie.org/", array_merge($exts,array($flag => true)));
    echo "test {$flag}:\n";
    foreach ($flags as $f) {
        var_dump($r->hasRenderFlag($f));
    }
}

--EXPECT--
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test filter_html:
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test no_images:
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test no_links:
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test no_styles:
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
test safe_links_only:
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
test with_toc_data:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
test hard_wrap:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
test xhtml:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
render flags and extensions should not conflict 1:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
render flags and extensions should not conflict 2:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test filter_html:
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test no_images:
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test no_links:
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
test no_styles:
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
bool(false)
test safe_links_only:
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
bool(false)
test with_toc_data:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
bool(false)
test hard_wrap:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
bool(false)
test xhtml:
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(false)
bool(true)
