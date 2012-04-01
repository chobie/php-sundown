--TEST--
Check for custom render of codeBlocks
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php

class CustomRender extends Sundown\Render\Base
{
    public function blockCode($code, $language)
    {
        echo "OK.";
        return $code;
    }
}

$m = new Sundown\Markdown(
    new CustomRender(),
    array()
);

$m->render('
This is a normal paragraph.

    This is a code block

This is another normal paragraph.
');

$m2 = new Sundown\Markdown(
    new CustomRender(),
    array('fenced_code_blocks' => true)
);

$m->render('
This is a normal paragraph.

``` php
This is a fenced code block
```

This is another normal paragraph.
');

--EXPECT--
OK.OK.
