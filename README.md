PHP + Upskirt
===========================

phpskirt is just simple wrapper of <https://github.com/tanoku/upskirt>.

License
-------

Copyright (c) 2011, Shuhei Tanuma

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

Install
-------

    git clone https://github.com/chobie/phpskirt.git && cd phpskirt
    git submodule init
    cd src
    phpize && ./configure
    make
    sudo make install
    # extension=phpskirt.so

Example
-------

    $markdown = new Upskirt(string $string [, array $extensions]);
    echo $markdown->to_html();
    echo $markdown->to_toc();

Supported Extensions
--------------------

````
<?php

// default: disable all.
// maybe this array will be integer (bit flags).
new Upskirt('something',array(
	"filter_html"=>true,
	"no_image"=>true,
	"no_links"=>true,
	"filter_styles"=>true,
	"safelink" => true,
	"generate_toc" => true,
	"hard_wrap" => true,
	"gh_blockcode" => true,
	"xhtml" => true,
	"autolink"=>true,
	"no_intraemphasis" => true,
	"tables" => true,
	"fenced_code" => true,
	"strikethrough" => true,
	"lax_htmlblock" => true,
	"space_header" => true,
));
````
