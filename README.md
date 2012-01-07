PHP + Sundown
===========================

php-sundown is just simple wrapper of <https://github.com/tanoku/sundown>.

[![Build Status](https://secure.travis-ci.org/chobie/php-sundown.png)](http://travis-ci.org/chobie/php-sundown)

License
-------

Copyright (c) 2012, Shuhei Tanuma

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

Requirements
------------

PHP5.3 higher

Install
-------

    git clone https://github.com/chobie/php-sundown.git php-sundown -b development
    cd php-sundown
    rake submodule compile
    sudo rake install
    # please add following line to your php.ini
    # extension=sundown.so

Conformance Testing
-------------------

    rake test:conformance

Overview
-------

php-sudown has two styles: basic and advance.

*basic*: less exntensiblity, fast rendering.
*advance*: custmize your own markdown render, little bit slower than basic.


### \Sundown::__construct(string $str [, array $config])

##### *Description*

make a basic Sundown instance with specified string and config.

##### *Parameters*

*str*: you want parse and render as markdown html.
*config*: sundown parser options

##### *Return Value*

*\Sundown*: sundown instance

##### *Example*

````php
<?php
$sd = new Sundown("Hello World");
$sd->to_html();
````

### \Sundown::to_html()

##### *Description*

parse text as markdown and returns rendered html.

##### *Parameters*

##### *Return Value*

*string*: rendered html

##### *Example*

````php
<?php
$sd = new Sundown("Hello World");
echo $sd->to_html();
````

### \Sundown::to_toc()

##### *Description*

parse text as markdown and only returns toc parts.

##### *Parameters*

##### *Return Value*

*string*: rendered html

##### *Example*

````php
<?php
$sd = new Sundown("Hello World");
echo $sd->to_toc();
````

### \Sundown\Markdown::__construct(\Sundown\Render\Base $render[, array $config])

##### *Description*

make a advance Sundown instance with specified render and config.

##### *Parameters*

*$render*: it inherits Sundown\Render\Base class. also class name accepts.
*$config*: sundown parser option.

##### *Return Value*

*\Sundown\Markdown*: 

##### *Example*

````php
<?php
$md = new \Sundown\Markdown(\Sundown\Render\HTML);
echo $md->render("Hello World");
````

### \Sundown\Markdown::render(string $str)

##### *Description*

parse and render specified string.

##### *Parameters*

*$str*: parse and render as markdown text.

##### *Return Value*

*mixed*: renderd something. (this depends render) 

##### *Example*

````php
<?php
$render = new \Sundown\Render\HTML();
$md = new \Sundown\Markdown($render);
echo $md->render("Hello World");n
````

### \Sundown\Render\Base

documented at [Render.md](../Render.md)
