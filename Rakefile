ENV["LANG"]="C"

# auto-detect maximum c compiler
# TODO: should go to cmake.rake as well
[
#  "4.8.1",
  "4.7.3", "4.7.2",
  "4.6.3",
].each do |ver|
  if system("g++-#{ver} --version")
    # puts "setting gcc-#{ver} as preferred compiler"
    ENV["CXX"]="g++-#{ver}"
    ENV["CC"]="gcc-#{ver}"
    ENV["GCOV"]="gcov-#{ver}"
    break
  end
end
@run_arguments = "--testrun=showcase"
@editfiles = FileList.new(["README.ROADMAP", "**/test_*.cc"])

load "devsupport/tasks/cmake.rake"

@cmake_options += " -DDEBUG=ON"

# TODO: Put this to CMake by any means
namespace :doc do
  desc "build doxygen documentation"
  task :doxygen do
    FileUtils.rm_rf 'doxygen' if File.exists? 'doxygen'
    FileUtils.mkdir 'doxygen'
    sh "doxygen"
    puts "now reload Your brower, or point it to"
    puts "file:///home/marian/pm-git/software/mguardd/trunk/doxygen/html/index.html"
    puts "doxygen/html/index.html"
  end

  desc "build all documentation for jenkins"
  task :all => [ :doxygen ]
end

