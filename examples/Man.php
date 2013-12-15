<?php
/**
 *  +----------------------------------------------------------------------+
 *  | PHP Version 5                                                        |
 *  +----------------------------------------------------------------------+
 *  | Copyright (c) 1997-2011 The PHP Group                                |
 *  +----------------------------------------------------------------------+
 *  | This source file is subject to version 3.01 of the PHP license,      |
 *  | that is bundled with this package in the file LICENSE, and is        |
 *  | available through the world-wide-web at the following url:           |
 *  | http://www.php.net/license/3_01.txt                                  |
 *  | If you did not receive a copy of the PHP license and are unable to   |
 *  | obtain it through the world-wide-web, please send a note to          |
 *  | license@php.net so we can mail you a copy immediately.               |
 *  +----------------------------------------------------------------------+
 *  | Authors: Shuhei Tanuma <chobieeee@php.net>                           |
 *  +----------------------------------------------------------------------+
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
