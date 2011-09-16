
task :compile do
	Dir.chdir("src") do
		sh "phpize"
		sh "./configure"
		sh "make"
	end
end

task :install => [:compile] do
	Dir.chdir("src") do
		sh "make install"
	end
end

task :clean do
	Dir.chdir("src") do
		sh "phpize --clean"
	end
end

desc "Run conformance tests"
task "test:conformance" do
	Dir.chdir("test") do
		script = "#{pwd}/Markdown.php"
		Dir.chdir("MarkdownTest_1.0.3") do 
			sh "./MarkdownTest.pl --script='#{script}' --tidy"
		end
	end
end

desc 'Gather required Sundown sources into extension directory'
task :gather => 'sundown/src/markdown.h' do |t|
  files =
    FileList[
      'sundown/src/{markdown,buffer,stack,autolink,html_blocks}.h',
      'sundown/src/{markdown,buffer,stack,autolink}.c',
      'sundown/html/{html,html_smartypants,houdini_html_e,houdini_href_e}.c',
      'sundown/html/{html,houdini}.h',
    ]
  cp files, 'src/',
    :preserve => true,
    :verbose => true
end

file 'sundown/src/markdown.h' do |t|
  abort "The Sundown submodule is required."
end

