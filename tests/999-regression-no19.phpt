--TEST--
Check for https://github.com/chobie/php-sundown/issues/19
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
//https://github.com/chobie/php-sundown/issues/19
$sundown = new Sundown\Markdown(
    new Sundown\Render\HTML(array('hard_wrap' => true)),
    array(
        'no_intra_emphasis' =>true,
        'tables' => true,
        'fenced_code_blocks' => true,
        'autolink' => true,
        'strikethrough' => true,
        'lax_html_blocks' => true,
    )
);

echo $sundown->render(
    <<<MARKDOWN
http://foo.bar/
test@example.com
MARKDOWN
);
--EXPECT--
<p><a href="http://foo.bar/">http://foo.bar/</a><br>
<a href="mailto:test@example.com">test@example.com</a></p>