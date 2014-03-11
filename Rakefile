ENV["LANG"]="C"
load "devsupport/tasks/setup.rb"

ds_configure do |c|
  c.gcc_versions = [
  "4.8.2",
  "4.7.3", "4.7.2", "4.7",
  "4.6.3", ]
  c.run_arguments = "--testrun=showcase"
  c.editfiles = FileList.new("README.ROADMAP", "**/test_*.cc")
  c.cmake_options = "-DDEBUG=ON"
end

ds_tasks_for :cmake
ds_cmake_configure

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

