
desc "compile php-sundown"
task :compile => ['sundown/src/markdown.h'] do
	sh "phpize"
	sh "./configure"
	sh "make"
end

desc "install php-sundown to your php"
task :install => [:compile] do
	sh "make install"
end

task :clean do
	sh "phpize --clean"
end

desc "run php test cases"
task :test do
	ENV["TESTS"] = "--show-diff -q"
	sh "make test"
end

desc "Run conformance tests"
task "test:conformance" do
	Dir.chdir("conformance_test") do
		script = "#{pwd}/Markdown.sh"
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
  cp files, './',
    :preserve => true,
    :verbose => true
end

file 'sundown/src/markdown.h' do |t|
  abort "The Sundown submodule is required."
end

task :submodule do
  sh "git submodule init"
  sh "git submodule update"
  Rake::Task['gather'].invoke
end
