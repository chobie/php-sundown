--TEST--
Check for Sundown\Render\HTML::block_code() feature
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$render = new Sundown\Render\HTML();
echo $render->blockCode("echo 'abc'","php") . PHP_EOL;
echo $render->blockCode("echo 'abc'", null) . PHP_EOL;
--EXPECT--
<pre><code class="php">echo &#39;abc&#39;</code></pre>

<pre><code>echo &#39;abc&#39;</code></pre>