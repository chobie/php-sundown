PHP + Sundown
===========================

php-sundown is just simple wrapper of <https://github.com/tanoku/sundown>.

[![Build Status](https://secure.travis-ci.org/chobie/php-sundown.png)](http://travis-ci.org/chobie/php-sundown)

License
-------
PHP License

THIS SOFTWARE IS PROVIDED BY THE PHP DEVELOPMENT TEAM ``AS IS'' AND 
ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE PHP
DEVELOPMENT TEAM OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

Author: Shuhei Tanuma

Requirements
------------

PHP5.3 higher

Install for developpers
-----------------------

    git clone https://github.com/chobie/php-sundown.git php-sundown -b development
    cd php-sundown
    # this command will fetch submodule and copy neccesally files to src dir. and compile it.
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

### \Sundown::toHTML()

##### *Description*

parse text as markdown and returns rendered html.

##### *Parameters*

##### *Return Value*

*string*: rendered html

##### *Example*

````php
<?php
$sd = new Sundown("Hello World");
echo $sd->toHTML();
````

### \Sundown::toTOC()

##### *Description*

parse text as markdown and only returns toc parts.

##### *Parameters*

##### *Return Value*

*string*: rendered html

##### *Example*

````php
<?php
$sd = new Sundown("Hello World");
echo $sd->toTOC();
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

*mixed*: rendered something. (this depends render) 

##### *Example*

````php
<?php
$render = new \Sundown\Render\HTML();
$md = new \Sundown\Markdown($render);
echo $md->render("Hello World");n
````



### \Sundown\Markdown::getExtensions()

##### *Description*

get current extensions.

##### *Parameters*

##### *Return Value*

* array

##### *Example*

````php
<?php
$md = new \Sundown\Markdown(\Sundown\Render\HTML,array("autolink"=>true));
$extensions = $md->getExtensions();
var_dump($extensions);
````

### \Sundown\Markdown::setExtensions(array $extensions)

##### *Description*

set extensions.

##### *Parameters*

*$extensions*: extensions array.

##### *Return Value*

* void

##### *Example*

````php
<?php
$md = new \Sundown\Markdown(\Sundown\Render\HTML,array("autolink"=>true));
$md->setExtensions(array("autolink"=>false));
var_dump($md->getExtensions());
````

### \Sundown\Markdown::getRender()

##### *Description*

get current render instance.

##### *Parameters*

##### *Return Value*

* Sundown\Render\Base 

##### *Example*

````php
<?php
$md = new \Sundown\Markdown(\Sundown\Render\HTML,array("autolink"=>true));
$md->getRender()->setRenderFlags(array("filter_html"=>true));
````


### \Sundown\Render\Base

documented at [Render.md](https://github.com/chobie/php-sundown/blob/development/docs/Render.md)

### Extensions and Render flags

documented at [ExtensionsAndRenderFlags.md](https://github.com/chobie/php-sundown/blob/development/docs/ExtensionsAndRenderFlags.md)



Contributors
------------
* Dominic Scheirlinck
* Shuhei Tanuma
