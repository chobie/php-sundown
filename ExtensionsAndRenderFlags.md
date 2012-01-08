# Extensions and Render flags.

````
extensions - a hash containing the Markdown extensions which the parser
            will identify. The following extensions are accepted:

            :no_intra_emphasis - do not parse emphasis inside of words.
                Strings such as `foo_bar_baz` will not generate `<em>`
                tags.

            :tables - parse tables, PHP-Markdown style

            :fenced_code_blocks - parse fenced code blocks, PHP-Markdown
                style. Blocks delimited with 3 or more `~` or backticks
                will be considered as code, without the need to be
                indented. An optional language name may be added at the
                end of the opening fence for the code block

            :autolink - parse links even when they are not enclosed in
                `<>` characters. Autolinks for the http, https and ftp
                protocols will be automatically detected. Email addresses
                are also handled, and http links without protocol, but
                starting with `www.`

            :strikethrough - parse strikethrough, PHP-Markdown style
                Two `~` characters mark the start of a strikethrough,
                e.g. `this is ~~good~~ bad`

            :lax_html_blocks - HTML blocks do not require to be surrounded
                by an empty line as in the Markdown standard.

            :space_after_headers - A space is always required between the
                hash at the beginning of a header and its name, e.g.
                `#this is my header` would not be a valid header.

            :superscript - parse superscripts after the `^` character;
                contiguous superscripts are nested together, and complex
                values can be enclosed in parenthesis,
                e.g. `this is the 2^(nd) time`

Example:

    $sd = new Sundown\Markdown(Sundown\Render\HTML,
        ["autolink" => true, "space_after_headers" => true]);
````

## Render flags

````
new Sundown\Render\HTML($render_flags=array())

Initializes an HTML renderer. The following flags are available:

    :filter_html - do not allow any user-inputted HTML in the output

    :no_images - do not generate any `<img>` tags

    :no_links - do not generate any `<a>` tags

    :no_styles - do not generate any `<style>` tags

    :safe_links_only - only generate links for protocols which are considered safe

    :with_toc_data - add HTML anchors to each header in the output HTML,
        to allow linking to each section.

    :hard_wrap - insert HTML `<br>` tags inside on paragraphs where the origin
        Markdown document had newlines (by default, Markdown ignores these
        newlines).

    :xhtml - output XHTML-conformant tags. This option is always enabled in the
        `Sundown\Render\XHTML` renderer. (@todo)

Example:

    $rndr = new Sundown\Render\HTML(["no_links" => true, "hard_wrap" => true])
````


### notes

Sundown class is able to mix extensions and render_flags.

````
new Sundown($text, [
	"hard_wrap"=>true, // render_flags,
	"autolink"=>true, // extensions
]);
````