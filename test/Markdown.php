#!/usr/bin/env php
<?php
$md = new Sundown\Markdown(Sundown\Render\HTML,array());
echo $md->render(file_get_contents($_SERVER['argv'][1]));