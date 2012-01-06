#!/usr/bin/env php -n -d extension=../../src/modules/sundown.so
<?php
$md = new Sundown\Markdown(Sundown\Render\HTML,array());
echo $md->render(file_get_contents($_SERVER['argv'][1]));
