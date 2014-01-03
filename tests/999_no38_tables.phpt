--TEST--
Check for https://github.com/chobie/php-sundown/issues/34 (incorrect render flags behavior)
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$src = '
| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |
';

$md = new \Sundown\Markdown(\Sundown\Render\HTML, array(
    'tables' => true,
));

echo $md->render($src);
--EXPECT--
<table><thead>
<tr>
<th>Tables</th>
<th align="center">Are</th>
<th align="right">Cool</th>
</tr>
</thead><tbody>
<tr>
<td>col 3 is</td>
<td align="center">right-aligned</td>
<td align="right">$1600</td>
</tr>
<tr>
<td>col 2 is</td>
<td align="center">centered</td>
<td align="right">$12</td>
</tr>
<tr>
<td>zebra stripes</td>
<td align="center">are neat</td>
<td align="right">$1</td>
</tr>
</tbody></table>