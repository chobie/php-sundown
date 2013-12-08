--TEST--
Check for list-multiparagraphs
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
<?php if (!extension_loaded("tidy")) print "skip"; ?>
--FILE--
<?php
$data = <<< DATA
*   This is a list item with two paragraphs. Lorem ipsum dolor
    sit amet, consectetuer adipiscing elit. Aliquam hendrerit
    mi posuere lectus.

    Vestibulum enim wisi, viverra nec, fringilla in, laoreet
    vitae, risus. Donec sit amet nisl. Aliquam semper ipsum
    sit amet velit.

*   Suspendisse id sem consectetuer libero luctus adipiscing.
DATA;
$md = new Sundown($data);
$result = $md->toHtml();

$tidy = new tidy;
$tidy->parseString($result, array("show-body-only"=>1));
$tidy->cleanRepair();
echo (string)$tidy;
--EXPECT--
<ul>
<li>
<p>This is a list item with two paragraphs. Lorem ipsum dolor sit
amet, consectetuer adipiscing elit. Aliquam hendrerit mi posuere
lectus.</p>
<p>Vestibulum enim wisi, viverra nec, fringilla in, laoreet vitae,
risus. Donec sit amet nisl. Aliquam semper ipsum sit amet
velit.</p>
</li>
<li>
<p>Suspendisse id sem consectetuer libero luctus adipiscing.</p>
</li>
</ul>
