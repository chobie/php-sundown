# Make your custom render

php-sundown is able to make custom render easily.
there are two render in php-sundown:

*Sundown\Render\Base*: Basic render class. this class does't decorate anything. 
*Sundown\Render\HTML*: HTML render class.
*Sundown\Render\XHTML*: XHTML render class.

### \Sundown\Render\Base::__construct()

### \Sundown\Render\Base::getRenderFlags()

### \Sundown\Render\Base::setRenderFlags(array $flags)

### SundownRenderBase::blockCode(string $code, string $language)

### SundownRenderBase::blockQuote(string $quote)

### SundownRenderBase::blockHtml(string $raw_html)

### SundownRenderBase::header(string $htext, int $header_level)

### SundownRenderBase::hrule()

### SundownRenderBase::listBox(string $contents, int $list_type)

### SundownRenderBase::listItem(string $text int $list_type)

### SundownRenderBase::paragraph(string $text)

### SundownRenderBase::table(string $header, string $body)

### SundownRenderBase::tableRow(string $content)

### SundownRenderBase::tableCell(string $content, string $alignment)

### SundownRenderBase::autolink(string $link, string $link_type)

### SundownRenderBase::codespan(string $code)

### SundownRenderBase::doubleEmphasis(string $text)

### SundownRenderBase::emphasis(string $text)

### SundownRenderBase::image(string $link, string $title, string $alt_text)

### SundownRenderBase::linebreak

### SundownRenderBase::link(string $link, string $title, string $content)

### SundownRenderBase::rawHtml(string $raw_html)

### SundownRenderBase::tripleEmphasis(stirng $text)

### SundownRenderBase::strikethrough(string $text)

### SundownRenderBase::superscript(string $text)

### SundownRenderBase::entity(string $text)

### SundownRenderBase::normalText(string $text)

### SundownRenderBase::docHeader()

### SundownRenderBase::docFooter()

### SundownRenderBase::preprocess(string $full_document)

### SundownRenderBase::postprocess(string $full_document)