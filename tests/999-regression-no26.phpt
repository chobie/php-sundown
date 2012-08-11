--TEST--
Check for https://github.com/chobie/php-sundown/issues/19
--SKIPIF--
<?php if (!extension_loaded("sundown")) print "skip"; ?>
--FILE--
<?php
$table = <<<EOS
| First Header  | Second Header |
| ------------- | ------------- |
| Content Cell  | Content Cell  |
| Content Cell  | Content Cell  |
EOS;

//works:
$sd = new Sundown($table, array('tables'=>true));
echo $sd->toHTML();

//sementation fault:
$md = new \Sundown\Markdown(\Sundown\Render\HTML, array('tables'=>true)); 
echo $md->render($table);
--EXPECT--
<table><thead>
<tr>
<th>First Header</th>
<th>Second Header</th>
</tr>
</thead><tbody>
<tr>
<td>Content Cell</td>
<td>Content Cell</td>
</tr>
<tr>
<td>Content Cell</td>
<td>Content Cell</td>
</tr>
</tbody></table>
<table><thead>
<tr>
<td>First Header</td>
<td>Second Header</td>
</tr>
</thead><tbody>
<tr>
<td>Content Cell</td>
<td>Content Cell</td>
</tr>
<tr>
<td>Content Cell</td>
<td>Content Cell</td>
</tr>
</tbody></table>