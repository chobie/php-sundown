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

Docs
-------

will be add soon on gh-pages.