# Make your custom render

php-sundown is able to make custom render easily.
there are two render in php-sundown:

*Sundown\Render\Base*: Basic render class. this class does't decorate anything. 
*Sundown\Render\HTML*: HTML render class.
*Sundown\Render\XHTML*: XHTML render class.

# EXPERIMENTAL

these methods name will be use came case.

### \Sundown\Render\Base::__construct()

### \Sundown\Render\Base::getRenderFlags()

### \Sundown\Render\Base::setRenderFlags(array $flags)

### SundownRenderBase::enable_pants

### SundownRenderBase::block_code

### SundownRenderBase::block_quote

### SundownRenderBase::block_html

### SundownRenderBase::header

### SundownRenderBase::hrule

### SundownRenderBase::list_box

### SundownRenderBase::list_item

### SundownRenderBase::paragraph

### SundownRenderBase::table

### SundownRenderBase::table_row

### SundownRenderBase::table_cell

### SundownRenderBase::autolink

### SundownRenderBase::codespan

### SundownRenderBase::double_emphasis

### SundownRenderBase::emphasis

### SundownRenderBase::image

### SundownRenderBase::linebreak

### SundownRenderBase::link

### SundownRenderBase::raw_html

### SundownRenderBase::triple_emphasis

### SundownRenderBase::strikethrough

### SundownRenderBase::superscript

### SundownRenderBase::entity

### SundownRenderBase::normal_text

### SundownRenderBase::doc_header

### SundownRenderBase::doc_footer

### SundownRenderBase::preprocess

### SundownRenderBase::postprocess