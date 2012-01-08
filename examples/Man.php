<?php
/*
 * The MIT License
 *
 * Copyright (c) 2011 - 2012 Shuhei Tanuma
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
namespace Sundown\Render;

/**
 * Man Render - sample custom render for Sundown
 *
 * <code>
 * $sd = new Sundown\Markdown(new Sundown\Render\Man());
 * echo $sd->render("# parsed as markdown, render as Man format")
 * </code>
 */
class Man extends Base
{
	public function normal_text($text)
	{
		return preg_replace("/-/","\\-",$text);
	}

	public function block_code($code, $language)
	{
		return "\n.nf\n{$this->normal_text($code)}\n.fi\n";
	}
	
	public function codespan($code)
	{
		return $this->block_code($code, null);
	}
 
	public function header($title, $level)
	{
		switch($level)
		{
			case 1:
				return "\n.TH {$title}\n";
				break;
			case 2:
				return "\n.SH {$title}\n";
				break;
			case 3:
				return "\n.SS {$title}\n";
				break;
		}
	}

	public function double_emphasis($text)
	{
		return "\\fI{$text}\\fP";
	}

	public function linebreak()
	{
		return "\n.LP\n";
	}

	public function paragraph($text)
	{
		return "\n.TP\n{$text}\n";
	}

	public function list_box($content, $list_type)
	{
		switch($list_type)
		{
			case 0:
				return "\n\n.nr step 0 1\n{$content}\n";
				break;
			case 1:
				return "\n.\n{$content}\n";
				break;
		}
	}

	public function list_item($content, $list_type)
	{
		$content = trim($content);
		switch($list_type)
		{
			case 0:
				return ".IP \\n+[step]\n{$content}\n";
				break;
			case 1:
				return ".IP \\[bu] 2 \n{$content}\n";
				break;
		}
	}
}
