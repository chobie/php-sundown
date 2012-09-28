--TEST--
Check for https://github.com/chobie/php-sundown/issues/28 (Render should interrupt by exception)
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
class R extends \Sundown\Render\Base
{
    public function image($link, $title, $alt)
    {
        if ($link == 'doesnt/exists.jpg') {
            throw new \Exception("err");
        }
    }
}

$c = <<<EOM
# Doc

![](doesnt/exists.jpg)

![](https://www.google.com/images/srpr/logo3w.png)
EOM;

$m = new \Sundown\Markdown(new R(), array('autolink' => true));
try {
  $m->render($c);
} catch (Exception $e) {
  echo $e->getMessage();
}
--EXPECT--
err